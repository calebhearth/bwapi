#include "Server.h"
#include <Util/Logger.h>
#include "GameImpl.h"
#include "UnitImpl.h"
#include <BWAPI/Client/GameData.h>
namespace BWAPI
{
  #define PIPE_TIMEOUT 2000
  #define PIPE_SYSTEM_BUFFER_SIZE 4096
  Server::Server()
  {
    pipeObjectHandle=CreateNamedPipe("\\\\.\\pipe\\bwapi_pipe",
                                           PIPE_ACCESS_DUPLEX,
                                           PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_NOWAIT,
                                           PIPE_UNLIMITED_INSTANCES,
                                           PIPE_SYSTEM_BUFFER_SIZE,
                                           PIPE_SYSTEM_BUFFER_SIZE,
                                           PIPE_TIMEOUT,
                                           NULL);
    if (pipeObjectHandle == INVALID_HANDLE_VALUE)
      Util::Logger::globalLog->log("Error: unable to make pipe");
    COMMTIMEOUTS c;
    c.ReadIntervalTimeout = 100;
    c.ReadTotalTimeoutMultiplier = 100;
    c.ReadTotalTimeoutConstant = 2000;
    c.WriteTotalTimeoutMultiplier = 100;
    c.WriteTotalTimeoutConstant = 2000;
    SetCommTimeouts(pipeObjectHandle,&c);
    int size=sizeof(BWAPIC::GameData);
    mapFileHandle = CreateFileMapping( INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, "Global\\bwapi_shared_memory");
    if (mapFileHandle == INVALID_HANDLE_VALUE)
      Util::Logger::globalLog->log("Error: unable to make shared memory");
    data = (BWAPIC::GameData*) MapViewOfFile(mapFileHandle, FILE_MAP_ALL_ACCESS, 0, 0, size);
    initializeSharedMemory();
    connected=false;
  }
  Server::~Server()
  {
    DisconnectNamedPipe(pipeObjectHandle);
  }
  void Server::update()
  {
    if (connected)
    {
      updateSharedMemory();
      callOnFrame();
      processCommands();
    }
    else
    {
      data->eventCount = 0;
      ((GameImpl*)Broodwar)->events.clear();
      checkForConnections();
    }
    data->commandCount     = 0;
    data->unitCommandCount = 0;
    data->shapeCount       = 0;
    data->stringCount      = 0;
  }
  bool Server::isConnected()
  {
    return connected;
  }
  int Server::addString(const char* text)
  {
    strncpy(data->strings[data->stringCount],text,256);
    return data->stringCount++;
  }
  int Server::addEvent(BWAPI::Event e)
  {
    BWAPIC::Event* e2=&(data->events[data->eventCount]);
    data->eventCount++;
    int id=data->eventCount;
    e2->type=e.type;
    e2->v1=0;
    e2->v2=0;
    if (e.type==BWAPI::EventType::MatchEnd)
      e2->v1=e.isWinner;

    if (e.type==BWAPI::EventType::SendText || e.type==BWAPI::EventType::SaveGame)
    {
      int str=addString(e.text.c_str());
      e2->v1=str;
    }

    if (e.type==BWAPI::EventType::PlayerLeft)
      e2->v1=getPlayerID(e.player);

    if (e.type==BWAPI::EventType::ReceiveText)
    {
      e2->v1=getPlayerID(e.player);
      int str=addString(e.text.c_str());
      e2->v2=str;
    }

    if (e.type==BWAPI::EventType::NukeDetect)
    {
      e2->v1=e.position.x();
      e2->v2=e.position.y();
    }

    if (e.type==BWAPI::EventType::UnitCreate ||
        e.type==BWAPI::EventType::UnitDestroy ||
        e.type==BWAPI::EventType::UnitMorph ||
        e.type==BWAPI::EventType::UnitShow ||
        e.type==BWAPI::EventType::UnitHide ||
        e.type==BWAPI::EventType::UnitRenegade)
      e2->v1=getUnitID(e.unit);
    return id;
  }

