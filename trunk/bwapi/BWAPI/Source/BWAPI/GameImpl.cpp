#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used stuff from Windows headers

#include "GameImpl.h"

#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <fstream>


#include <Util/FileLogger.h>
#include <Util/Dictionary.h>
#include <Util/Sentence.h>
#include <Util/Exceptions.h>
#include <Util/Strings.h>

#include <BWAPI/ForceImpl.h>
#include <BWAPI/PlayerImpl.h>
#include <BWAPI/UnitImpl.h>
#include <BWAPI/Command.h>
#include <BWAPI/CommandCancelTrain.h>
#include <BWAPI/Map.h>
#include <BWAPI/ScreenLogger.h>
#include <BWAPI/Flag.h>
#include <BWAPI.h>


#include <BW/Unit.h>
#include <BW/Offsets.h>
#include <BW/UnitTarget.h>
#include <BW/OrderTypes.h>
#include <BW/Latency.h>
#include <BW/TileType.h>
#include <BW/TileSet.h>
#include <BW/UnitType.h>
#include <BW/GameType.h>
#include <BW/WeaponType.h>

#include "BWAPI/AIModule.h"
#include "DLLMain.h"

#include "Globals.h"
#include "ShapeBox.h"
#include "ShapeCircle.h"
#include "ShapeEllipse.h"
#include "ShapeDot.h"
#include "ShapeLine.h"
#include "ShapeTriangle.h"
#include "ShapeText.h"

