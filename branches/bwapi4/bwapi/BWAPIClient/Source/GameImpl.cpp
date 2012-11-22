#include <BWAPI.h>
#include <BWAPI/Client/GameImpl.h>
#include <BWAPI/Client/ForceImpl.h>
#include <BWAPI/Client/PlayerImpl.h>
#include <BWAPI/Client/RegionImpl.h>
#include <BWAPI/Client/UnitImpl.h>
#include <BWAPI/Client/BulletImpl.h>

#include "TemplatesImpl.h"

#include <Util/Foreach.h>
#include <Util/Types.h>
#include <string>
#include <cassert>

#include <BWAPI/Unitset.h>

namespace BWAPI
{
  Game* BroodwarPtr = nullptr;
  GameImpl::GameImpl(GameData* data)
  {
    this->data = data;
    for(int i = 0; i < 5; ++i)
      forceVector.push_back(ForceImpl(i));
    for(int i = 0; i < 12; ++i)
      playerVector.push_back(PlayerImpl(i));
    for(int i = 0; i < 10000; ++i)
      unitVector.push_back(UnitImpl(i));
    for(int i = 0; i < 100; ++i)
      bulletVector.push_back(BulletImpl(i));
    inGame = false;
  }
  int GameImpl::addShape(const BWAPIC::Shape &s)
  {
    assert(data->shapeCount < GameData::MAX_SHAPES);
    data->shapes[data->shapeCount] = s;
    return data->shapeCount++;
  }
  int GameImpl::addString(const char* text)
  {
    assert(data->stringCount < GameData::MAX_STRINGS);
    strncpy(data->strings[data->stringCount], text, 255);
    data->strings[data->stringCount][sizeof(data->strings[0])-1] = 0; // Null-terminate the string
    return data->stringCount++;
  }
  int GameImpl::addText(BWAPIC::Shape &s, const char* text)
  {
    s.extra1 = addString(text);
    return addShape(s);
  }
  int GameImpl::addCommand(const BWAPIC::Command &c)
  {
    assert(data->commandCount < GameData::MAX_COMMANDS);
    data->commands[data->commandCount] = c;
    return data->commandCount++;
  }
  int GameImpl::addUnitCommand(BWAPIC::UnitCommand& c)
  {
    assert(data->unitCommandCount < GameData::MAX_UNIT_COMMANDS);
    data->unitCommands[data->unitCommandCount] = c;
    return data->unitCommandCount++;
  }
  Unit *GameImpl::_unitFromIndex(int index)
  {
    return this->getUnit(index);
  }
  Event GameImpl::makeEvent(BWAPIC::Event e)
  {
    Event e2;
    e2.setType(e.type);
    if (e.type == EventType::MatchEnd)
      e2.setWinner(e.v1 != 0);
    if (e.type == EventType::NukeDetect)
      e2.setPosition(Position(e.v1,e.v2));
    if (e.type == EventType::PlayerLeft)
      e2.setPlayer(getPlayer(e.v1));
    if (e.type == EventType::SaveGame || e.type == EventType::SendText)
      e2.setText(data->eventStrings[e.v1]);
    if (e.type == EventType::ReceiveText)
    {
      e2.setPlayer(getPlayer(e.v1));
      e2.setText(data->eventStrings[e.v2]);
    }
    if (e.type == EventType::UnitDiscover ||
        e.type == EventType::UnitEvade ||
        e.type == EventType::UnitShow ||
        e.type == EventType::UnitHide ||
        e.type == EventType::UnitCreate ||
        e.type == EventType::UnitDestroy ||
        e.type == EventType::UnitRenegade ||
        e.type == EventType::UnitMorph ||
        e.type == EventType::UnitComplete )
      e2.setUnit(getUnit(e.v1));
    return e2;

  }
  void GameImpl::clearAll()
  {
    //clear everything
    inGame = false;
    startLocations.clear();
    forces.clear();
    playerSet.clear();
    bullets.clear();
    accessibleUnits.clear();
    minerals.clear();
    geysers.clear();
    neutralUnits.clear();
    staticMinerals.clear();
    staticGeysers.clear();
    staticNeutralUnits.clear();
    selectedUnits.clear();
    pylons.clear();
    events.clear();
    thePlayer  = NULL;
    theEnemy   = NULL;
    theNeutral = NULL;
    _allies.clear();
    _enemies.clear();
    _observers.clear();

    //clear unit data
    for(int i = 0; i < 10000; ++i)
      unitVector[i].clear();

    //clear player data
    for(int i = 0; i < 12; ++i)
      playerVector[i].units.clear();

    foreach( Region *r, regionsList )
      delete ((RegionImpl*)r);
    regionsList.clear();
    memset(this->regionArray, 0, sizeof(this->regionArray));
  }
  //------------------------------------------------- ON MATCH START -----------------------------------------
  void GameImpl::onMatchStart()
  {
    clearAll();
    inGame = true;

    //load forces, players, and initial units from shared memory
    for(int i = 1; i < data->forceCount; ++i)
      forces.insert(&forceVector[i]);
    for(int i = 0; i < data->playerCount; ++i)
      playerSet.insert(&playerVector[i]);
    for(int i = 0; i < data->initialUnitCount; ++i)
    {
      accessibleUnits.insert(&unitVector[i]);
      //save the initial state of each initial unit
      unitVector[i].saveInitialState();
    }

    //load start locations from shared memory
    for(int i = 0; i < data->startLocationCount; ++i)
      startLocations.push_back(BWAPI::TilePosition(data->startLocations[i].x,data->startLocations[i].y));

    for ( int i = 0; i < data->regionCount; ++i )
    {
      this->regionArray[i] = new RegionImpl(i);
      regionsList.insert(this->regionArray[i]);
    }
    for ( int i = 0; i < data->regionCount; ++i )
      this->regionArray[i]->setNeighbors();

    thePlayer  = getPlayer(data->self);
    theEnemy   = getPlayer(data->enemy);
    theNeutral = getPlayer(data->neutral);
    _allies.clear();
    _enemies.clear();
    _observers.clear();
    // check if the current player exists
    if ( thePlayer )
    {
      // iterate each player
      foreach(Player* p, playerSet)
      {
        // check if the player should not be updated
        if ( p->leftGame() || p->isDefeated() || p == thePlayer )
          continue;
        // add player to allies set
        if ( thePlayer->isAlly(p) )
          _allies.insert(p);
        // add player to enemies set
        if ( thePlayer->isEnemy(p) )
          _enemies.insert(p);
        // add player to observers set
        if ( p->isObserver() )
          _observers.insert(p);
      }
    }
    onMatchFrame();
    staticMinerals = minerals;
    staticGeysers = geysers;
    staticNeutralUnits = neutralUnits;
    textSize = 1;
  }
  //------------------------------------------------- ON MATCH END -------------------------------------------
  void GameImpl::onMatchEnd()
  {
    clearAll();
  }
  //------------------------------------------------- ON MATCH FRAME -----------------------------------------
  void GameImpl::onMatchFrame()
  {
    events.clear();
    bullets.clear();
    for(int i = 0; i < 100; ++i)
    {
      if (bulletVector[i].exists())
        bullets.insert(&bulletVector[i]);
    }
    nukeDots.clear();
    for(int i = 0; i < data->nukeDotCount; ++i)
      nukeDots.insert(Position(data->nukeDots[i].x,data->nukeDots[i].y));

    for(int e = 0; e < data->eventCount; ++e)
    {
      events.push_back(this->makeEvent(data->events[e]));
      int id = data->events[e].v1;
      if (data->events[e].type == EventType::UnitDiscover)
      {
        Unit* u = &unitVector[id];
        accessibleUnits.insert(u);
        ((PlayerImpl*)u->getPlayer())->units.insert(u);
        if (u->getPlayer()->isNeutral())
        {
          neutralUnits.insert(u);
          if ( u->getType().isMineralField() )
            minerals.insert(u);
          else if ( u->getType() == UnitTypes::Resource_Vespene_Geyser )
            geysers.insert(u);
        }
        else
        {
          if (u->getPlayer() == Broodwar->self() && u->getType() == UnitTypes::Protoss_Pylon)
            pylons.insert(u);
        }
      }
      else if (data->events[e].type == EventType::UnitEvade)
      {
        Unit* u = &unitVector[id];
        accessibleUnits.erase(u);
        ((PlayerImpl*)u->getPlayer())->units.erase(u);
        if (u->getPlayer()->isNeutral())
        {
          neutralUnits.erase(u);
          if ( u->getType().isMineralField() )
            minerals.erase(u);
          else if (u->getType()==UnitTypes::Resource_Vespene_Geyser)
            geysers.erase(u);
        }
        else
        {
          if (u->getPlayer() == Broodwar->self() && u->getType() == UnitTypes::Protoss_Pylon)
            pylons.erase(u);
        }
      }
      else if (data->events[e].type==EventType::UnitRenegade)
      {
        Unit* u = &unitVector[id];
        for (auto p = playerSet.begin(); p != playerSet.end(); ++p )
          ((PlayerImpl*)*p)->units.erase(u);
        ((PlayerImpl*)u->getPlayer())->units.insert(u);
      }
      else if (data->events[e].type == EventType::UnitMorph)
      {
        Unit* u = &unitVector[id];
        if (u->getType() == UnitTypes::Resource_Vespene_Geyser)
        {
          geysers.insert(u);
          neutralUnits.insert(u);
        }
        else if (u->getType().isRefinery())
        {
          geysers.erase(u);
          neutralUnits.erase(u);
        }
      }
    }
    foreach(Unit* u_, accessibleUnits)
    {
      UnitImpl* u = static_cast<UnitImpl*>(u_);
      u->connectedUnits.clear();
      u->loadedUnits.clear();
    }
    foreach(Unit* u, accessibleUnits)
    {
      if ( u->getType() == UnitTypes::Zerg_Larva && u->getHatchery() )
        ((UnitImpl*)u->getHatchery())->connectedUnits.insert(u);
      if ( u->getType() == UnitTypes::Protoss_Interceptor && u->getCarrier() )
        ((UnitImpl*)u->getCarrier())->connectedUnits.insert(u);
      if ( u->getTransport() )
        ((UnitImpl*)u->getTransport())->loadedUnits.insert(u);
    }
    selectedUnits.clear();
    for ( int i = 0; i < data->selectedUnitCount; ++i )
    {
      Unit* u = getUnit(data->selectedUnits[i]);
      if ( u )
        selectedUnits.insert(u);
    }
    // clear player sets
    _allies.clear();
    _enemies.clear();
    _observers.clear();
    if ( thePlayer )
    {
      // iterate each player
      foreach(Player* p, playerSet)
      {
        // check if player should be skipped
        if ( p->leftGame() || p->isDefeated() || p == thePlayer )
          continue;
        // add player to allies set
        if ( thePlayer->isAlly(p) )
          _allies.insert(p);
        // add player to enemy set
        if ( thePlayer->isEnemy(p) )
          _enemies.insert(p);
        // add player to obs set
        if ( p->isObserver() )
          _observers.insert(p);
      }
    }
  }
  //----------------------------------------------- GET FORCE ------------------------------------------------
  Force* GameImpl::getForce(int forceId) const
  {
    if (forceId < 0 || forceId >= (int)forceVector.size())
      return NULL;
    return (Force*)(&forceVector[forceId]);
  }
  Region *GameImpl::getRegion(int regionID) const
  {
    if ( regionID < 0 || regionID >= data->regionCount )
      return NULL;
    return regionArray[regionID];
  }
  //----------------------------------------------- GET PLAYER -----------------------------------------------
  Player* GameImpl::getPlayer(int playerId) const
  {
    if (playerId < 0 || playerId >= (int)playerVector.size())
      return NULL;
    return (Player*)(&playerVector[playerId]);
  }
  //----------------------------------------------- GET UNIT -------------------------------------------------
  Unit* GameImpl::getUnit(int unitId) const
  {
    if (unitId < 0 || unitId >= (int)unitVector.size())
      return NULL;
    return (Unit*)(&unitVector[unitId]);
  }
  //----------------------------------------------- INDEX TO UNIT --------------------------------------------
  Unit* GameImpl::indexToUnit(int unitIndex) const
  {
    if ( unitIndex < 0 || unitIndex >= 1700 )
      return NULL;
    return getUnit(data->unitArray[unitIndex]);
  }
  //--------------------------------------------- GET GAME TYPE ----------------------------------------------
  GameType GameImpl::getGameType() const
  {
    return GameType(data->gameType);
  }
  //---------------------------------------------- GET LATENCY -----------------------------------------------
  int GameImpl::getLatency() const
  {
    return data->latency;
  }
  //--------------------------------------------- GET FRAME COUNT --------------------------------------------
  int GameImpl::getFrameCount() const
  {
    return data->frameCount;
  }
  //--------------------------------------------- GET REPLAY FRAME COUNT -------------------------------------
  int GameImpl::getReplayFrameCount() const
  {
    return data->replayFrameCount;
  }
  //------------------------------------------------ GET FPS -------------------------------------------------
  int GameImpl::getFPS() const
  {
    return data->fps;
  }
  //------------------------------------------------ GET FPS -------------------------------------------------
  double GameImpl::getAverageFPS() const
  {
    return data->averageFPS;
  }
  //-------------------------------------------- GET MOUSE POSITION ------------------------------------------
  Position GameImpl::getMousePosition() const
  {
    return Position(data->mouseX,data->mouseY);
  }
  //---------------------------------------------- GET MOUSE STATE -------------------------------------------
  bool GameImpl::getMouseState(int button) const
  {
    if ( button < 0 || button >= M_MAX )
      return false;
    return data->mouseState[button];
  }
  //----------------------------------------------- GET KEY STATE --------------------------------------------
  bool GameImpl::getKeyState(int key) const
  {
    if ( key < 0 || key >= K_MAX )
      return false;
    return data->keyState[key];
  }
  //-------------------------------------------- GET SCREEN POSITION -----------------------------------------
  Position GameImpl::getScreenPosition() const
  {
    return Position(data->screenX,data->screenY);
  }
  //-------------------------------------------- SET SCREEN POSITION -----------------------------------------
  void GameImpl::setScreenPosition(int x, int y)
  {
    addCommand(BWAPIC::Command(BWAPIC::CommandType::SetScreenPosition,x,y));
  }
  //----------------------------------------------- PING MINIMAP ---------------------------------------------
  void GameImpl::pingMinimap(int x, int y)
  {
    addCommand(BWAPIC::Command(BWAPIC::CommandType::PingMinimap,x,y));
  }
  //----------------------------------------------- IS FLAG ENABLED ------------------------------------------
  bool GameImpl::isFlagEnabled(int flag) const
  {
    if ( flag < 0 || flag >= BWAPI::Flag::Max ) 
      return false;
    return data->flags[flag];
  }
  //----------------------------------------------- ENABLE FLAG ----------------------------------------------
  void GameImpl::enableFlag(int flag)
  {
    if ( flag < 0 || flag >= BWAPI::Flag::Max ) 
      return;
    if ( data->flags[flag] == false )
      addCommand(BWAPIC::Command(BWAPIC::CommandType::EnableFlag,flag));
  }
  //----------------------------------------------- GET UNITS IN RECTANGLE -----------------------------------
  Unitset GameImpl::getUnitsInRectangle(int left, int top, int right, int bottom, const UnitFilter &pred) const
  {
    Unitset unitFinderResults;

    // Have the unit finder do its stuff
    Templates::iterateUnitFinder<unitFinder>(data->xUnitSearch,
                                             data->yUnitSearch,
                                             left,
                                             top,
                                             right,
                                             bottom,
                                             [&](Unit *u){ if ( !pred.isValid() || pred(u) )
                                                             unitFinderResults.push_back(u); });
    // Return results
    return unitFinderResults;
  }
  Unit *GameImpl::getClosestUnitInRectangle(Position center, const UnitFilter &pred, int left, int top, int right, int bottom) const
  {
    int bestDistance = 99999999;
    Unit *pBestUnit = nullptr;

    Templates::iterateUnitFinder<unitFinder>(data->xUnitSearch,
                                             data->yUnitSearch,
                                             left,
                                             top,
                                             right,
                                             bottom,
                                             [&](Unit *u){ if ( !pred.isValid() || pred(u) )
                                                           {
                                                              int newDistance = u->getDistance(center);
                                                              if ( newDistance < bestDistance )
                                                              {
                                                                pBestUnit = u;
                                                                bestDistance = newDistance;
                                                              }
                                                           } } );
    return pBestUnit;
  }
  Unit *GameImpl::getBestUnit(const BestUnitFilter &best, const UnitFilter &pred, Position center, int radius) const
  {
    Unit *pBestUnit = nullptr;
    Position rad(radius,radius);
    
    Position topLeft(center - rad);
    Position botRight(center + rad);

    topLeft.makeValid();
    botRight.makeValid();

    Templates::iterateUnitFinder<unitFinder>(data->xUnitSearch,
                                             data->yUnitSearch,
                                             topLeft.x,
                                             topLeft.y,
                                             botRight.x,
                                             botRight.y,
                                             [&](Unit *u){ if ( !pred.isValid() || pred(u) )
                                                           {
                                                             if ( pBestUnit == nullptr )
                                                               pBestUnit = u;
                                                             else
                                                               pBestUnit = best(pBestUnit,u); 
                                                           } } );

    return pBestUnit;
  }
  //----------------------------------------------- MAP WIDTH ------------------------------------------------
  int GameImpl::mapWidth() const
  {
    return data->mapWidth;
  }
  //----------------------------------------------- MAP HEIGHT -----------------------------------------------
  int GameImpl::mapHeight() const
  {
    return data->mapHeight;
  }
  //---------------------------------------------- MAP FILE NAME ---------------------------------------------
  std::string GameImpl::mapFileName() const
  {
    return std::string(data->mapFileName);
  }
  //---------------------------------------------- MAP PATH NAME ---------------------------------------------
  std::string GameImpl::mapPathName() const
  {
    return std::string(data->mapPathName);
  }
  //------------------------------------------------ MAP NAME ------------------------------------------------
  std::string GameImpl::mapName() const
  {
    return std::string(data->mapName);
  }
  //----------------------------------------------- GET MAP HASH ---------------------------------------------
  std::string GameImpl::mapHash() const
  {
    return std::string(data->mapHash);
  }
  //--------------------------------------------- IS WALKABLE ------------------------------------------------
  bool GameImpl::isWalkable(int x, int y) const
  {
    if ( !WalkPosition(x, y) )
      return 0;
    return data->isWalkable[x][y];
  }
  //--------------------------------------------- GET GROUND HEIGHT ------------------------------------------
  int GameImpl::getGroundHeight(int x, int y) const
  {
    if ( !TilePosition(x, y) )
      return 0;
    return data->getGroundHeight[x][y];
  }
  //--------------------------------------------- IS BUILDABLE -----------------------------------------------
  bool GameImpl::isBuildable(int x, int y, bool includeBuildings) const
  {
    if ( !TilePosition(x, y) )
      return 0;
    return data->isBuildable[x][y] && ( includeBuildings ? !data->isOccupied[x][y] : true );
  }
  //--------------------------------------------- IS VISIBLE -------------------------------------------------
  bool GameImpl::isVisible(int x, int y) const
  {
    if ( !TilePosition(x, y) )
      return 0;
    return data->isVisible[x][y];
  }
  //--------------------------------------------- IS EXPLORED ------------------------------------------------
  bool GameImpl::isExplored(int x, int y) const
  {
    if ( !TilePosition(x, y) )
      return 0;
    return data->isExplored[x][y];
  }
  //--------------------------------------------- HAS CREEP --------------------------------------------------
  bool GameImpl::hasCreep(int x, int y) const
  {
    if ( !TilePosition(x, y) )
      return 0;
    return data->hasCreep[x][y];
  }
  //--------------------------------------------- HAS POWER --------------------------------------------------
  bool GameImpl::hasPowerPrecise(int x, int y, UnitType unitType) const
  {
    return Templates::hasPower<Unit>(x, y, unitType, pylons);
  }
  //------------------------------------------------ PRINTF --------------------------------------------------
  void GameImpl::printf(const char *format, ...)
  {
    char *buffer;
    vstretchyprintf(buffer, format);
    addCommand(BWAPIC::Command(BWAPIC::CommandType::Printf,addString(buffer)));
    free(buffer);
    return;
  }
  //--------------------------------------------- SEND TEXT --------------------------------------------------
  void GameImpl::sendText(const char *format, ...)
  {
    char *buffer;
    vstretchyprintf(buffer, format);
    addCommand(BWAPIC::Command(BWAPIC::CommandType::SendText,addString(buffer)));
    free(buffer);
    return;
  }
  //--------------------------------------------- SEND TEXT EX -----------------------------------------------
  void GameImpl::sendTextEx(bool toAllies, const char *format, ...)
  {
    return; //todo: implement
  }
  //---------------------------------------------- CHANGE RACE -----------------------------------------------
  void GameImpl::changeRace(Race race)
  {
    addCommand(BWAPIC::Command(BWAPIC::CommandType::ChangeRace));
  }
  //----------------------------------------------- IS IN GAME -----------------------------------------------
  bool GameImpl::isInGame() const
  {
    return data->isInGame;
  }
  //--------------------------------------------- IS MULTIPLAYER ---------------------------------------------
  bool GameImpl::isMultiplayer() const
  {
    return data->isMultiplayer;
  }  
  //--------------------------------------------- IS BATTLE NET ----------------------------------------------
  bool GameImpl::isBattleNet() const
  {
    return data->isBattleNet;
  }
  //----------------------------------------------- IS PAUSED ------------------------------------------------
  bool GameImpl::isPaused() const
  {
    return data->isPaused;
  }
  //----------------------------------------------- IS REPLAY ------------------------------------------------
  bool GameImpl::isReplay() const
  {
    return data->isReplay;
  }
  //----------------------------------------------- START GAME -----------------------------------------------
  void GameImpl::startGame()
  {
    addCommand(BWAPIC::Command(BWAPIC::CommandType::StartGame));
  }
  //----------------------------------------------- PAUSE GAME -----------------------------------------------
  void GameImpl::pauseGame()
  {
    addCommand(BWAPIC::Command(BWAPIC::CommandType::PauseGame));
  }
  //---------------------------------------------- RESUME GAME -----------------------------------------------
  void GameImpl::resumeGame()
  {
    addCommand(BWAPIC::Command(BWAPIC::CommandType::ResumeGame));
  }
  //---------------------------------------------- LEAVE GAME ------------------------------------------------
  void GameImpl::leaveGame()
  {
    addCommand(BWAPIC::Command(BWAPIC::CommandType::LeaveGame));
  }
  //--------------------------------------------- RESTART GAME -----------------------------------------------
  void GameImpl::restartGame()
  {
    addCommand(BWAPIC::Command(BWAPIC::CommandType::RestartGame));
  }
  //--------------------------------------------- SET ALLIANCE -----------------------------------------------
  bool GameImpl::setAlliance(BWAPI::Player *player, bool allied, bool alliedVictory)
  {
    /* Set the current player's alliance status */
    if ( !self() || isReplay() || !player || player == self() )
    {
      lastError = Errors::Invalid_Parameter;
      return false;
    }

    addCommand(BWAPIC::Command(BWAPIC::CommandType::SetAllies, player->getID(), allied ? (alliedVictory ? 2 : 1) : 0));
    lastError = Errors::None;
    return true;
  }
  //----------------------------------------------- SET VISION -----------------------------------------------
  bool GameImpl::setVision(BWAPI::Player *player, bool enabled)
  {
    /* Set the current player's vision status */
    if ( !self() || isReplay() || !player || player == self() )
    {
      lastError = Errors::Invalid_Parameter;
      return false;
    }

    addCommand(BWAPIC::Command(BWAPIC::CommandType::SetVision, player->getID(), enabled ? 1 : 0));
    lastError = Errors::None;
    return true;
  }
  //---------------------------------------------- SET GAME SPEED --------------------------------------------
  void GameImpl::setLocalSpeed(int speed)
  {
    addCommand(BWAPIC::Command(BWAPIC::CommandType::SetLocalSpeed, speed));
  }
  void GameImpl::setFrameSkip(int frameSkip)
  {
    lastError = Errors::None;
    if ( frameSkip > 0 )
    {
      addCommand(BWAPIC::Command(BWAPIC::CommandType::SetFrameSkip, frameSkip));
      return;
    }
    lastError = Errors::Invalid_Parameter;
  }
  //------------------------------------------- ISSUE COMMAND ------------------------------------------------
  bool GameImpl::issueCommand(const Unitset& units, UnitCommand command)
  {
    bool success = false;
    //FIX FIX FIX naive implementation
    foreach(Unit* u, units)
    {
      success |= u->issueCommand(command);
    }
    return success;
  }
  //------------------------------------------ GET SELECTED UNITS --------------------------------------------
  const Unitset& GameImpl::getSelectedUnits()
  {
    lastError = Errors::None;
    return selectedUnits;
  }
  //--------------------------------------------- SELF -------------------------------------------------------
  Player* GameImpl::self()
  {
    lastError = Errors::None;
    return thePlayer;
  }
  //--------------------------------------------- ENEMY ------------------------------------------------------
  Player* GameImpl::enemy()
  {
    lastError = Errors::None;
    return theEnemy;
  }
  //--------------------------------------------- ENEMY ------------------------------------------------------
  Player* GameImpl::neutral()
  {
    lastError = Errors::None;
    return theNeutral;
  }
  //--------------------------------------------- ALLIES -----------------------------------------------------
  Playerset& GameImpl::allies()
  {
    /* Returns a set of all the ally players that have not left or been defeated. Does not include self. */
    lastError = Errors::None;
    return _allies;
  }
  //--------------------------------------------- ENEMIES ----------------------------------------------------
  Playerset& GameImpl::enemies()
  {
    /* Returns a set of all the enemy players that have not left or been defeated. */
    lastError = Errors::None;
    return _enemies;
  }
  //-------------------------------------------- OBSERVERS ---------------------------------------------------
  Playerset& GameImpl::observers()
  {
    /* Returns a set of all the enemy players that have not left or been defeated. */
    lastError = Errors::None;
    return _observers;
  }