  void Server::setWaitForResponse(bool wait)
  {
    DWORD dwMode = PIPE_READMODE_MESSAGE;
    if (wait)
      dwMode |= PIPE_WAIT;
    else
      dwMode |= PIPE_NOWAIT;
    SetNamedPipeHandleState(pipeObjectHandle,&dwMode,NULL,NULL);
  }
  void Server::checkForConnections()
  {
    if (connected) return;
    BOOL success = ConnectNamedPipe(pipeObjectHandle, NULL);
    if (!success && GetLastError() != ERROR_PIPE_CONNECTED) return;
    if (GetLastError() == ERROR_PIPE_CONNECTED)
      connected = true;
    if (!connected) return;
    setWaitForResponse(true);
  }
  void Server::initializeSharedMemory()
  {
    //called once when Starcraft starts. Not at the start of every match.
    data->eventCount       = 0;
    data->commandCount     = 0;
    data->unitCommandCount = 0;
    data->shapeCount       = 0;
    data->stringCount      = 0;
    clearAll();
    strncpy(data->mapFilename,"",260);
    strncpy(data->mapName,"",32);
  }
  void Server::onMatchStart()
  {
    data->self          = getPlayerID(Broodwar->self());
    data->isMultiplayer = Broodwar->isMultiplayer();
    data->isBattleNet   = Broodwar->isBattleNet();
    data->isReplay      = Broodwar->isReplay();

    //load static map data
    for(int x=0;x<Broodwar->mapWidth()*4;x++)
      for(int y=0;y<Broodwar->mapHeight()*4;y++)
      {
        data->isWalkable[x][y]      = Broodwar->isWalkable(x,y);
      }

    for(int x=0;x<Broodwar->mapWidth();x++)
      for(int y=0;y<Broodwar->mapHeight();y++)
      {
        data->isBuildable[x][y] = Broodwar->isBuildable(x,y);
        data->getGroundHeight[x][y] = Broodwar->getGroundHeight(x,y);
      }

    data->mapWidth       = Broodwar->mapWidth();
    data->mapHeight      = Broodwar->mapHeight();
    strncpy(data->mapFilename,Broodwar->mapFilename().c_str(),260);
    strncpy(data->mapName,Broodwar->mapName().c_str(),32);
    data->mapHash        = Broodwar->getMapHash();

    data->startLocationCount = Broodwar->getStartLocations().size();
    int i=0;
    for(std::set< TilePosition >::iterator t=Broodwar->getStartLocations().begin();t!=Broodwar->getStartLocations().end();t++)
    {
      data->startLocationsX[i] = (*t).x();
      data->startLocationsY[i] = (*t).y();
      i++;
    }
    //static force data
    for(std::set<Force*>::iterator i=Broodwar->getForces().begin();i!=Broodwar->getForces().end();i++)
    {
      int id=getForceID(*i);
      strncpy(data->forces[id].name,(*i)->getName().c_str(),32);
    }
    //static player data
    for(std::set<Player*>::iterator i=Broodwar->getPlayers().begin();i!=Broodwar->getPlayers().end();i++)
    {
      int id=getPlayerID(*i);
      BWAPIC::PlayerData* p=&(data->players[id]);

      strncpy(p->name,(*i)->getName().c_str(),32);
      p->race  = (*i)->getRace().getID();
      p->type  = (*i)->getType().getID();
      p->force = getForceID((*i)->getForce());
      for(int j=0;j<12;j++)
      {
        p->isAlly[j]  = false;
        p->isEnemy[j] = false;
      }
      for(std::set<Player*>::iterator j=Broodwar->getPlayers().begin();j!=Broodwar->getPlayers().end();j++)
      {
        p->isAlly[getPlayerID(*j)]  = (*i)->isAlly(*j);
        p->isEnemy[getPlayerID(*j)] = (*i)->isEnemy(*j);
      }
      p->isNeutral          = (*i)->isNeutral();
      p->startLocationX     = (*i)->getStartLocation().x();
      p->startLocationY     = (*i)->getStartLocation().y();
    }

    data->forceCount       = forceVector.size();
    data->playerCount      = playerVector.size();
    data->initialUnitCount = unitVector.size();
  }
  void Server::clearAll()
  {
    //clear force info
    data->forceCount=0;
    forceVector.clear();
    forceLookup.clear();

    //clear player info
    data->playerCount=0;
    playerVector.clear();
    playerLookup.clear();

    //clear unit info
    data->initialUnitCount=0;
    unitVector.clear();
    unitLookup.clear();
  }