namespace BWAPI
{
  Game* Broodwar;
  GameImpl BroodwarImpl;

  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  GameImpl::GameImpl()
      : onStartCalled(false)
      , unitsOnTileData(0, 0)
      , quietSelect(true)
      , enabled(true)
      , client(NULL)
      , startedClient(false)
      , hcachedShapesMutex(::CreateMutex(NULL, FALSE, _T("cachedShapesVector")))
  {
    BWAPI::Broodwar = static_cast<Game*>(this);
    BW::UnitType::initialize();

    try
    {
      this->configuration = new Util::Dictionary("bwapi-data\\bwapi.ini");
      config = this->configuration;
    }
    catch (GeneralException& exception)
    {
      FILE*f = fopen("bwapi-error", "wt");
      fprintf_s(f, "Couldn't load configuration file bwapi.ini because: %s", exception.getMessage().c_str());
      fclose(f);
    }
    try
    {
      this->commandLog        = new Util::FileLogger(config->get("log_path") + "\\commands", Util::LogLevel::MicroDetailed);
      this->newOrderLog       = new Util::FileLogger(config->get("log_path") + "\\new_orders", Util::LogLevel::MicroDetailed);
      this->badAssumptionLog  = new Util::FileLogger(config->get("log_path") + "\\bad_assumptions", Util::LogLevel::MicroDetailed);
      this->newUnitLog        = new Util::FileLogger(config->get("log_path") + "\\new_unit_id", Util::LogLevel::MicroDetailed);
      this->unitSum           = new Util::FileLogger(config->get("log_path") + "\\unit_sum", Util::LogLevel::MicroDetailed);
      this->fatalError        = new Util::FileLogger(config->get("log_path") + "\\FATAL-ERROR", Util::LogLevel::MicroDetailed);

      unitArrayCopy = new BW::UnitArray;
      unitArrayCopyLocal = new BW::UnitArray;

      for (int i = 0; i < 12; i++)
        players[i] = new PlayerImpl((u8)i);

      for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
        unitArray[i] = new UnitImpl(&unitArrayCopy->unit[i],
                                    &BW::BWDATA_UnitNodeTable->unit[i],
                                    &unitArrayCopyLocal->unit[i],
                                    i);

      for (int i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        unitTypes.insert(BW::UnitType((BW::UnitID::Enum)i));
    }
    catch (GeneralException& exception)
    {
      FILE*f = fopen("bwapi-error", "wt");
      fprintf_s(f, "Exception caught inside Game constructor: %s", exception.getMessage().c_str());
      fclose(f);
    }
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  GameImpl::~GameImpl()
  {
    delete unitArrayCopy;
    delete unitArrayCopyLocal;

    for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
      delete unitArray[i];

    for (int i = 0; i < 12; i++)
      delete players[i];

    delete this->commandLog;
    delete this->newOrderLog;
    delete this->badAssumptionLog;
    delete this->newUnitLog;
    delete this->unitSum;
    delete this->fatalError;
    delete this->configuration;
  }
  //----------------------------------------------- MAP WIDTH ------------------------------------------------
  int GameImpl::mapWidth() const
  {
    return Map::getWidth();
  }
  //----------------------------------------------- MAP HEIGHT -----------------------------------------------
  int GameImpl::mapHeight() const
  {
    return Map::getHeight();
  }
  //---------------------------------------------- MAP FILENAME ----------------------------------------------
  std::string GameImpl::mapFilename() const
  {
    return Map::getFileName();
  }
  //------------------------------------------------ MAP NAME ------------------------------------------------
  std::string GameImpl::mapName() const
  {
    return Map::getName();
  }
  //------------------------------------------------ BUILDABLE -----------------------------------------------
  bool GameImpl::buildable(int x, int y) const
  {
    return this->map.buildable(x, y);
  }
  //------------------------------------------------ WALKABLE ------------------------------------------------
  bool GameImpl::walkable(int x, int y) const
  {
    return this->map.walkable(x, y);
  }
  //------------------------------------------------- VISIBLE ------------------------------------------------
  bool GameImpl::visible(int x, int y) const
  {
    return this->map.visible(x, y);
  }
  //------------------------------------------------ HAS CREEP -----------------------------------------------
  bool GameImpl::hasCreep(int x, int y) const
  {
    if (this->isFlagEnabled(Flag::CompleteMapInformation) || this->visible(x, y))
      return this->map.hasCreep(x, y);
    return false;
  }
  //------------------------------------------------ HAS POWER -----------------------------------------------
  bool GameImpl::hasPower(int x, int y, int tileWidth, int tileHeight) const
  {
    if (!(tileWidth==2 && tileHeight==2) && !(tileWidth==3 && tileHeight==2) && !(tileWidth==4 && tileHeight==3))
    {
      return false;
    }
    if (tileWidth==4)
    {
      x++;
    }
    for(std::list<UnitImpl*>::const_iterator i=this->myPylons.begin();i!=this->myPylons.end();i++)
    {
      int px=(*i)->getTilePosition().x();
      int py=(*i)->getTilePosition().y();
      int bx=x-px+4;
      int by=y-py+7;
      if (bx>=0 && by>=0 && bx<=14 && by<=8)
      {
        switch(by)
        {
          case 0:
            if (bx>=1 && bx<=12) return true;
          break;
          case 1:
            if (bx<=13) return true;
          break;
          case 2:
          case 3:
          case 4:
          case 5:
            return true;
          break;
          case 6:
            if (bx<=13) return true;
          case 7:
            if (bx>=1 && bx<=12) return true;
          case 8:
            if (bx>=4 && bx<=9) return true;
          break;
        }
      }
    }
    return false;
  }
  //---------------------------------------------- GROUND HEIGHT ---------------------------------------------
  int GameImpl::groundHeight(int x, int y) const
  {
    return this->map.groundHeight(x, y);
  }
  //--------------------------------------------- GET START LOCATIONS ----------------------------------------
  const std::set< TilePosition >& GameImpl::getStartLocations() const
  {
    return this->startLocations;
  }
  //----------------------------------------------- GET MAP HASH ---------------------------------------------
  int GameImpl::getMapHash() const
  {
    return BWAPI::Map::getMapHash();
  }
  //----------------------------------------------- GET FORCES -----------------------------------------------
  std::set< Force* > GameImpl::getForces() const
  {
    return this->forces;
  }
  //----------------------------------------------- GET PLAYERS ----------------------------------------------
  std::set< Player* > GameImpl::getPlayers() const
  {
    std::set<Player*> players;
    for (int i = 0; i < 12; i++)
    {
      if (this->players[i] != NULL && this->players[i]->getName().length() > 0)
      {
        players.insert(this->players[i]);
      }
    }
    return players;
  }
  //------------------------------------------------- GET UNITS ----------------------------------------------
  std::set< Unit* > GameImpl::getAllUnits() const
  {
    std::set<Unit*> units;
    if (this->isFlagEnabled(Flag::CompleteMapInformation))
    {
      for (std::set<UnitImpl*>::const_iterator i = this->units.begin(); i != this->units.end(); i++)
      {
        units.insert((Unit*)(*i));
      }
    }
    else
    {
      for (std::set<UnitImpl*>::const_iterator i = this->units.begin(); i != this->units.end(); i++)
      {
        if ((*i)->isVisible())
        {
          units.insert((Unit*)(*i));
        }
      }
    }
    return units;
  }
  //---------------------------------------------- GET MINERALS ----------------------------------------------
  std::set< Unit* > GameImpl::getMinerals() const
  {
    std::set<Unit*> units;
    if (this->isFlagEnabled(Flag::CompleteMapInformation))
    {
      for (std::set<UnitImpl*>::const_iterator i = this->units.begin(); i != this->units.end(); i++)
      {
        if ((*i)->isMineral())
          units.insert((Unit*)(*i));
      }
    }
    else
    {
      for (std::set<UnitImpl*>::const_iterator i = this->units.begin(); i != this->units.end(); i++)
      {
        if ((*i)->isVisible() && (*i)->isMineral())
          units.insert((Unit*)(*i));
      }
    }
    return units;
  }
  //---------------------------------------------- GET GEYSERS -----------------------------------------------
  std::set< Unit* > GameImpl::getGeysers() const
  {
    std::set<Unit*> units;
    if (this->isFlagEnabled(Flag::CompleteMapInformation))
    {
      for (std::set<UnitImpl*>::const_iterator i = this->units.begin(); i != this->units.end(); i++)
      {
        if ((*i)->getType() == BW::UnitID::Resource_VespeneGeyser)
          units.insert((Unit*)(*i));
      }
    }
    else
    {
      for (std::set<UnitImpl*>::const_iterator i = this->units.begin(); i != this->units.end(); i++)
      {
        if ((*i)->isVisible() && (*i)->getType() == BW::UnitID::Resource_VespeneGeyser)
          units.insert((Unit*)(*i));
      }
    }
    return units;
  }
  //------------------------------------------- GET NEUTRAL UNITS --------------------------------------------
  std::set< Unit* > GameImpl::getNeutralUnits() const
  {
    std::set<Unit*> units;
    if (this->isFlagEnabled(Flag::CompleteMapInformation))
    {
      for (std::set<UnitImpl*>::const_iterator i = this->units.begin(); i != this->units.end(); i++)
      {
        if (((PlayerImpl*)(*i)->getPlayer())->getID() == 11)
          units.insert((Unit*)(*i));
      }
    }
    else
    {
      for (std::set<UnitImpl*>::const_iterator i = this->units.begin(); i != this->units.end(); i++)
      {
        if ((*i)->isVisible() && ((PlayerImpl*)(*i)->getPlayer())->getID() == 11)
          units.insert((Unit*)(*i));
      }
    }
    return units;
  }
  //--------------------------------------------- ISSUE COMMAND ----------------------------------------------
  void GameImpl::IssueCommand(PBYTE pbBuffer, u32 iSize)
  {
    __asm
    {
      MOV ECX, pbBuffer
      MOV EDX, iSize
    }
    NewIssueCommand();
  }
  //------------------------------------------------- UPDATE -------------------------------------------------
  void GameImpl::update()
  {
    try
    {
      if (!this->isOnStartCalled())
        this->onGameStart();
      if (!this->enabled)
        return;
      memcpy(this->unitArrayCopy, BW::BWDATA_UnitNodeTable, sizeof(BW::UnitArray));
      memcpy(this->unitArrayCopyLocal, BW::BWDATA_UnitNodeTable, sizeof(BW::UnitArray));

      for (int i = 0; i < BW::PLAYER_COUNT; i++)
        this->players[i]->update();

      this->units.clear();
      std::list<UnitImpl*> unitList;
      for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
      {
        this->getUnit(i)->buildUnit = NULL;
      }
      for (UnitImpl* i = this->getFirst(); i != NULL; i = i->getNext())
      {
        unitList.push_back(i);
        if (i->getOrderTarget() != NULL && i->getBWOrder() == BW::OrderID::ConstructingBuilding)
        {
          UnitImpl* j = (UnitImpl*)(i->getOrderTarget());
          i->buildUnit = j;
          j->buildUnit = i;
        }
      }
      this->myPylons.clear();
      for (std::list<UnitImpl*>::iterator i = unitList.begin(); i != unitList.end(); i++)
      {
        if (this->units.find(*i) == this->units.end())
        {
          this->units.insert(*i);
          if ((*i)->getPlayer()==(Player*)this->BWAPIPlayer && (*i)->getBWType().getID()==BW::UnitID::Protoss_Pylon && (*i)->isCompleted())
          {
            this->myPylons.push_back(*i);
          }
          std::list<BWAPI::Unit*> loadedUnits = (*i)->getLoadedUnits();
          for (std::list<BWAPI::Unit*>::iterator j = loadedUnits.begin(); j != loadedUnits.end(); j++)
          {
            this->units.insert((UnitImpl*)(*j));
          }
        }
      }
      refreshSelectionStates();

      while ((int)(this->commandBuffer.size()) > this->getLatency())
      {
        for (unsigned int i = 0; i < this->commandBuffer[0].size(); i++)
        {
          delete this->commandBuffer[0][i];
        }
        this->commandBuffer.erase(this->commandBuffer.begin());
      }
      this->commandBuffer.push_back(std::vector<Command *>());
      for (unsigned int i = 0; i < this->commandBuffer.size(); i++)
        for (unsigned int j = 0; j < this->commandBuffer[i].size(); j++)
          this->commandBuffer[i][j]->execute();
      this->frameCount ++;
      this->logUnknownOrStrange();
      this->updateUnitsOnTile();
    }
    catch (GeneralException& exception)
    {
      FILE*f = fopen("bwapi-error", "wt");
      fprintf_s(f, "Exception caught inside Game::update: %s", exception.getMessage().c_str());
      fclose(f);
    }
    if (this->startedClient == false)
    {

      TCHAR szDllPath[MAX_PATH];
      std::string ai_dll = config->get("ai_dll");
      for (unsigned int i = 0; i < ai_dll.length(); i++)
        szDllPath[i] = TCHAR(ai_dll[i]);

      szDllPath[ai_dll.length()] = TCHAR('\0');
      Util::Logger::globalLog->logCritical("Loading AI DLL from: %s", ai_dll.c_str());
      bool loaded;
      if (!(hMod = LoadLibrary(szDllPath)))
      {
        loaded = false;
        Util::Logger::globalLog->logCritical("ERROR: Failed to load the AI Module");
        this->client = new AIModule();
      }
      else
      {
        loaded = true;
        Util::Logger::globalLog->logCritical("Loaded AI Module");
        Util::Logger::globalLog->logCritical("Importing by Virtual Function Table from AI DLL");

        typedef AIModule* (*PFNCreateA1)(BWAPI::Game*);

        Util::Logger::globalLog->logCritical("Creating an Object of AIModule");

        PFNCreateA1 newAIModule = (PFNCreateA1)GetProcAddress(hMod, TEXT("newAIModule"));
        this->client = newAIModule(this);
        Util::Logger::globalLog->logCritical("Created an Object of AIModule");
      }
      this->client->onStart();
      this->client->onFrame();
      this->lockFlags();
      if (loaded)
      {
        sendText("BWAPI: Loaded the AI Module: %s", ai_dll.c_str());
      }
      else
      {
        sendText("Error: Failed to load the AI Module");
      }
      this->startedClient = true;
    }

    this->client->onFrame();
    this->loadSelected();
    if (WAIT_OBJECT_0 == ::WaitForSingleObject(hcachedShapesMutex, INFINITE))
    {
      for (unsigned int i = 0; i < this->cachedShapes.size(); i++)
      {
        delete this->cachedShapes[i];
      }
      this->cachedShapes = this->shapes;
      ::ReleaseMutex(hcachedShapesMutex);
    }
    this->shapes.clear();
  }
  //---------------------------------------- REFRESH SELECTION STATES ----------------------------------------
  void GameImpl::refreshSelectionStates()
  {
    for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
      this->unitArray[i]->setSelected(false);
    this->saveSelected();
    for (int i = 0; savedSelectionStates[i] != NULL; i++)
      BWAPI::UnitImpl::BWUnitToBWAPIUnit(savedSelectionStates[i])->setSelected(true);
  }
  //-------------------------------------------- IS SINGLE PLAYER --------------------------------------------
  bool GameImpl::isSinglePlayer() const
  {
    return (*BW::BWDATA_IsMultiplayer==0);
  }
  //------------------------------------------- IS ON START CALLED -------------------------------------------
  bool GameImpl::isOnStartCalled() const
  {
    return this->onStartCalled;
  }
  //------------------------------------------ SET ON START CALLED -------------------------------------------
  void GameImpl::setOnStartCalled(bool onStartCalled)
  {
    this->onStartCalled = onStartCalled;
  }
#pragma warning(push)
#pragma warning(disable:4312)
  //------------------------------------------------ IN GAME -------------------------------------------------
  bool GameImpl::inGame() const
  {
    return *(BW::BWDATA_InGame) != 0;
  }
  //----------------------------------------------- IN REPLAY ------------------------------------------------
  bool GameImpl::inReplay() const
  {
    return *(BW::BWDATA_InReplay) != 0;
  }
  const int BUFFER_SIZE = 2048;
  char buffer[BUFFER_SIZE];

  //---------------------------------------------- PRINT WITH PLAYER ID --------------------------------------
  void GameImpl::printEx(s32 pID, const char* text, ...)
  {
    va_list ap;
    va_start(ap, text);
    vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
    va_end(ap);

    char* txtout = buffer;
    if (inGame() || inReplay())
      __asm
    {
      pushad
      push 0       // Unknown
      mov eax, pID   // Player ID (-1 for notification area)
      push txtout  // Text
      call dword ptr [BW::BWFXN_PrintText]
      popad
    }
    else
      printf(txtout); // until lobby print private text is found
  }
  //------------------------------------------------- PRINTF -------------------------------------------------
  void GameImpl::printf(const char* text, ...)
  {
    if (inReplay() || inGame())
    {
      va_list ap;
      va_start(ap, text);
      vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
      va_end(ap);

      printEx(8, buffer);
      return;
    }

    va_list ap;
    va_start(ap, text);
    vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
    va_end(ap);

    char* txtout = buffer;
    if (!inGame())
    {
        __asm
      {
        pushad
        mov edi, txtout
        call [BW::BWFXN_SendLobbyCallTarget]
        popad
      }
    }
  }

  void GameImpl::sendText(const char* text, ...)
  {
    if (inReplay() || (inGame() && isSinglePlayer()))
    {
      va_list ap;
      va_start(ap, text);
      vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
      va_end(ap);
      if (inReplay())
      {
        printEx(8, buffer);
      }
      else
      {
        printEx(this->BWAPIPlayer->getID(), buffer);
      }
      return;
    }

    va_list ap;
    va_start(ap, text);
    vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
    va_end(ap);

    char* txtout = buffer;
    if (inGame())
    {
      memset(BW::BWDATA_SendTextRequired, 0xFF, 2);
      __asm
      {
        pushad
        mov esi, txtout
        call [BW::BWFXN_SendPublicCallTarget]
        popad
      }
    }
    else
      __asm
    {
      pushad
      mov edi, txtout
      call [BW::BWFXN_SendLobbyCallTarget]
      popad
    }
  }
#pragma warning(pop)
  //---------------------------------------------- CHANGE SLOT -----------------------------------------------
  void GameImpl::changeSlot(BW::Orders::ChangeSlot::Slot slot, u8 slotID)
  {
    IssueCommand((PBYTE)&BW::Orders::ChangeSlot(slot, slotID), 3);
  }
  //---------------------------------------------- CHANGE RACE -----------------------------------------------
  void GameImpl::changeRace(BWAPI::Race race)
  {
    IssueCommand((PBYTE)&BW::Orders::ChangeRace(static_cast<BW::Race::Enum>(race.getID()), this->BWAPIPlayer->getID()), 3);
  }
  //----------------------------------------- ADD TO COMMAND BUFFER ------------------------------------------
  void GameImpl::addToCommandBuffer(Command* command)
  {
    this->reselected = true;
    command->execute();
    this->commandBuffer[this->commandBuffer.size() - 1].push_back(command);
    this->commandLog->log("(%4d) %s", this->frameCount, command->describe().c_str());
  }
  //--------------------------------------------- ON GAME START ----------------------------------------------
  void GameImpl::onGameStart()
  {
    this->frameCount = 0;
    this->setOnStartCalled(true);
    this->BWAPIPlayer = NULL;
    this->opponent = NULL;

    /* set all the flags to the default of disabled */
    for (int i = 0; i < FLAG_COUNT; i++)
      this->flags[i] = false;
    this->flagsLocked = false;

    map.load();

    if (*(BW::BWDATA_InReplay))
    {
      for (int i = 0; i < FLAG_COUNT; i++)
        this->flags[i] = true;
      this->flagsLocked = false;
    }
    else
    {
      for (int i = 0; i < BW::PLAYABLE_PLAYER_COUNT; i++)
        if (strcmp(BW::BWDATA_CurrentPlayer, this->players[i]->getName().c_str()) == 0)
          this->BWAPIPlayer = this->players[i];

      if (this->BWAPIPlayer == NULL ||
          this->BWAPIPlayer->getForceName() == "Observers" ||
          this->BWAPIPlayer->getForceName() == "Observer")
      {
        this->BWAPIPlayer = NULL;
        return;
      }

      for (int i = 0; i < BW::PLAYABLE_PLAYER_COUNT; i++)
        if ((this->players[i]->playerType() == BW::PlayerType::Computer ||
             this->players[i]->playerType() == BW::PlayerType::Human ||
             this->players[i]->playerType() == BW::PlayerType::ComputerSlot) &&
            this->opponent == NULL &&
            this->players[i]->getForceName() != "Observers" &&
            this->players[i]->getForceName() != "Observer" &&
            this->BWAPIPlayer->getAlliance(i) == 0)
          this->opponent = this->players[i];
    }
    BW::Positions* posptr = BW::startPositions;
    startLocations.clear();
    while (posptr->x != 0 || posptr->y != 0)
    {
      startLocations.insert(BWAPI::TilePosition((int)((posptr->x - BW::TILE_SIZE*2) / BW::TILE_SIZE),
                                                (int)((posptr->y - (int)(BW::TILE_SIZE*1.5)) / BW::TILE_SIZE)));
      posptr++;
    }
    std::set<std::string> force_names;
    std::map<std::string, ForceImpl*> force_name_to_forceimpl;
    for (int i = 0; i < 12; i++)
    {
      if (this->players[i] != NULL && this->players[i]->getName().length() > 0)
      {
        force_names.insert(std::string(this->players[i]->getForceName()));
      }
    }
    for (std::set<std::string>::iterator i = force_names.begin(); i != force_names.end(); i++)
    {
      ForceImpl* newforce = new ForceImpl(*i);
      forces.insert((Force*)newforce);
      force_name_to_forceimpl.insert(std::make_pair(*i, newforce));
    }
    for (int i = 0; i < 12; i++)
    {
      if (this->players[i] != NULL && this->players[i]->getName().length() > 0)
      {
        ForceImpl* force = force_name_to_forceimpl.find(std::string(this->players[i]->getForceName()))->second;
        force->players.insert(this->players[i]);
        this->players[i]->force = force;
      }
    }
  }

  //---------------------------------------------- ON SEND TEXT ----------------------------------------------
  bool GameImpl::onSendText(const char* text)
  {
    if (!this->parseText(text) && this->isFlagEnabled(BWAPI::Flag::UserInput))
    {
      if (this->client!=NULL)
      {
        return !this->client->onSendText(std::string(text));
      }
    }
    return true;
  }
  //----------------------------------------------- PARSE TEXT -----------------------------------------------
  bool GameImpl::parseText(const char* text)
  {
    std::string message = text;
    std::vector<std::string> parsed = Util::Strings::splitString(message, " ");
    if (parsed.size() < 3)
      parsed.push_back("");
    if (parsed.size() < 3)
      parsed.push_back("");
    if (parsed[0] == "/surrender")
    {
      this->surrender();
    }
    else if (parsed[0] == "/latency")
    {
      printf("latency: %d",getLatency());
      return true;
    }
    else if (parsed[0] == "/save")
    {
      if (parsed[1] == "allies")
      {
        std::set<Player*> players=this->getPlayers();
        for(std::set<Player*>::iterator i=players.begin();i!=players.end();i++)
        {
          for(std::set<Player*>::iterator j=players.begin();j!=players.end();j++)
          {
            BWAPI::Broodwar->printf("%s[%d] alliance data for %s[%d]: %d", (*i)->getName().c_str(), (*i)->getID(), (*j)->getName().c_str(), (*j)->getID(),(int)((PlayerImpl*)(*i))->getAlliance((*j)->getID()));
            Util::Logger::globalLog->log("%s[%d] alliance data for %s[%d]: %d", (*i)->getName().c_str(), (*i)->getID(), (*j)->getName().c_str(), (*j)->getID(),(int)((PlayerImpl*)(*i))->getAlliance((*j)->getID()));
          }
        }
      }
      else if (parsed[1] == "unit")
      {
        std::ofstream unitData;
        unitData.open ("bwapi-data/unitData.txt");
        for (std::set<BWAPI::UnitType>::const_iterator u = BWAPI::UnitTypes::allUnitTypes().begin(); u != BWAPI::UnitTypes::allUnitTypes().end(); u++)
        {
          std::pair<const BWAPI::UnitType*, int> requiredUnits[3];
          BW::UnitType bwu((BW::UnitID::Enum)u->getID());
          requiredUnits[0] = requiredUnits[1] = requiredUnits[2] = std::make_pair((BWAPI::UnitType*)NULL, 0);
          int i = 0;
          for (std::map<const BWAPI::UnitType*, int>::const_iterator r = u->requiredUnits().begin(); r != u->requiredUnits().end(); r++)
          {
            requiredUnits[i++] = *r;
          }

          unitData << "      unitTypeData[" << Util::Strings::stringToVariableName(u->getName()) << ".getID()].set(\""
          << u->getName() << "\",\"";
          if (*u != UnitTypes::None && *u != UnitTypes::Unknown)
          {
            unitData << std::string(bwu.getSubLabel());
          }
          unitData << "\",Races::" << Util::Strings::stringToVariableName(u->getRace().getName()) << ",&("
          << Util::Strings::stringToVariableName(u->whatBuilds().first->getName()) << "),"
          << u->whatBuilds().second << ",";
          for (int i = 0; i < 3; i++)
          {
            if (requiredUnits[i].first == NULL)
            {
              unitData << "NULL,";
            }
            else
            {
              unitData << "&(" << Util::Strings::stringToVariableName(requiredUnits[i].first->getName()) << "),";
            }
            unitData << requiredUnits[i].second << ",";
          }
          unitData << "&(TechTypes::" << Util::Strings::stringToVariableName(u->requiredTech()->getName()) << "),";
          int count=0;
          for(std::set<TechType>::iterator t=TechTypes::allTechTypes().begin();t!=TechTypes::allTechTypes().end();t++)
          {
            for(std::set<const UnitType*>::const_iterator u2=t->whatUses().begin();u2!=t->whatUses().end();u2++)
            {
              if ((*u2)->getID()==u->getID())
              {
                unitData << "&(TechTypes::" << Util::Strings::stringToVariableName(t->getName()) << "),";
                count++;
              }
            }
          }
          for(int c=count;c<4;c++)
          {
            unitData << "&(TechTypes::None),";
          }
          if (*u != UnitTypes::None && *u != UnitTypes::Unknown)
          {
            unitData << "&(UpgradeTypes::";
            if (BWAPI::UpgradeType(bwu.armorUpgrade())==UpgradeTypes::Unknown)
            {
              unitData << "None),";
            }
            else
            {
              unitData << Util::Strings::stringToVariableName(BWAPI::UpgradeType(bwu.armorUpgrade()).getName()) << "),";
            }
            unitData << (int)bwu.maxHitPoints() << "," << (int)bwu.maxShields() << "," << (int)bwu.maxEnergy() << ","
            << (int)bwu.armor() << "," << (int)bwu.mineralPrice() << "," << (int)bwu.gasPrice() << ","
            << (int)bwu.buildTime() << "," << (int)bwu.supplyRequired() << "," << (int)bwu.supplyProvided() << ","
            << (int)bwu.spaceRequired() << "," << (int)bwu.spaceProvided() << "," << (int)bwu.buildScore() << ","
            << (int)bwu.destroyScore() << ",&(UnitSizeTypes::" << BWAPI::UnitSizeType((int)bwu.size()).getName() << ")," << (int)bwu.tileWidth() << ","
            << (int)bwu.tileHeight() << "," << (int)bwu.dimensionLeft() << "," << (int)bwu.dimensionUp() << ","
            << (int)bwu.dimensionRight() << "," << (int)bwu.dimensionDown() << "," << (int)bwu.seekRange() << ","
            << (int)bwu.sightRange() << ",&(WeaponTypes::"
            << Util::Strings::stringToVariableName(BWAPI::WeaponType((int)bwu.groundWeapon()).getName()) << "),"
            << (int)bwu.maxGroundHits() << ",&(WeaponTypes::"
            << Util::Strings::stringToVariableName(BWAPI::WeaponType((int)bwu.airWeapon()).getName()) << "),"
            << (int)bwu.maxAirHits() << "," << (int)bwu.topSpeed() << "," << (int)bwu.acceleration() << ","
            << (int)bwu.haltDistance() << "," << (int)bwu.turnRadius() << ","
            << bwu.canProduce() << "," << bwu.canAttack() << "," << bwu.canMove() << ","
            << bwu.isFlyer() << "," << bwu.regeneratesHP() << "," << bwu.isSpellcaster() << ","
            << bwu.hasPermanentCloak() << "," << bwu.isInvincible() << "," << bwu.isOrganic() << ","
            << bwu.isMechanical() << "," << bwu.isRobotic() << "," << bwu.isDetector() << ","
            << bwu.isResourceContainer() << "," << bwu.isResourceDepot() << "," << bwu.isWorker() << ","
            << bwu.requiresPsi() << "," << bwu.requiresCreep() << "," << bwu.isTwoUnitsInOneEgg() << ","
            << bwu.isBurrowable() << "," << bwu.isCloakable() << "," << bwu.isBuilding() << ","
            << bwu.isAddon() << "," << bwu.isFlyingBuilding() << "," << bwu.isNeutral() << ","
            << bwu.isRefinery()
            << ");\n";
          }
          else
          {
            unitData << "&(UpgradeTypes::None), 0,0,0, 0,0,0, 0,0,0, 255,0,0, 0,&(UnitSizeTypes::None),0, 0,0,0, 0,0,0, 0,&(WeaponTypes::None), 0,&(WeaponTypes::None),0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);\n";
          }
        }
        unitData.close();
      }
      else if (parsed[1] == "weapon")
      {
        std::ofstream weaponData;
        weaponData.open ("bwapi-data/weaponData.txt");
        for (std::set<BWAPI::WeaponType>::const_iterator w = BWAPI::WeaponTypes::allWeaponTypes().begin(); w != BWAPI::WeaponTypes::allWeaponTypes().end(); w++)
        {
          BW::WeaponType bww((BW::WeaponID::Enum)w->getID());

          weaponData << "      weaponTypeData[" << Util::Strings::stringToVariableName(w->getName()) << ".getID()].set(\""
          << w->getName() << "\",";
          if (*w == BWAPI::WeaponTypes::None || *w == BWAPI::WeaponTypes::Unknown)
          {
            weaponData << "0,0,0,0,&(UpgradeTypes::None),&(DamageTypes::None),&(ExplosionTypes::None),0,0,0,0,0,0,0,0,0,0,0,0,0,0);";
          }
          else
          {
            weaponData << (int)bww.damageAmount() << "," << (int)bww.damageBonus() << ","
            << (int)bww.damageCooldown() << "," << (int)bww.damageFactor() << ",&(UpgradeTypes::"
            << Util::Strings::stringToVariableName(bww.upgradeType().getName()) << "),&(DamageTypes::"
            << Util::Strings::stringToVariableName(BWAPI::DamageType(bww.damageType()).getName()) << "),&(ExplosionTypes::"
            << Util::Strings::stringToVariableName(BWAPI::ExplosionType(bww.explosionType()).getName())
            << ")," << bww.minRange() << "," << bww.maxRange() << "," << bww.innerSplashRadius() << ","
            << bww.medianSplashRadius() << "," << bww.outerSplashRadius() << "," << bww.targetsAir() << ","
            << bww.targetsGround() << "," << bww.targetsMechanical() << "," << bww.targetsOrganic() << ","
            << bww.targetsNonBuilding() << "," << bww.targetsNonRobotic() << "," << bww.targetsTerrain() << ","
            << bww.targetsOrgOrMech() << "," << bww.targetsOwn()
            << ");\n";
          }
        }
        weaponData.close();
      }
      else if (parsed[1] == "unitsDat")
      {

        struct unitsDat_Unknown
        {
          u8 unitType[BW::UNIT_TYPE_COUNT];
        };
        static unitsDat_Unknown* BWDATA_UnitUnknown = (unitsDat_Unknown*) BW::unitsDat[25].address;
        std::ofstream unitsDat;
        unitsDat.open("bwapi-data/unitsDat.txt");
        for (std::set<UnitType>::const_iterator i = UnitTypes::allUnitTypes().begin(); i != UnitTypes::allUnitTypes().end(); i++)
        {
          BW::UnitType bwu((BW::UnitID::Enum)i->getID());
          unitsDat << "(" << i->getID() << ")" << i->getName() << ", seek range: " << (int)bwu.seekRange() << "\n";

          //unitsDat << "(" << (int)BW::WeaponType(BW::UnitType(BW::UnitID::Enum(i->getID())).groundWeapon()).getID() << ")" << BW::WeaponType(BW::UnitType(BW::UnitID::Enum(i->getID())).groundWeapon()).getName() << "\n";
          //unitsDat << "(" << (int)BW::WeaponType(BW::UnitType(BW::UnitID::Enum(i->getID())).airWeapon()).getID() << ")" << BW::WeaponType(BW::UnitType(BW::UnitID::Enum(i->getID())).airWeapon()).getName() << "\n";
        }
        unitsDat.close();
      }
      else if (parsed[1] == "weaponsDat")
      {
        std::ofstream weaponsDat;
        weaponsDat.open("bwapi-data/weaponsDat.txt");
        for (std::set<WeaponType>::const_iterator i = WeaponTypes::allWeaponTypes().begin(); i != WeaponTypes::allWeaponTypes().end(); i++)
        {
          weaponsDat << i->getID() << ": " << std::string(i->getName()) << "\n";
          weaponsDat << "  damage amount:" << i->damageAmount() << "\n";
          weaponsDat << "  damage bonus:" << i->damageBonus() << "\n";
          weaponsDat << "  damage cooldown:" << i->damageCooldown() << "\n";
          weaponsDat << "  damage factor:" << i->damageFactor() << "\n";
          weaponsDat << "  damage type:" << i->damageType()->getName() << "(" << i->damageType()->getID() << ")\n";
          weaponsDat << "  explosion type:" << i->explosionType()->getName() << "(" << i->explosionType()->getID() << ")\n";
          weaponsDat << "  min range:" << i->minRange() << ", max range: " << i->maxRange() << "\n";
          weaponsDat << "  targets ground:" << i->targetsGround() << ", targetsAir: " << i->targetsAir() << "\n";
        }
        weaponsDat.close();
      }
      else if (parsed[1] == "upgradesMax")
      {
        std::ofstream upgradesMax;
        upgradesMax.open("bwapi-data/upgradesMax.txt");
        for (std::set<UpgradeType>::const_iterator i = UpgradeTypes::allUpgradeTypes().begin(); i != UpgradeTypes::allUpgradeTypes().end(); i++)
        {
          BW::UpgradeType ut=BW::UpgradeType(BW::UpgradeID::Enum((*i).getID()));
          upgradesMax << i->getID() << ": " << (int)ut.upgradesMax() << "\n"; 
        }
        upgradesMax.close();
      }
      else
      {
        this->printf("Unknown command '%s''s - possible commands are: unit", parsed[1].c_str());
      }
      return true;
    }
    else if (parsed[0] == "/unit")
    {
      if (parsed[1] == "info")
      {
        for (u16 i = 0; savedSelectionStates[i] != NULL; i++)
          this->printf(BWAPI::UnitImpl::BWUnitToBWAPIUnit(savedSelectionStates[i])->getName().c_str());
      }
      else if (parsed[1] == "data")
      {
        if (savedSelectionStates[0] != NULL)
        {
          BW::Unit* unit = savedSelectionStates[0];
          Util::Logger::globalLog->log("Unit address: %x", unit);
          for (__int8 i = 0; i < BW::UNIT_SIZE_IN_BYTES; i++)
          {
            Util::Logger::globalLog->log("%x: %x", i, ((__int8*)unit)[i] % 256);
          }
        }
      }
      else if (parsed[1] == "first")
      {
        for (int i = 0; i < 12; i++)
        {
          if (this->players[i]->getFirst() != NULL)
          {
            this->printf("%d - %x", i, this->players[i]->getFirst());
          }
          else
          {
            this->printf("%d - NULL", i);
          }
        }
      }
      else
        this->printf("Unknown command '%s''s - possible commands are: data, info", parsed[1].c_str());
      return true;
    }
    return false;
  }
  //---------------------------------------------- ON GAME END -----------------------------------------------
  void GameImpl::onGameEnd()
  {
    this->setOnStartCalled(false);
    this->client->onEnd();
    if (this->client != NULL)
    {
      delete this->client;
      this->client=NULL;
    }
    this->commandBuffer.clear();
    FreeLibrary(hMod);
    Util::Logger::globalLog->logCritical("Unloaded AI Module");
    for (int i = 0; i < 13; i++)
    {
      this->savedSelectionStates[i] = NULL;
    }
    this->invalidIndices.clear();
    this->selectedUnitSet.clear();
    this->reselected = false;
    this->startedClient = false;
    if (WAIT_OBJECT_0 == ::WaitForSingleObject(hcachedShapesMutex, INFINITE))
    {
      for (unsigned int i = 0; i < this->cachedShapes.size(); i++)
      {
        delete this->cachedShapes[i];
      }
      this->cachedShapes.clear();
      ::ReleaseMutex(hcachedShapesMutex);
    }
    this->shapes.clear();
  }
  //----------------------------------------------- START GAME -----------------------------------------------
  void GameImpl::startGame()
  {
    this->IssueCommand((PBYTE)&BW::Orders::StartGame(), sizeof(BW::Orders::StartGame));
  }
  //----------------------------------------------- PAUSE GAME -----------------------------------------------
  void GameImpl::pauseGame()
  {
    this->IssueCommand((PBYTE)&BW::Orders::PauseGame(), sizeof(BW::Orders::PauseGame));
  }
  //---------------------------------------------- RESUME GAME -----------------------------------------------
  void GameImpl::resumeGame()
  {
    this->IssueCommand((PBYTE)&BW::Orders::ResumeGame(), sizeof(BW::Orders::ResumeGame));
  }
  //---------------------------------------------- SURRENDER -----------------------------------------------
  void GameImpl::surrender()
  {
    *BW::BWDATA_QuitMission_UNKNOWN1=0xFFFFFFFE;
    *BW::BWDATA_QuitMission_UNKNOWN2=0x00000001;
    *BW::BWDATA_QuitMission_UNKNOWN3=0x00000000;
    *BW::BWDATA_QuitMission_UNKNOWN4=0x000001BD;
    u32 var1=2;
    u32 ptr1=(u32)(&var1);
    //Most of the following ASM was taking from the function at 004C95A0
    __asm
    {
      MOV EAX,0x00000000
      MOV EBX,0x00000000
      MOV ECX,0x02A59BBC
      MOV EDX,ptr1
      MOV ESI,ptr1

      MOV EAX,DWORD PTR DS:[BW::BWDATA_QuitMission_UNKNOWN1]
      PUSH EBX
      XOR EBX,EBX
      CMP EAX, -2
      PUSH ESI
      MOV ESI, ECX
      MOV DWORD PTR DS:[0x6D1234],EBX
      MOV EDX,DWORD PTR DS:[BW::BWDATA_QuitMission_UNKNOWN2]
      MOV EAX,DWORD PTR DS:[BW::BWDATA_QuitMission_UNKNOWN3]
      MOV BYTE PTR DS:[0x6D11EC],BL
      MOV WORD PTR DS:[0x51CE90],7
      MOV ECX,DWORD PTR DS:[BW::BWDATA_QuitMission_UNKNOWN4]
      MOV DWORD PTR DS:[0x6D0F31],ECX
      POP ESI
      POP EBX
    }
  }
  //---------------------------------------------- GET MOUSE X -----------------------------------------------
  int GameImpl::getMouseX() const
  {
    if (this->isFlagEnabled(BWAPI::Flag::UserInput) == false)
      return 0;
    return *(BW::BWDATA_MouseX);
  }
  //---------------------------------------------- GET MOUSE Y -----------------------------------------------
  int GameImpl::getMouseY() const
  {
    if (this->isFlagEnabled(BWAPI::Flag::UserInput) == false)
      return 0;
    return *(BW::BWDATA_MouseY);
  }
  //---------------------------------------------- GET SCREEN X ----------------------------------------------
  int GameImpl::getScreenX() const
  {
    if (this->isFlagEnabled(BWAPI::Flag::UserInput) == false)
      return 0;
    return *(BW::BWDATA_ScreenX);
  }
  //---------------------------------------------- GET SCREEN Y ----------------------------------------------
  int GameImpl::getScreenY() const
  {
    if (this->isFlagEnabled(BWAPI::Flag::UserInput) == false)
      return 0;
    return *(BW::BWDATA_ScreenY);
  }
  //------------------------------------------- SET SCREEN POSITION ------------------------------------------
  void GameImpl::setScreenPosition(int x, int y)
  {
    *(BW::BWDATA_ScreenX) = x;
    *(BW::BWDATA_ScreenY) = y;
  }
  //---------------------------------------------- GET MOUSE X -----------------------------------------------
  int GameImpl::_getMouseX() const
  {
    return *(BW::BWDATA_MouseX);
  }
  //---------------------------------------------- GET MOUSE Y -----------------------------------------------
  int GameImpl::_getMouseY() const
  {
    return *(BW::BWDATA_MouseY);
  }
  //---------------------------------------------- GET SCREEN X ----------------------------------------------
  int GameImpl::_getScreenX() const
  {
    return *(BW::BWDATA_ScreenX);
  }
  //---------------------------------------------- GET SCREEN Y ----------------------------------------------
  int GameImpl::_getScreenY() const
  {
    return *(BW::BWDATA_ScreenY);
  }
  //----------------------------------------------------------------------------------------------------------
#pragma warning(push)
#pragma warning(disable:4312)
  void GameImpl::refresh()
  {
    void (_stdcall* refresh)(void) = (void (_stdcall*) ())BW::BWFXN_Refresh;
    refresh();
  }
#pragma warning(pop)
  //----------------------------------------------------------------------------------------------------------
  UnitImpl* GameImpl::getUnit(int index)
  {
    return this->unitArray[index];
  }
  //--------------------------------------------- SAVE SELECTED ----------------------------------------------
  void GameImpl::saveSelected()
  {
    this->reselected = false;
    memcpy(&savedSelectionStates, BW::BWDATA_CurrentPlayerSelectionGroup, 4*12);
    savedSelectionStates[12] = NULL;
    int i = 0;
    selectedUnitSet.clear();
    while (savedSelectionStates[i] != NULL)
    {
      selectedUnitSet.insert(UnitImpl::BWUnitToBWAPIUnit(savedSelectionStates[i]));
      i++;
    }
  }
  //--------------------------------------------- LOAD SELECTED ----------------------------------------------
  void GameImpl::loadSelected()
  {
    if (!this->reselected)
    {
      return;
    }

    int unitCount = 0;
    while (savedSelectionStates[unitCount] != NULL)
      unitCount ++;
    if (quietSelect)
      BW::selectUnits(unitCount, savedSelectionStates);
    else
      BW::selectUnitsHelperSTD(unitCount, savedSelectionStates, true, true);
  }
  //------------------------------------------ GET SELECTED UNITS --------------------------------------------
  const std::set<BWAPI::Unit*>& GameImpl::getSelectedUnits() const
  {
    if (this->isFlagEnabled(BWAPI::Flag::UserInput) == false)
      return emptySet;
    return selectedUnitSet;
  }
  //-------------------------------------------- ON CANCEL TRAIN ---------------------------------------------
  void GameImpl::onCancelTrain()
  {
    try
    {
      if (savedSelectionStates[0] != NULL)
        this->addToCommandBuffer(new CommandCancelTrain(BWAPI::UnitImpl::BWUnitToBWAPIUnit(savedSelectionStates[0])));
    }
    catch (GeneralException& exception)
    {
      Util::Logger::globalLog->log("Exception in onCancelTrain: %s", exception.getMessage().c_str());
    }
  }
  //--------------------------------------------- ON REMOVE UNIT ---------------------------------------------
  void GameImpl::onRemoveUnit(BW::Unit* unit)
  {
    if (this->client != NULL)
    {
      this->client->onRemoveUnit(BWAPI::UnitImpl::BWUnitToBWAPIUnit(unit));
    }
  }
  //----------------------------------------- LOG UNKNOWN OR STRANGE -----------------------------------------
  void GameImpl::logUnknownOrStrange()
  {
    for each (UnitImpl* i in this->units)
    {
      if (i==NULL)
      {
        this->newUnitLog->log("Error: NULL pointer");
      }
      else
      {
        if (!i->getBWType().isValid())
          this->newUnitLog->log("%s", i->getName().c_str());
      }
    }
  }
  //--------------------------------------------- LOG UNIT LIST ----------------------------------------------
  void GameImpl::logUnitList()
  {
    this->unitSum->log("----------------------------------------");
    for each (UnitImpl* i in this->units)
      this->unitSum->log("%s", i->getName().c_str());
    this->unitSum->log("----------------------------------------");
  }
  //----------------------------------------------- GET FIRST ------------------------------------------------
  UnitImpl* GameImpl::getFirst()
  {
    return UnitImpl::BWUnitToBWAPIUnit(*BW::BWDATA_UnitNodeTable_FirstElement);
  }
  //---------------------------------------------- GET LATENCY -----------------------------------------------
  BWAPI::Latency::Enum GameImpl::getLatency()
  {
    if (isSinglePlayer())
    {
      return BWAPI::Latency::SinglePlayer;
    }
    switch(*BW::BWDATA_Latency)
    {
      case 0:
        return BWAPI::Latency::LanLow;
      case 1:
        return BWAPI::Latency::LanMedium;
      case 2:
        return BWAPI::Latency::LanHigh;
      default:
        return BWAPI::Latency::LanLow;
    }
  }
  //--------------------------------------- PRINT UNIT COUNT PER TILE ----------------------------------------
  void GameImpl::printUnitCountPerTile()
  {
    FILE* f = fopen("unit-counts.txt", "wt");
    for (int y = 0; y < Map::getHeight(); y++)
    {
      for (int x = 0; x < Map::getWidth(); x++)
      {
        if (this->map.buildable(x, y))
          fprintf_s(f, "%d", this->unitsOnTile(x, y).size());
        else
          fprintf_s(f, "X");
      }
      fprintf_s(f, "\n");
    }
    fclose(f);
  }
  //------------------------------------------ UPDATE UNITS ON TILE ------------------------------------------
  void GameImpl::updateUnitsOnTile()
  {
    this->unitsOnTileData.resize(Map::getWidth(), Map::getHeight());
    for (int y = 0; y < Map::getHeight(); y++)
      for (int x = 0; x < Map::getWidth(); x++)
        this->unitsOnTileData[x][y].clear();
    for each (UnitImpl* i in this->units)
      if (i->isValid())
      {
        int startX =   (i->getPosition().x() - i->getType().dimensionLeft()) / BW::TILE_SIZE;
        int endX   =   (i->getPosition().x() + i->getType().dimensionRight() + BW::TILE_SIZE - 1) / BW::TILE_SIZE; // Division - round up
        int startY =   (i->getPosition().y() - i->getType().dimensionUp()) / BW::TILE_SIZE;
        int endY =     (i->getPosition().y() + i->getType().dimensionDown() + BW::TILE_SIZE - 1) / BW::TILE_SIZE;
        for (int x = startX; x < endX; x++)
          for (int y = startY; y < endY; y++)
            this->unitsOnTileData[x][y].insert(i);
      }
  }
  //--------------------------------------------- GET FRAME COUNT --------------------------------------------
  int GameImpl::getFrameCount() const
  {
    return this->frameCount;
  }
  //--------------------------------------------- UNITS ON TILE ----------------------------------------------
  std::set<Unit*> GameImpl::unitsOnTile(int x, int y) const
  {
    if (this->isFlagEnabled(Flag::CompleteMapInformation) || visible(x,y))
    {
      return unitsOnTileData[x][y];
    }
    std::set<Unit*> emptySet;
    return emptySet;
  }
  //--------------------------------------------- GET LAST ERROR ---------------------------------------------
  Error GameImpl::getLastError() const
  {
    return this->lastError;
  }
  //--------------------------------------------- SET LAST ERROR ---------------------------------------------
  void GameImpl::setLastError(BWAPI::Error e)
  {
    this->lastError = e;
  }
  //--------------------------------------------- IS FLAG ENABLED --------------------------------------------
  bool GameImpl::isFlagEnabled(BWAPI::Flag::Enum flag) const
  {
    return this->flags[flag];
  }
  //----------------------------------------------- ENABLE FLAG ----------------------------------------------
  void GameImpl::enableFlag(BWAPI::Flag::Enum flag)
  {
    if (this->flagsLocked == true)
    {
      this->sendText("Flags can only be enabled at the start of a game.");
      return;
    }
    this->flags[flag] = true;
    if (flag == BWAPI::Flag::CompleteMapInformation)
    {
      this->sendText("Enabled Flag CompleteMapInformation");
    }
    if (flag == BWAPI::Flag::UserInput)
    {
      this->sendText("Enabled Flag UserInput");
    }
  }
  //-------------------------------------------------- LOCK FLAGS --------------------------------------------
  void GameImpl::lockFlags()
  {
    this->flagsLocked = true;
  }
  //----------------------------------------------------- SELF -----------------------------------------------
  Player* GameImpl::self() const
  {
    return (Player*)this->BWAPIPlayer;
  }
  //----------------------------------------------------- ENEMY ----------------------------------------------
  Player* GameImpl::enemy() const
  {
    return (Player*)this->opponent;
  }
  //----------------------------------------------------- DRAW -----------------------------------------------
  void GameImpl::addShape(Shape* s)
  {
    this->shapes.push_back(s);
  }
  void GameImpl::drawBox(CoordinateType::Enum ctype, int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    addShape(new ShapeBox(ctype, left, top, right, bottom, color.getID(), isSolid));
  }
  void GameImpl::drawTriangle(CoordinateType::Enum ctype, int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    addShape(new ShapeTriangle(ctype, ax, ay, bx, by, cx, cy, color.getID(), isSolid));
  }
  void GameImpl::drawDot(CoordinateType::Enum ctype, int x, int y, Color color)
  {
    addShape(new ShapeDot(ctype, x, y, color.getID()));
  }
  void GameImpl::drawCircle(CoordinateType::Enum ctype, int x, int y, int radius, Color color, bool isSolid)
  {
    addShape(new ShapeCircle(ctype, x, y, radius, color.getID(), isSolid));
  }
  void GameImpl::drawEllipse(CoordinateType::Enum ctype, int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    addShape(new ShapeEllipse(ctype, x, y, xrad, yrad, color.getID(), isSolid));
  }
  void GameImpl::drawLine(CoordinateType::Enum ctype, int x1, int y1, int x2, int y2, Color color)
  {
    int screen_x1 = x1;
    int screen_y1 = y1;
    int screen_x2 = x2;
    int screen_y2 = y2;
    if (ctype == 2)
    {
      screen_x1 = x1 - BWAPI::BroodwarImpl._getScreenX();
      screen_y1 = y1 - BWAPI::BroodwarImpl._getScreenY();
      screen_x2 = x2 - BWAPI::BroodwarImpl._getScreenX();
      screen_y2 = y2 - BWAPI::BroodwarImpl._getScreenY();
    }
    else if (ctype == 3)
    {
      screen_x1 = x1 + BWAPI::BroodwarImpl._getMouseX();
      screen_y1 = y1 + BWAPI::BroodwarImpl._getMouseY();
      screen_x2 = x2 + BWAPI::BroodwarImpl._getMouseX();
      screen_y2 = y2 + BWAPI::BroodwarImpl._getMouseY();
    }
    if ((screen_x1 < 0 && screen_x2 < 0) ||
        (screen_y1 < 0 && screen_y2 < 0) ||
        (screen_x1 > 640 && screen_x2 > 640) ||
        (screen_y1 > 480 && screen_y2 > 480)) return;
    addShape(new ShapeLine(ctype, x1, y1, x2, y2, color.getID()));
  }
  void GameImpl::text(CoordinateType::Enum ctype, int x, int y, const char* text, ...)
  {
    va_list ap;
    va_start(ap, text);
    vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
    va_end(ap);
    addShape(new ShapeText(ctype,x,y,std::string(buffer)));
  }
};