  //---------------------------------------------- SET TEXT SIZE ---------------------------------------------
  void GameImpl::setTextSize(int size)
  {
    if ( size < 0 )
      size = 0;
    if ( size > 3 )
      size = 3;
    textSize = size;
  }
  //-------------------------------------------------- DRAW TEXT ---------------------------------------------
  void GameImpl::drawText(int ctype, int x, int y, const char *format, ...)
  {
    if ( !data->hasGUI ) return;
    char *buffer;
    vstretchyprintf(buffer, format);
    BWAPIC::Shape s(BWAPIC::ShapeType::Text,ctype,x,y,0,0,0,textSize,0,false);
    addText(s,buffer);
    free(buffer);
  }
  void GameImpl::drawTextMap(int x, int y, const char *format, ...)
  {
    if ( !data->hasGUI ) return;
    char *buffer;
    vstretchyprintf(buffer, format);
    BWAPIC::Shape s(BWAPIC::ShapeType::Text,(int)BWAPI::CoordinateType::Map,x,y,0,0,0,textSize,0,false);
    addText(s,buffer);
    free(buffer);
  }
  void GameImpl::drawTextMouse(int x, int y, const char *format, ...)
  {
    if ( !data->hasGUI ) return;
    char *buffer;
    vstretchyprintf(buffer, format);
    BWAPIC::Shape s(BWAPIC::ShapeType::Text,(int)BWAPI::CoordinateType::Mouse,x,y,0,0,0,textSize,0,false);
    addText(s,buffer);
    free(buffer);
  }
  void GameImpl::drawTextScreen(int x, int y, const char *format, ...)
  {
    if ( !data->hasGUI ) return;
    char *buffer;
    vstretchyprintf(buffer, format);
    BWAPIC::Shape s(BWAPIC::ShapeType::Text,(int)BWAPI::CoordinateType::Screen,x,y,0,0,0,textSize,0,false);
    addText(s,buffer);
    free(buffer);
  }
  //--------------------------------------------------- DRAW BOX ---------------------------------------------
  void GameImpl::drawBox(int ctype, int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    if ( !data->hasGUI ) return;
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Box,ctype,left,top,right,bottom,0,0,color,isSolid));
  }
  //------------------------------------------------ DRAW TRIANGLE -------------------------------------------
  void GameImpl::drawTriangle(int ctype, int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    if ( !data->hasGUI ) return;
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Triangle,ctype,ax,ay,bx,by,cx,cy,color,isSolid));
  }
  //------------------------------------------------- DRAW CIRCLE --------------------------------------------
  void GameImpl::drawCircle(int ctype, int x, int y, int radius, Color color, bool isSolid)
  {
    if ( !data->hasGUI ) return;
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Circle,ctype,x,y,0,0,radius,0,color,isSolid));
  }
  //------------------------------------------------- DRAW ELIPSE --------------------------------------------
  void GameImpl::drawEllipse(int ctype, int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    if ( !data->hasGUI ) return;
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Ellipse,ctype,x,y,0,0,xrad,yrad,color,isSolid));
  }

  void GameImpl::drawDot(int ctype, int x, int y, Color color)
  {
    if ( !data->hasGUI ) return;
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Dot,ctype,x,y,0,0,0,0,color,false));
  }
  //-------------------------------------------------- DRAW LINE ---------------------------------------------
  void GameImpl::drawLine(int ctype, int x1, int y1, int x2, int y2, Color color)
  {
    if ( !data->hasGUI ) return;
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Line,ctype,x1,y1,x2,y2,0,0,color,false));
  }
  void* GameImpl::getScreenBuffer()
  {
    return nullptr;
  }
  int GameImpl::getLatencyFrames()
  {
    return data->latencyFrames;
  }
  int GameImpl::getLatencyTime()
  {
    return data->latencyTime;
  }
  int GameImpl::getRemainingLatencyFrames()
  {
    return data->remainingLatencyFrames;
  }
  int GameImpl::getRemainingLatencyTime()
  {
    return data->remainingLatencyTime;
  }
  int GameImpl::getRevision()
  {
    return data->revision;
  }
  bool GameImpl::isDebug()
  {
    return data->isDebug;
  }
  bool GameImpl::isLatComEnabled()
  {
    return data->hasLatCom;
  }
  void GameImpl::setLatCom(bool isEnabled)
  {
    int e=0;
    if (isEnabled) e=1;
    //update shared memory
    data->hasLatCom = isEnabled;
    //queue up command for server so it also applies the change
    addCommand(BWAPIC::Command(BWAPIC::CommandType::SetLatCom, e));
  }
  bool GameImpl::isGUIEnabled()
  {
    return data->hasGUI;
  }
  void GameImpl::setGUI(bool enabled)
  {
    int e=0;
    if (enabled) e=1;
    data->hasGUI = enabled;
    //queue up command for server so it also applies the change
    addCommand(BWAPIC::Command(BWAPIC::CommandType::SetGui, e));
  }
  int GameImpl::getInstanceNumber()
  {
    return data->instanceID;
  }
  int GameImpl::getAPM(bool includeSelects)
  {
    if ( includeSelects )
      return data->botAPM_selects;
    return data->botAPM_noselects;
  }
  bool GameImpl::setMap(const char *mapFileName)
  {
    if ( !mapFileName || strlen(mapFileName) >= MAX_PATH || !mapFileName[0] )
      return setLastError(Errors::Invalid_Parameter);

    if ( GetFileAttributes(mapFileName) == INVALID_FILE_ATTRIBUTES )
      return setLastError(Errors::File_Not_Found);

    addCommand( BWAPIC::Command(BWAPIC::CommandType::SetMap, addString(mapFileName)) );
    return setLastError();
  }
  bool GameImpl::hasPath(Position source, Position destination) const
  {
    Broodwar->setLastError();
    if ( !source.isValid() || !destination.isValid() )
      return Broodwar->setLastError(Errors::Unreachable_Location);

    if ( data )
    {
      unsigned short srcIdx = data->mapTileRegionId[source.x/32][source.y/32];
      unsigned short dstIdx = data->mapTileRegionId[destination.x/32][destination.y/32];

      unsigned short srcGroup = 0;
      unsigned short dstGroup = 0;
      if ( srcIdx & 0x2000 )
      {
        int minitilePosX = (source.x&0x1F)/8;
        int minitilePosY = (source.y&0x1F)/8;
        int minitileShift = minitilePosX + minitilePosY * 4;
        unsigned short miniTileMask = data->mapSplitTilesMiniTileMask[srcIdx&0x1FFF];
        unsigned short rgn1         = data->mapSplitTilesRegion1[srcIdx&0x1FFF];
        unsigned short rgn2         = data->mapSplitTilesRegion2[srcIdx&0x1FFF];
        if ( (miniTileMask >> minitileShift) & 1 )
          srcGroup = data->regions[rgn2].islandID;
        else
          srcGroup = data->regions[rgn1].islandID;
      }
      else
      {
        srcGroup = data->regions[srcIdx].islandID;
      }

      if ( dstIdx & 0x2000 )
      {
        int minitilePosX = (destination.x&0x1F)/8;
        int minitilePosY = (destination.y&0x1F)/8;
        int minitileShift = minitilePosX + minitilePosY * 4;

        unsigned short miniTileMask = data->mapSplitTilesMiniTileMask[dstIdx&0x1FFF];
        unsigned short rgn1         = data->mapSplitTilesRegion1[dstIdx&0x1FFF];
        unsigned short rgn2         = data->mapSplitTilesRegion2[dstIdx&0x1FFF];

        if ( (miniTileMask >> minitileShift) & 1 )
          dstGroup = data->regions[rgn2].islandID;
        else
          dstGroup = data->regions[rgn1].islandID;
      }
      else
      {
        dstGroup = data->regions[dstIdx].islandID;
      }

      if ( srcGroup == dstGroup )
        return true;
    }
    return Broodwar->setLastError(Errors::Unreachable_Location);
  }
  int GameImpl::elapsedTime() const
  {
    return data->elapsedTime;
  }
  void GameImpl::setCommandOptimizationLevel(int level)
  {
    //queue up command for server so it also applies the change
    addCommand(BWAPIC::Command(BWAPIC::CommandType::SetCommandOptimizerLevel, level));
  }
  int GameImpl::countdownTimer() const
  {
    return data->countdownTimer;
  }
  //------------------------------------------------- GET REGION AT ------------------------------------------
  BWAPI::Region *GameImpl::getRegionAt(int x, int y) const
  {
    if ( !Position(x,y) )
    {
      Broodwar->setLastError(BWAPI::Errors::Invalid_Parameter);
      return nullptr;
    }
    unsigned short idx = data->mapTileRegionId[x/32][y/32];
    if ( idx & 0x2000 )
    {
      int minitilePosX = (x&0x1F)/8;
      int minitilePosY = (y&0x1F)/8;
      int minitileShift = minitilePosX + minitilePosY * 4;
      unsigned short miniTileMask = data->mapSplitTilesMiniTileMask[idx&0x1FFF];
      unsigned short rgn1         = data->mapSplitTilesRegion1[idx&0x1FFF];
      unsigned short rgn2         = data->mapSplitTilesRegion2[idx&0x1FFF];
      if ( (miniTileMask >> minitileShift) & 1 )
        return Broodwar->getRegion(rgn2);
      else
        return Broodwar->getRegion(rgn1);
    }
    return Broodwar->getRegion(idx);
  }
  int GameImpl::getLastEventTime() const
  {
    return 0;
  }
  bool GameImpl::setReplayVision(Player *player, bool enabled)
  {
    if ( !player || !isReplay() )
    {
      lastError = Errors::Invalid_Parameter;
      return false;
    }
    addCommand(BWAPIC::Command(BWAPIC::CommandType::SetReplayVision, player->getID(), enabled ? 1 : 0));
    lastError = Errors::None;
    return true;
  }
  bool GameImpl::setRevealAll(bool reveal)
  {
    if ( !isReplay() )
    {
      lastError = Errors::Invalid_Parameter;
      return false;
    }
    addCommand(BWAPIC::Command(BWAPIC::CommandType::SetRevealAll, reveal ? 1 : 0));
    lastError = Errors::None;
    return true;
  }
};