  void Server::updateSharedMemory()
  {
    data->eventCount = 0;
    bool matchStarting = false;
    for(std::list<Event>::iterator e=((GameImpl*)Broodwar)->events.begin();e!=((GameImpl*)Broodwar)->events.end();e++)
    {
      addEvent(*e);
      if (e->type == EventType::MatchStart)
      {
        matchStarting = true;
      }
    }
    if (matchStarting)
      clearAll();

    ((GameImpl*)Broodwar)->events.clear();

    data->frameCount = Broodwar->getFrameCount();
    data->fps        = Broodwar->getFPS();
    data->averageFPS = Broodwar->getAverageFPS();
    data->mouseX     = Broodwar->getMouseX();
    data->mouseY     = Broodwar->getMouseY();
    data->isInGame   = Broodwar->isInGame();
    if (Broodwar->isInGame())
    {
      data->gameType          = Broodwar->getGameType().getID();
      data->latency           = Broodwar->getLatency();
      for(int i=0;i<3;i++)
        data->mouseState[i]   = Broodwar->getMouseState(i);
      for(int i=0;i<256;i++)
        data->keyState[i]     = Broodwar->getKeyState(i);
      data->screenX           = Broodwar->getScreenX();
      data->screenY           = Broodwar->getScreenY();
      data->flags[0]          = Broodwar->isFlagEnabled(0);
      data->flags[1]          = Broodwar->isFlagEnabled(1);
      data->isPaused          = Broodwar->isPaused();
      data->selectedUnitCount = Broodwar->getSelectedUnits().size();
      int i=0;
      for(std::set< Unit* >::iterator t=Broodwar->getSelectedUnits().begin();t!=Broodwar->getSelectedUnits().end();t++)
      {
        data->selectedUnits[i] = getUnitID(*t);
        i++;
      }

      //dynamic map data
      for(int x=0;x<Broodwar->mapWidth();x++)
      {
        for(int y=0;y<Broodwar->mapHeight();y++)
        {
          data->isVisible[x][y]  = Broodwar->isVisible(x,y);
          data->isExplored[x][y] = Broodwar->isExplored(x,y);
          data->hasCreep[x][y]   = Broodwar->hasCreep(x,y);
        }
      }
      //(no dynamic force data)

      //dynamic player data
      for(std::set<Player*>::iterator i=Broodwar->getPlayers().begin();i!=Broodwar->getPlayers().end();i++)
      {
        int id                = getPlayerID(*i);
        BWAPIC::PlayerData* p = &(data->players[id]);

        p->isVictorious       = (*i)->isVictorious();
        p->isDefeated         = (*i)->isDefeated();
        p->leftGame           = (*i)->leftGame();
        p->minerals           = (*i)->minerals();
        p->gas                = (*i)->gas();
        p->cumulativeMinerals = (*i)->cumulativeMinerals();
        p->cumulativeGas      = (*i)->cumulativeGas();
        for(int j=0;j<3;j++)
        {
          p->supplyTotal[j]   = (*i)->supplyTotal(Race(0));
          p->supplyUsed[j]    = (*i)->supplyUsed(Race(0));
        }
        for(int j=0;j<228;j++)
        {
          p->allUnitCount[j]       = (*i)->allUnitCount(UnitType(j));
          p->completedUnitCount[j] = (*i)->completedUnitCount(UnitType(j));
          p->deadUnitCount[j]      = (*i)->deadUnitCount(UnitType(j));
          p->killedUnitCount[j]    = (*i)->killedUnitCount(UnitType(j));
        }
        for(int j=0;j<63;j++)
        {
          p->upgradeLevel[j] = (*i)->getUpgradeLevel(UpgradeType(j));
          p->isUpgrading[j]  = (*i)->isUpgrading(UpgradeType(j));
        }
        for(int j=0;j<47;j++)
        {
          p->hasResearched[j] = (*i)->hasResearched(TechType(j));
          p->isResearching[j] = (*i)->isResearching(TechType(j));
        }
      }

      //dynamic unit data
      for(std::set<Unit*>::iterator i=Broodwar->getAllUnits().begin();i!=Broodwar->getAllUnits().end();i++)
      {
        int id              = getUnitID(*i);
        BWAPIC::UnitData* u = &(data->units[id]);
        BWAPIC::UnitData* u2= ((UnitImpl*)(*i))->self;
        u->player           = u2->player;
        u->type             = u2->type;
        u->positionX        = u2->positionX;
        u->positionY        = u2->positionY;
        u->angle            = u2->angle;
        u->velocityX        = u2->velocityX;
        u->velocityY        = u2->velocityY;
        u->hitPoints        = u2->hitPoints;
        u->shields          = u2->shields;
        u->energy           = u2->energy;
        u->resources        = u2->resources;

        u->killCount            = u2->killCount;
        u->scarabCount          = u2->scarabCount;
        u->spiderMineCount      = u2->spiderMineCount;
        u->groundWeaponCooldown = u2->groundWeaponCooldown;
        u->airWeaponCooldown    = u2->airWeaponCooldown;
        u->spellCooldown        = u2->spellCooldown;
        u->defenseMatrixPoints  = u2->defenseMatrixPoints;

        u->defenseMatrixTimer = u2->defenseMatrixTimer;
        u->ensnareTimer       = u2->ensnareTimer;
        u->irradiateTimer     = u2->irradiateTimer;
        u->lockdownTimer      = u2->lockdownTimer;
        u->maelstromTimer     = u2->maelstromTimer;
        u->orderTimer         = u2->orderTimer;
        u->plagueTimer        = u2->plagueTimer;
        u->removeTimer        = u2->removeTimer;
        u->stasisTimer        = u2->stasisTimer;
        u->stimTimer          = u2->stimTimer;

        u->buildType             = u2->buildType;
        u->trainingQueueCount    = u2->trainingQueueCount;
        for(int s=0;s<5;s++)
          u->trainingQueue[s]    = u2->trainingQueue[s];
        u->tech                  = u2->tech;
        u->upgrade               = u2->upgrade;
        u->remainingBuildTime    = u2->remainingBuildTime;
        u->remainingTrainTime    = u2->remainingTrainTime;
        u->remainingResearchTime = u2->remainingResearchTime;
        u->remainingUpgradeTime  = u2->remainingUpgradeTime;
        u->buildUnit             = u2->buildUnit;

        u->target                = u2->target;
        u->targetPositionX       = u2->targetPositionX;
        u->targetPositionY       = u2->targetPositionY;
        u->order                 = u2->order;
        u->orderTarget           = u2->orderTarget;
        u->secondaryOrder        = u2->secondaryOrder;
        u->child                 = u2->child;
        u->rallyPositionX        = u2->rallyPositionX;
        u->rallyPositionY        = u2->rallyPositionY;
        u->rallyUnit             = u2->rallyUnit;
        u->addon                 = u2->addon;
        u->nydusExit             = u2->nydusExit;

        u->transport             = u2->transport;
        u->carrier               = u2->carrier;
        u->hatchery              = u2->hatchery;

        u->exists              = u2->exists;
        u->hasNuke             = u2->hasNuke;
        u->isAccelerating      = u2->isAccelerating;
        u->isAttacking         = u2->isAttacking;
        u->isBeingGathered     = u2->isBeingGathered;
        u->isBeingHealed       = u2->isBeingHealed;
        u->isBlind             = u2->isBlind;
        u->isBraking           = u2->isBraking;
        u->isBurrowed          = u2->isBurrowed;
        u->carryResourceType   = u2->carryResourceType;
        u->isCloaked           = u2->isCloaked;
        u->isCompleted         = u2->isCompleted;
        u->isConstructing      = u2->isConstructing;
        u->isHallucination     = u2->isHallucination;
        u->isIdle              = u2->isIdle;
        u->isLifted            = u2->isLifted;
        u->isMorphing          = u2->isMorphing;
        u->isMoving            = u2->isMoving;
        u->isParasited         = u2->isParasited;
        u->isSelected          = u2->isSelected;
        u->isStartingAttack    = u2->isStartingAttack;
        u->isTraining          = u2->isTraining;
        u->isUnderStorm        = u2->isUnderStorm;
        u->isUnpowered         = u2->isUnpowered;
        for(int s=0;s<9;s++)
          u->isVisible[s]      = u2->isVisible[s];
      }

      for(int i=0;i<1700;i++)
        data->unitArray[i] = getUnitID(Broodwar->indexToUnit(i));

      //dynamic bullet data
      for(int id=0;id<100;id++)
      {
        BWAPIC::BulletData* b = &(data->bullets[id]);
        Bullet* b2 = (Bullet*)BroodwarImpl.getBullet(id);
        b->exists = b2->exists();
        if (b->exists)
        {
          b->id              = b2->getID();
          b->player          = getPlayerID(b2->getPlayer());
          b->type            = b2->getType().getID();
          b->source          = getUnitID(b2->getSource());
          b->positionX       = b2->getPosition().x();
          b->positionY       = b2->getPosition().y();
          b->angle           = b2->getAngle();
          b->velocityX       = b2->getVelocityX();
          b->velocityY       = b2->getVelocityY();
          b->target          = getUnitID(b2->getTarget());
          b->targetPositionX = b2->getTargetPosition().x();
          b->targetPositionY = b2->getTargetPosition().y();
          b->removeTimer = b2->getRemoveTimer();
          for(std::set<Player*>::iterator j=Broodwar->getPlayers().begin();j!=Broodwar->getPlayers().end();j++)
          {
            int playerid = getPlayerID(*j);
            b->isVisible[playerid] = b2->isVisible(*j);
          }
        }
      }
    }
    if (matchStarting)
      Server::onMatchStart();
  }

  int Server::getForceID(Force* force)
  {
    if (force==NULL) return -1;
    if (forceLookup.find(force)==forceLookup.end())
    {
      forceLookup[force]=forceVector.size();
      forceVector.push_back(force);
    }
    return forceLookup[force];
  }
  Force* Server::getForce(int id)
  {
    if ((int)forceVector.size()<=id || id<0)
      return NULL;
    return forceVector[id];
  }

  int Server::getPlayerID(Player* player)
  {
    if (player==NULL) return -1;
    if (playerLookup.find(player)==playerLookup.end())
    {
      playerLookup[player]=playerVector.size();
      playerVector.push_back(player);
    }
    return playerLookup[player];
  }
  Player* Server::getPlayer(int id)
  {
    if ((int)playerVector.size()<=id || id<0)
      return NULL;
    return playerVector[id];
  }

  int Server::getUnitID(Unit* unit)
  {
    if (unit==NULL) return -1;
    if (unitLookup.find(unit)==unitLookup.end())
    {
      unitLookup[unit]=unitVector.size();
      unitVector.push_back(unit);
    }
    return unitLookup[unit];
  }
  Unit* Server::getUnit(int id)
  {
    if ((int)unitVector.size()<=id || id<0)
      return NULL;
    return unitVector[id];
  }

  void Server::callOnFrame()
  { 
    DWORD writtenByteCount;
    int code=3;
    WriteFile(pipeObjectHandle,&code,sizeof(int),&writtenByteCount,NULL);
    while (code!=4)
    {
      DWORD receivedByteCount;
      BOOL success = ReadFile(pipeObjectHandle,&code,sizeof(int),&receivedByteCount,NULL);
      if (!success)
      {
        DisconnectNamedPipe(pipeObjectHandle);
        connected=false;
        setWaitForResponse(false);
        break;
      }
    }
  }
  void Server::processCommands()
  {
    for(int i=0;i<data->commandCount;i++)
    {
      BWAPIC::CommandType::Enum c=data->commands[i].type;
      int v1=data->commands[i].value1;
      int v2=data->commands[i].value2;
      switch (c)
      {
        case BWAPIC::CommandType::SetScreenPosition:
          if (Broodwar->isInGame())
            Broodwar->setScreenPosition(v1,v2);
        break;
        case BWAPIC::CommandType::PingMinimap:
          if (Broodwar->isInGame())
            Broodwar->pingMinimap(v1,v2);
        break;
        case BWAPIC::CommandType::EnableFlag:
          if (Broodwar->isInGame())
            Broodwar->enableFlag(v1);
        break;
        case BWAPIC::CommandType::Printf:
          if (Broodwar->isInGame())
            Broodwar->printf(data->strings[v1]);
        break;
        case BWAPIC::CommandType::SendText:
          if (Broodwar->isInGame())
            Broodwar->sendText(data->strings[v1]);
        break;
        case BWAPIC::CommandType::ChangeRace:
          Broodwar->changeRace(Race(v1));
        break;
        case BWAPIC::CommandType::StartGame:
          Broodwar->startGame();
        break;
        case BWAPIC::CommandType::PauseGame:
          if (Broodwar->isInGame())
            Broodwar->pauseGame();
        break;
        case BWAPIC::CommandType::ResumeGame:
          if (Broodwar->isInGame())
            Broodwar->resumeGame();
        break;
        case BWAPIC::CommandType::LeaveGame:
          if (Broodwar->isInGame())
            Broodwar->leaveGame();
        break;
        case BWAPIC::CommandType::RestartGame:
          if (Broodwar->isInGame())
            Broodwar->restartGame();
        break;
        case BWAPIC::CommandType::SetLocalSpeed:
          if (Broodwar->isInGame())
            Broodwar->setLocalSpeed(v1);
        break;
        case BWAPIC::CommandType::SetTextSize:
          if (Broodwar->isInGame())
            Broodwar->setTextSize(v1);
        break;
        default:
        break;
      }
    }
    if (Broodwar->isInGame())
    {
      for(int i=0;i<data->unitCommandCount;i++)
      {
        if (data->unitCommands[i].unitIndex<0 || data->unitCommands[i].unitIndex>=(int)unitVector.size()) continue;
        Unit* unit=unitVector[data->unitCommands[i].unitIndex];
        Unit* target=NULL;
        if (data->unitCommands[i].targetIndex>=0 && data->unitCommands[i].targetIndex<(int)unitVector.size())
        {
          target=unitVector[data->unitCommands[i].targetIndex];
        }
        unit->issueCommand(UnitCommand(data->unitCommands[i].type,target,data->unitCommands[i].x,data->unitCommands[i].y,data->unitCommands[i].extra));
      }
      for(int i=0;i<data->shapeCount;i++)
      {
        BWAPIC::ShapeType::Enum s=data->shapes[i].type;
        switch (s)
        {
          case BWAPIC::ShapeType::Text:
            Broodwar->drawText(data->shapes[i].ctype,data->shapes[i].x1,data->shapes[i].y1,data->strings[data->shapes[i].extra1]);
          break;
          case BWAPIC::ShapeType::Box:
            Broodwar->drawBox(data->shapes[i].ctype,data->shapes[i].x1,data->shapes[i].y1,data->shapes[i].x2,data->shapes[i].y2,Color(data->shapes[i].color),data->shapes[i].isSolid);
          break;
          case BWAPIC::ShapeType::Triangle:
            Broodwar->drawTriangle(data->shapes[i].ctype,data->shapes[i].x1,data->shapes[i].y1,data->shapes[i].x2,data->shapes[i].y2,data->shapes[i].extra1,data->shapes[i].extra2,Color(data->shapes[i].color),data->shapes[i].isSolid);
          break;
          case BWAPIC::ShapeType::Circle:
            Broodwar->drawCircle(data->shapes[i].ctype,data->shapes[i].x1,data->shapes[i].y1,data->shapes[i].extra1,Color(data->shapes[i].color),data->shapes[i].isSolid);
          break;
          case BWAPIC::ShapeType::Ellipse:
            Broodwar->drawEllipse(data->shapes[i].ctype,data->shapes[i].x1,data->shapes[i].y1,data->shapes[i].extra1,data->shapes[i].extra2,Color(data->shapes[i].color),data->shapes[i].isSolid);
          break;
          case BWAPIC::ShapeType::Dot:
            Broodwar->drawDot(data->shapes[i].ctype,data->shapes[i].x1,data->shapes[i].y1,Color(data->shapes[i].color));
          break;
          case BWAPIC::ShapeType::Line:
            Broodwar->drawLine(data->shapes[i].ctype,data->shapes[i].x1,data->shapes[i].y1,data->shapes[i].x2,data->shapes[i].y2,Color(data->shapes[i].color));
          break;
          default:
          break;
        }
      }
    }
  }
}