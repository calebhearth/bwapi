#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used stuff from Windows headers

#include "Engine.h"
#include "Unit.h"   // BWAPI Unit
#include "Map.h"
#include "BridgeServer.h"
#include "Shape.h"

#include <DLLMain.h>

#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <fstream>
#include <stdexcept>

#include <Util\Version.h>
#include <Util\FileLogger.h>
#include <Util\Exceptions.h>
#include <Util\Strings.h>
#include <Util\Foreach.h>
#include <Util\Gnu.h>
#include <Util\FlagArray.h>
#include <Util\LookupTable.h>

#include <BW/Broodwar.h>
#include <BW/Hook.h>
#include <BW/Unit.h>
#include <BW/UnitArray.h>
#include <BW/Offsets.h>
#include <BW/UnitID.h>
#include <BW/Command.h>
#include <BW/Latency.h>
#include <BW/TileType.h>
#include <BW/TileSet.h>
#include <BW/UnitType.h>
#include <BW/GameType.h>
#include <BW/WeaponType.h>
#include <BW/CheatType.h>
#include <BW/RaceID.h>

#include <BWAPITypes\BuildPosition.h>
#include <BWAPITypes\WalkPosition.h>
#include <BWAPITypes\Position.h>
#include <BWAPITypes\UnitType.h>
#include <BWAPITypes\Latency.h>
#include <BWAPITypes\Flag.h>
#include <BWAPITypes\UnitCommandTypeId.h>
#include <BWAPITypes\UnitCommand.h>
#include <BWAPITypes\UnitState.h>

#include <Bridge/PipeMessage.h>
#include <Bridge/SharedStuff.h>
#include <Bridge/Constants.h>

#include <Tracer\Tracer.h>

namespace BWAPI
{
  namespace Engine
  {
//private:
    //----------------------------------- ENGINE STATE --------------------------------
    // broodwar game state
    enum GameState
    {
      Startup,
      InMenu,
      InMatch
    };
    GameState gameState = Startup;

    // match state TODO: move some things to ::Map
    int frameCount;
    std::set<BW::UnitType> unitTypes;
    std::set<BW::Position> startLocations;
    BW::Unit* savedSelectionStates[13];

    // reflects needed states from last frame to detect add and remove events.
    // is index correlated with BW::getUnitArray();
    BWAPI::Unit bwUnitArrayMirror[BW::UNIT_ARRAY_MAX_LENGTH];

    // index correlated with the unit array mirror. Used for performance optimisation
    Util::FlagArray bwUnitArrayMirrorFlags;

    // BWAPI state
    bool flags[Flags::count];

    // error handling
    Util::Logger* newUnitLog;
    Util::Logger* commandLog;   // All commands ordered from BWAPI
    //------------------------------- -------------------------------------------------
    // TODO: enlist all private functions defined in Engine here
    //------------------------------- -------------------------------------------------
    void init();
    bool parseText(const char* text);
    void setLocalSpeed(int speed);

    void simulateUnitCommand(const BWAPI::UnitCommand &simulatedCommand, BWAPI::UnitState &stateToAlter);
    void executeUnitCommand(const BWAPI::UnitCommand& command);
    //---------------------------------------------- INITIALIZATION --------------------------------------------
    void init()
    {
      /* initialize the unit types */
      BW::UnitType::initialize();

      try
      {
        // create log handles
        commandLog = new Util::FileLogger(std::string(logPath) + "\\commands", Util::LogLevel::MicroDetailed);
        newUnitLog = new Util::FileLogger(std::string(logPath) + "\\new_unit_id", Util::LogLevel::MicroDetailed);

        // iterate through unit types and create UnitType for each
        for (int i = 0; i < BW::UNIT_TYPE_COUNT; i++)
          unitTypes.insert(BW::UnitType((u16)i));
      }
      catch (GeneralException& exception)
      {
        FILE*f = fopen("bwapi-error", "a+");
        fprintf(f, "Exception caught inside Game constructor: %s", exception.getMessage().c_str());
        fclose(f);
      }
    }
    //----------------------------------------------- ENABLE FLAG ----------------------------------------------
    void  enableFlag(int flag)
    {
      // Enable the specified flag
      if (flag >= Flags::count)
      {
        BW::sendText("Invalid flag (%d).", flag);
        return;
      }

      // Modify flag state
      flags[flag] = true;
      switch(flag)
      {
      case Flags::CompleteMapInformation:
        BW::sendText("Enabled Flag CompleteMapInformation");
        break;
      case Flags::UserInput:
        BW::sendText("Enabled Flag UserInput");
        break;
      }
    }
    //--------------------------------------------- SAVE SELECTED ----------------------------------------------
    void saveSelected()
    {
      memcpy(&savedSelectionStates, BW::BWDATA_CurrentPlayerSelectionGroup, 4*12);
      savedSelectionStates[12] = NULL;
    }
    //--------------------------------------------- LOAD SELECTED ----------------------------------------------
    void loadSelected()
    {
      int unitCount = 0;
      while (savedSelectionStates[unitCount] != NULL)
        unitCount++;
      BW::selectUnits(unitCount, savedSelectionStates);
    }
    //------------------------------------------------- ON DLL LOAD --------------------------------------------
    void onDllLoad()
    {
      // stop!!!
      // please reconsider, this thread is not the starcraft thread. hardly foreseeable future bugs can
      // be avoided altogether by moving initialisation code to update()
      // throw something here to unload the dll and signal dll initialisation as failed
    }
    //--------------------------------------------- ON MATCH START ---------------------------------------------
    void onMatchStart()
    {

      /* TODO: reform that? oh geez
      // initialize the variables
      frameCount = 0;
      BWAPIPlayer = NULL;
      opponent = NULL;

      // set all the flags to the default of disabled
      for (int i = 0; i < Flags::count; i++)
        flags[i] = false;
      flagsLocked = false;

      // load the map data
      map.load();

      if (*(BW::BWDATA_InReplay)) // set replay flags
      {
        for (int i = 0; i < Flags::count; i++)
          flags[i] = true;
        flagsLocked = false;
      }
      else
      {
        // find the current player by name
        for (int i = 0; i < BW::PLAYABLE_PLAYER_COUNT; i++)
          if (strcmp(BW::BWDATA_CurrentPlayer, players[i]->getName().c_str()) == 0)
            BWAPIPlayer = players[i];

        // error if player not found
        if (BWAPIPlayer == NULL)
        {
          commandLog->log("Error: Could not locate BWAPI player.");
          return;
        }

        // find the opponent player
        for (int i = 0; i < BW::PLAYABLE_PLAYER_COUNT; i++)
          if ((players[i]->playerType() == BW::PlayerType::Computer ||
               players[i]->playerType() == BW::PlayerType::Human ||
               players[i]->playerType() == BW::PlayerType::ComputerSlot) &&
              opponent == NULL &&
              BWAPIPlayer->isEnemy(players[i]))
            opponent = players[i];

        // error if opponent not found
        if (opponent == NULL)
          commandLog->log("Warning: Could not find any opponent");
      }

      // get the start locations
      BW::Positions* posptr = BW::BWDATA_startPositions;
      startLocations.clear();
      playerSet.clear();
      forces.clear();
      while (posptr->x != 0 || posptr->y != 0)
      {
        startLocations.insert(BWAPI::TilePosition((int)((posptr->x - BW::TILE_SIZE*2)          / BW::TILE_SIZE),
                                                  (int)((posptr->y - (int)(BW::TILE_SIZE*1.5)) / BW::TILE_SIZE)));
        posptr++;
      }

      // get force names
      std::set<std::string> force_names;
      std::map<std::string, ForceImpl*> force_name_to_forceimpl;
      for (int i = 0; i < BW::PLAYER_COUNT; i++)
        if (players[i] != NULL && players[i]->getName().length() > 0)
        {
          force_names.insert(std::string(players[i]->getForceName()));
          playerSet.insert(players[i]);
        }

      // create ForceImpl for force names
      foreach (std::string i, force_names)
      {
        ForceImpl* newforce = new ForceImpl(i);
        forces.insert((Force*)newforce);
        force_name_to_forceimpl.insert(std::make_pair(i, newforce));
      }

      // create ForceImpl for players
      for (int i = 0; i < BW::PLAYER_COUNT; i++)
        if (players[i] != NULL && players[i]->getName().length() > 0)
        {
          ForceImpl* force = force_name_to_forceimpl.find(std::string(players[i]->getForceName()))->second;
          force->players.insert(players[i]);
          players[i]->force = force;
        }
      unitsOnTileData.resize(Map::getWidth(), Map::getHeight());
      */
    }
    //------------------------------------------------- TRANSFER MATCH START DATA ------------------------------
    void transferMatchStartData()
    {
      BWAPI::StaticGameData &staticData = *BridgeServer::gameData;

      for (int x=0;x<Map::getWidth()*4;x++)
        for (int y=0;y<Map::getHeight()*4;y++)
        {
          staticData.getGroundHeight[x][y] = Map::groundHeight(x,y);
          staticData.isWalkable[x][y] = Map::walkable(x,y);
        }
      for (int x=0;x<Map::getWidth();x++)
        for (int y=0;y<Map::getHeight();y++)
          staticData.isBuildable[x][y] = Map::buildable(x,y);

      staticData.mapFilename.set(Map::getFileName());

      staticData.mapName.set(Map::getName());

      staticData.mapWidth      = Map::getWidth();
      staticData.mapHeight     = Map::getHeight();
      staticData.mapHash       = Map::getMapHash();
      staticData.isMultiplayer = BW::isMultiplayer();
      staticData.isReplay      = BW::isInReplay();
    }
    //------------------------------------------------- UPDATE -------------------------------------------------
    void _update(GameState nextState)
    {
      // prerequisites
      if(nextState == Startup)
      {
        return;
      }

      // state transitions
      GameState lastState = gameState;
      gameState = nextState;

      // each root "if" is a conditioned function

      //--------------------------------------------------------------------------------------
      //  tracer frame
      if(gameState == InMatch)
      {
        Tracer::onMatchFrame();
      }

      //--------------------------------------------------------------------------------------
      // init bridge server
      if(lastState == Startup && nextState != Startup)
      {
        try
        {
          BridgeServer::initConnectionServer();
        }
        catch(GeneralException &e)
        {
          e.append("initializing bridge server");
          throw;
        }
      }

      //--------------------------------------------------------------------------------------
      // equivalent to onStartGame()
      // do what has to be done once each match start
      if(lastState != InMatch
        && nextState == InMatch)
      {
        // init BW's interface
        BW::onMatchInit();

        // enable user input as long as no agent is in charge
        //if(!BridgeServer::isAgentConnected()) TODO: uncomment
          Engine::enableFlag(Flags::UserInput);

        // reset frame count
        frameCount = 0;

        // load map data
        Map::load();

        // init bwUnitTableMirror
        {
          // mark all array as unused
          for(int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
          {
            bwUnitArrayMirror[i].isInChain = false;
            bwUnitArrayMirror[i].wasInChain = false;
            bwUnitArrayMirror[i].isDying = false;
            bwUnitArrayMirror[i].knownUnit = NULL;
          }
        }

        // init flag array
        bwUnitArrayMirrorFlags.setSize(BW::UNIT_ARRAY_MAX_LENGTH);
        bwUnitArrayMirrorFlags.setAllFlags(false);
      }

      //--------------------------------------------------------------------------------------
      // handle incoming connections
      if(BridgeServer::isBridgeInitialized()
        && !BridgeServer::isAgentConnected())
      {
        BridgeServer::acceptIncomingConnections();
        if(BridgeServer::isAgentConnected())
        {
          BW::printf("connected");
        }

        // fill the const part of static data, for the rest of the match

        // manage onStartGame RPC
        {
          //
          // invoke onStartGame()
          try
          {
            BridgeServer::invokeOnStartMatch(lastState != InMatch);
          }
          catch(GeneralException &exception)
          {
            BridgeServer::disconnect();
            BW::printf("disconnected. %s\n", exception.getMessage().c_str());
          }
        }
      }

      //--------------------------------------------------------------------------------------
      // agent onFrame
      if(gameState == InMatch
        && BridgeServer::isAgentConnected())
      {
        // fill buffers with recent world state data
        {
          BWAPI::StaticGameData &staticData = *BridgeServer::gameData;
          staticData.getLatency     = BW::getLatency();
          staticData.frameCount     = frameCount;
          staticData.mouseX         = BW::getMouseX();
          staticData.mouseY         = BW::getMouseY();
          staticData.screenX        = BW::getScreenX();
          staticData.screenY        = BW::getScreenY();
          for (int x=0;x<Map::getWidth();x++)
          {
            for (int y=0;y<Map::getHeight();y++)
            {
              staticData.isVisible[x][y] = Map::visible(x,y);
              staticData.isExplored[x][y] = Map::isExplored(x,y);
              staticData.hasCreep[x][y] = Map::hasCreep(x,y);
            }
          }
          staticData.isPaused      = BW::isPaused();
          staticData.unitCount=0;

          // traverse all game unit chains
          for(BW::Unit *bwUnit = *BW::BWDATA_UnitNodeChain_VisibleUnit_First; bwUnit; bwUnit = bwUnit->nextUnit)
          {
            int linear = BW::BWDATA_UnitNodeTable->getIndexByUnit(bwUnit); // get linear index
            Unit &mirror = bwUnitArrayMirror[linear];

            mirror.isInChain = true;
            bwUnitArrayMirrorFlags.setFlag(linear, true); // mark slot for processing

            // not interesting if this unit existed before
            if(mirror.wasInChain)
              continue;

            // unit did not exist before. init the reflection for this unit
            mirror.isDying = false;
            mirror.knownUnit = NULL;
          }

          // flag based search
          for(int w = -1;;)
          {
            // find all non-zero words
            w = bwUnitArrayMirrorFlags.findNextWord(w+1);
            if(w == -1)
              break;  // reached end

            // check each unit reflection that corresponds to a set flag
            int word = bwUnitArrayMirrorFlags.getWord(w);
            int baseIndex = w * Util::FlagArray::flagsInWord;
            for(int f = 0; f < Util::FlagArray::flagsInWord; f++)
            {
              if(!(word & (1 << f)))
                continue;
              // flag is set => this unit slot is used.

              // get corresponding BW::Unit and mirror
              int linear = baseIndex + f;
              BW::Unit &bwUnit = BW::BWDATA_UnitNodeTable->unit[linear];
              Unit &mirror = bwUnitArrayMirror[linear];

              // process the chain states
              bool wasInChain = mirror.wasInChain;  // we need these values for this iteration
              bool isInChain = mirror.isInChain;
              mirror.wasInChain = isInChain;        // init the values for next iteration
              mirror.isInChain = false;

              // remove flag when unit gets removed
              if(!isInChain)
              {
                // slot not used anymore
                bwUnitArrayMirrorFlags.setFlag(linear, false);
              }

              // if flag was set but slot is not used
              if(!isInChain && !wasInChain)
              {
                // optimization not working, debug me!
                __debugbreak();
                continue;
              }

              if(!isInChain && wasInChain)
              {
                // unit perished
                if(mirror.knownUnit)
                {
                  BridgeServer::removeKnownUnit(mirror.knownUnitIndex, UnitRemoveEventTypeIds::Died);
                  mirror.knownUnit = NULL;
                }
                continue;
              }

              // TODO: find out if the unit is dying. not urgent tho
              bool isDying = false;
              if(!isDying && mirror.isDying)
              {
                // the unit is not dying anymore? It's a new one
                // remove previous.
                if(mirror.knownUnit)
                {
                  BridgeServer::removeKnownUnit(mirror.knownUnitIndex, UnitRemoveEventTypeIds::Died);
                  mirror.knownUnit = NULL;
                }
              }

              // check the new knownability of this unit
              // TODO: extend knowability criteria
              bool isKnown = Map::visible(bwUnit.position.x/32, bwUnit.position.y/32);

              // if knownability changed
              if(!!mirror.knownUnit != isKnown)
              {
                if(isKnown)
                {
                  // unit becomes known

                  // reserve a KnownUnitEntry and store it's address so it gets filled
                  mirror.knownUnitIndex = BridgeServer::addKnownUnit(&mirror.knownUnit, UnitAddEventTypeIds::Created);
                }
                else
                {
                  // unit becomes not known

                  // release KnownUnit address
                  BridgeServer::removeKnownUnit(mirror.knownUnitIndex, UnitRemoveEventTypeIds::Died);
                  mirror.knownUnit = NULL;
                }
              }

              if(isKnown)
              {
                // transfer recent data about this particular BW unit
                KnownUnit &knownUnit = *mirror.knownUnit;

                // TODO: implement clearance limit
                knownUnit.position              = bwUnit.position;
                knownUnit.type                  = (UnitTypeId)bwUnit.unitID.id;

                /* TODO: find according BW::Unit members
                knownUnit.state.id                    = (int)&knownUnit;
                knownUnit.state.player                = bwUnit.getPlayer()->getID();
                knownUnit.state.hitPoints             = bwUnit.getHitPoints();
                knownUnit.state.shields               = bwUnit.getShields();
                knownUnit.state.energy                = bwUnit.getEnergy();
                knownUnit.state.resources             = bwUnit.getResources();
                knownUnit.state.killCount             = bwUnit.getKillCount();
                knownUnit.state.groundWeaponCooldown  = bwUnit.getGroundWeaponCooldown();
                knownUnit.state.airWeaponCooldown     = bwUnit.getAirWeaponCooldown();
                knownUnit.state.spellCooldown         = bwUnit.getSpellCooldown();
                knownUnit.state.defenseMatrixPoints   = bwUnit.getDefenseMatrixPoints();

                knownUnit.state.defenseMatrixTimer    = bwUnit.getDefenseMatrixTimer();
                knownUnit.state.ensnareTimer          = bwUnit.getEnsnareTimer();
                knownUnit.state.irradiateTimer        = bwUnit.getIrradiateTimer();
                knownUnit.state.lockdownTimer         = bwUnit.getLockdownTimer();
                knownUnit.state.maelstromTimer        = bwUnit.getMaelstromTimer();
                knownUnit.state.plagueTimer           = bwUnit.getPlagueTimer();
                knownUnit.state.removeTimer           = bwUnit.getRemoveTimer();
                knownUnit.state.stasisTimer           = bwUnit.getStasisTimer();
                knownUnit.state.stimTimer             = bwUnit.getStimTimer();

                knownUnit.state.isAccelerating        = bwUnit.isAccelerating();
                knownUnit.state.isBeingConstructed    = bwUnit.isBeingConstructed();
                knownUnit.state.isBeingGathered       = bwUnit.isBeingGathered();
                knownUnit.state.isBeingHealed         = bwUnit.isBeingHealed();
                knownUnit.state.isBlind               = bwUnit.isBlind();
                knownUnit.state.isBraking             = bwUnit.isBraking();
                knownUnit.state.isBurrowed            = bwUnit.isBurrowed();
                knownUnit.state.isCarryingGas         = bwUnit.isCarryingGas();
                knownUnit.state.isCarryingMinerals    = bwUnit.isCarryingMinerals();
                knownUnit.state.isCloaked             = bwUnit.isCloaked();
                knownUnit.state.isCompleted           = bwUnit.isCompleted();
                knownUnit.state.isConstructing        = bwUnit.isConstructing();
                knownUnit.state.isDefenseMatrixed     = bwUnit.isDefenseMatrixed();
                knownUnit.state.isEnsnared            = bwUnit.isEnsnared();
                knownUnit.state.isFollowing           = bwUnit.isFollowing();
                knownUnit.state.isGatheringGas        = bwUnit.isGatheringGas();
                knownUnit.state.isGatheringMinerals   = bwUnit.isGatheringMinerals();
                knownUnit.state.isHallucination       = bwUnit.isHallucination();
                knownUnit.state.isIdle                = bwUnit.isIdle();            // rusty: regarding SCV's not reporting that their idle.. I was calling worker->build() on both of them, and giving them the same location of the geyser
                knownUnit.state.isIrradiated          = bwUnit.isIrradiated();
                knownUnit.state.isLifted              = bwUnit.isLifted();
                knownUnit.state.isLoaded              = bwUnit.isLoaded();
                knownUnit.state.isLockedDown          = bwUnit.isLockedDown();
                knownUnit.state.isMaelstrommed        = bwUnit.isMaelstrommed();
                knownUnit.state.isMorphing            = bwUnit.isMorphing();
                knownUnit.state.isMoving              = bwUnit.isMoving();
                knownUnit.state.isParasited           = bwUnit.isParasited();
                knownUnit.state.isPatrolling          = bwUnit.isPatrolling();
                knownUnit.state.isPlagued             = bwUnit.isPlagued();
                knownUnit.state.isRepairing           = bwUnit.isRepairing();
                knownUnit.state.isResearching         = bwUnit.isResearching();
                knownUnit.state.isSelected            = bwUnit.isSelected();
                knownUnit.state.isSieged              = bwUnit.isSieged();
                knownUnit.state.isStartingAttack      = bwUnit.isStartingAttack();
                knownUnit.state.isStasised            = bwUnit.isStasised();
                knownUnit.state.isStimmed             = bwUnit.isStimmed();
                knownUnit.state.isTraining            = bwUnit.isTraining();
                knownUnit.state.isUnderStorm          = bwUnit.isUnderStorm();
                knownUnit.state.isUnpowered           = bwUnit.isUnpowered();
                knownUnit.state.isUpgrading           = bwUnit.isUpgrading();
                */
              } //if(isKnown)
            } //foreach flag
          } //foeach flagarray word
        } //fill buffers with recend world state data

        // update remote shared memry
        BridgeServer::updateRemoteSharedMemory();

        // call OnFrame RPC
        BridgeServer::invokeOnFrame();

        // process sendTexts
        {
          for each(Bridge::CommandEntry::SendText *entry in BridgeServer::sendTextEntries)
          {
            if(entry->printOnly)
              BW::printf(entry->text);
            else
              BW::sendText(entry->text);
          }
        }

        // process issued commands
        {
          struct LatencyCommandEntry
          {
            bool valid;
            int startFrame;
            BWAPI::UnitCommand command;
          };
          static std::deque<LatencyCommandEntry> latencyCommandQueue;

          // execute and recruit new commands into the latency simulation queue
          {
            saveSelected();

            for each(Bridge::CommandEntry::UnitOrder* order in BridgeServer::orderEntries)
            {
              BWAPI::UnitCommand& command = order->unitCommand;

              // TODO: validate command before executing it
              executeUnitCommand(command);

              LatencyCommandEntry entry;
              entry.valid = true;
              entry.startFrame = frameCount;
              entry.command = command;
              latencyCommandQueue.push_back(entry);
            }

            loadSelected();
          }

          // Remove commands that are too old
          {
            int ageLimit = BW::getLatency();

            for(std::deque<LatencyCommandEntry>::iterator it;
              latencyCommandQueue.begin() != latencyCommandQueue.end();
              )
            {
              it = latencyCommandQueue.begin();

              // check current entry
              LatencyCommandEntry &entry = (*it);
              int age = (frameCount - entry.startFrame);
              if(age < ageLimit)
                // from here on these entries are fresh enough
                break;
              // remove this outdated entry
              latencyCommandQueue.erase(it);
            }
          }

          // run simulation on the rest
          {
            std::deque<LatencyCommandEntry>::iterator it = latencyCommandQueue.begin();
            for(; it != latencyCommandQueue.end(); it++)
            {
              // check current entry
              LatencyCommandEntry &entry = (*it);

              // skip invalidated entries (removing them costs too much)
              if(!entry.valid)
                continue;

              // simulate this command
              KnownUnit &unitEntry = BridgeServer::gameData->units.at(entry.command.unitIndex);
              simulateUnitCommand(entry.command, unitEntry);
            }
          }
        }
      }

      //--------------------------------------------------------------------------------------
      // count frames
      if(gameState == InMatch)
      {
        frameCount++;
      }
    }
    //---------------------------------------------- UPDATE EXCEPTION HANDLER ----------------------------------
    void update(GameState nextState) throw()
    {
      bool wasConnected = BridgeServer::isAgentConnected();
      try
      {
        _update(nextState);
      }
      catch(GeneralException &exception)
      {
        BridgeServer::disconnect();
        if(BW::isInGame())
        {
          if(wasConnected)
            BW::printf("disconnected: %s\n", exception.getMessage().c_str());
          else
            BW::printf("exception: %s\n", exception.getMessage().c_str());
        }
      }
      catch(std::exception &exception)
      {
        BridgeServer::disconnect();
        if(BW::isInGame())
        {
          if(wasConnected)
            BW::printf("disconnected: %s\n", exception.what());
          else
            BW::printf("exception: %s\n", exception.what());
        }
      }
    }
    //---------------------------------------------- ON MENU FRAME ---------------------------------------------
    void onMenuFrame()
    {
      update(InMenu);
    }
    //------------------------------------------------- UPDATE -------------------------------------------------
    void onMatchStart();
    void onMatchFrame()
    {
      try
      {
        static bool onStartCalled = false;
        if(!onStartCalled)
        {
          onMatchStart();
          onStartCalled = true;
        }
      }
      catch (GeneralException& exception)
      {
        FILE*f = fopen("bwapi-error", "a+");
        fprintf(f, "Exception caught inside Game::update: %s", exception.getMessage().c_str());
        fclose(f);
      }

      update(InMatch);
    }
    //----------------------------------------------- PARSE TEXT -----------------------------------------------
    bool parseText(const char* text)
    {
      /* analyze the string */
      std::string message = text;
      std::vector<std::string> parsed = Util::Strings::splitString(message, " ");

      /* fix for bad referencing */
      while (parsed.size() < 5)
        parsed.push_back("");

      /* commands list */
      if (parsed[0] == "/leave")
      {
        BW::leaveGame();
        return true;
      }
      else if (parsed[0] == "/latency")
      {
        BW::printf("latency: %d", BW::getLatency());
        return true;
      }
      else if (parsed[0] == "/speed")
      {
        if (parsed[1] != "")
          setLocalSpeed(atoi(parsed[1].c_str()));
        else
          setLocalSpeed(-1);
        return true;
      }
      else if (parsed[0] == "/restart")
      {
        BW::restartGame();
        return true;
      }
      else if (parsed[0] == "/tracer")
      {
        if(!Tracer::command(parsed))
        {
          BW::printf("unknown tracer command");
        }
        return true;
      }
      return false;
    }
    //---------------------------------------------- ON MATCH END ----------------------------------------------
    void onMatchEnd()
    {
    }
    //----------------------------------------------------------------------------------------------------------
    void refresh()
    {
      /* Unusued
          TODO: find out what "Unused" mean? what does this function do? decide whether move to BW:: or remove
      #ifdef __GNUC__
        __asm__("call [BW::BWFXN_Refresh]");
      #else
        __asm call BW::BWFXN_Refresh
      #endif
      */
    }
    //--------------------------------------------- ON UNIT DEATH ---------------------------------------------
    void onUnitDeath(BW::Unit* unit)
    {
      // Unused hook
    }
    //--------------------------------------------------- GAME SPEED -------------------------------------------
    void  setLocalSpeed(int speed)
    {
      // Sets the frame rate of the client
      if (speed < 0)
      {
        // Reset the speed if it is negative
        BW::BWDATA_GameSpeedModifiers[0] = 501;
        BW::BWDATA_GameSpeedModifiers[1] = 333;
        BW::BWDATA_GameSpeedModifiers[2] = 249;
        BW::BWDATA_GameSpeedModifiers[3] = 201;
        BW::BWDATA_GameSpeedModifiers[4] = 168;
        BW::BWDATA_GameSpeedModifiers[5] = 144;
        BW::BWDATA_GameSpeedModifiers[6] = 126;
      }
      else
      {
        // Set all speeds if it is positive
        for (int i = 0; i < 7; i++)
          BW::BWDATA_GameSpeedModifiers[i] = speed;
      }
    }

    bool inScreen(int ctype, int x, int y)
    {
      int screen_x1 = x;
      int screen_y1 = y;
      if (ctype == 2)
      {
        screen_x1 -= *(BW::BWDATA_ScreenX);
        screen_y1 -= *(BW::BWDATA_ScreenY);
      }
      else if (ctype == 3)
      {
        screen_x1 += *(BW::BWDATA_MouseX);
        screen_y1 += *(BW::BWDATA_MouseY);
      }
      if (screen_x1 < 0   || screen_y1 < 0 ||
          screen_x1 >= 640 || screen_y1 >= 480) return false;
      return true;
    }

    bool inScreen(int ctype, int x1, int y1, int x2, int y2)
    {
      int screen_x1 = x1;
      int screen_y1 = y1;
      int screen_x2 = x2;
      int screen_y2 = y2;
      if (ctype == 2)
      {
        screen_x1 -= *(BW::BWDATA_ScreenX);
        screen_y1 -= *(BW::BWDATA_ScreenY);
        screen_x2 -= *(BW::BWDATA_ScreenX);
        screen_y2 -= *(BW::BWDATA_ScreenY);
      }
      else if (ctype == 3)
      {
        screen_x1 += *(BW::BWDATA_MouseX);
        screen_y1 += *(BW::BWDATA_MouseY);
        screen_x2 += *(BW::BWDATA_MouseX);
        screen_y2 += *(BW::BWDATA_MouseY);
      }
      if ((screen_x1 < 0 && screen_x2 < 0) ||
          (screen_y1 < 0 && screen_y2 < 0) ||
          (screen_x1 >= 640 && screen_x2 >= 640) ||
          (screen_y1 >= 480 && screen_y2 >= 480)) return false;
      return true;
    }

    bool inScreen(int ctype, int x1, int y1, int x2, int y2, int x3, int y3)
    {
      int screen_x1 = x1;
      int screen_y1 = y1;
      int screen_x2 = x2;
      int screen_y2 = y2;
      int screen_x3 = x3;
      int screen_y3 = y3;
      if (ctype == 2)
      {
        screen_x1 -= *(BW::BWDATA_ScreenX);
        screen_y1 -= *(BW::BWDATA_ScreenY);
        screen_x2 -= *(BW::BWDATA_ScreenX);
        screen_y2 -= *(BW::BWDATA_ScreenY);
        screen_x3 -= *(BW::BWDATA_ScreenX);
        screen_y3 -= *(BW::BWDATA_ScreenY);
      }
      else if (ctype == 3)
      {
        screen_x1 += *(BW::BWDATA_MouseX);
        screen_y1 += *(BW::BWDATA_MouseY);
        screen_x2 += *(BW::BWDATA_MouseX);
        screen_y2 += *(BW::BWDATA_MouseY);
        screen_x3 += *(BW::BWDATA_MouseX);
        screen_y3 += *(BW::BWDATA_MouseY);
      }
      if ((screen_x1 < 0 && screen_x2 < 0 && screen_x3 < 0) ||
          (screen_y1 < 0 && screen_y2 < 0 && screen_y3 < 0) ||
          (screen_x1 >= 640 && screen_x2 >= 640 && screen_x3 >= 640) ||
          (screen_y1 >= 480 && screen_y2 >= 480 && screen_y3 >= 480)) return false;
      return true;
    }

    bool gluMessageBox(char* message, int type)
    {
      bool rval = false;
      switch(type)
      {
      case MB_OKCANCEL:
        __asm
        {
          mov eax, message
          call BW::BWFXN_gluPOKCancel_MBox
          mov rval, al
        }
        break;
      default:  // MB_OK
        __asm
        {
          mov eax, message
            call BW::BWFXN_gluPOK_MBox
        }
        return false;
      }
      return rval;
    }

    bool gluEditBox(char* message, char* dest, size_t destsize, char* restricted)
    {
      bool rval;
      __asm
      {
        push restricted
        push destsize
        push dest
        push message
        call [BW::BWFXN_gluPEdit_MBox]
        mov  rval, al
      }
      return rval;
    }
    void onMessageIntercepted(const char* text)
    {
      if (parseText(text) || !flags[Flags::UserInput])
      {
        return;
      }
      else
      {
        if(BridgeServer::isAgentConnected())
        {
          //push text onto stack
          try
          {
            BridgeServer::pushSendText(text);
          }
          catch(...)
          {
            BridgeServer::disconnect();
          }
        }
        else
        {
          BW::sendText("%s", text);
          return;
        }
      }
      return;
    }
    //---------------------------------------- ON MATCH DRAW HIGH ----------------------------------------------
    void eachDrawShape(Util::MemoryFrame shapePacket)
    {
      int type = shapePacket.getAs<int>();
      if(type == Bridge::DrawShape::Text::_typeId)
      {
        Bridge::DrawShape::Text text;
        shapePacket.readTo(text);
        if(!shapePacket.size())
        {
          // packet too small
          throw GeneralException(__FUNCTION__ ": text shape packet too small for text");
        }
        if(shapePacket.endAs<char>()[-1] != 0)
        {
          // not null terminated
          throw GeneralException(__FUNCTION__ ": text shape packet text not null terminated");
        }
        BW::drawText(text.pos.x, text.pos.y, shapePacket.beginAs<char>());
      }
      if(type == Bridge::DrawShape::Line::_typeId)
      {
        Bridge::DrawShape::Line line;
        shapePacket.readTo(line);
        drawLine(line.from.x, line.from.y, line.to.x, line.to.y, line.color);
      }
      if(type == Bridge::DrawShape::Rectangle::_typeId)
      {
        Bridge::DrawShape::Rectangle rect;
        shapePacket.readTo(rect);
        drawRectangle(rect.pos.x, rect.pos.y, rect.size.x, rect.size.y, rect.color, rect.isSolid);
      }
      if(type == Bridge::DrawShape::Circle::_typeId)
      {
        Bridge::DrawShape::Circle circle;
        shapePacket.readTo(circle);
        drawCircle(circle.center.x, circle.center.y, circle.radius, circle.color, circle.isSolid);
      }
      if(type == Bridge::DrawShape::Dot::_typeId)
      {
        Bridge::DrawShape::Dot dot;
        shapePacket.readTo(dot);
        BW::drawDot(dot.pos.x, dot.pos.y, dot.color);
      }
    }
    void onMatchDrawHigh()
    {
      // hooks may not throw
      try
      {
        Tracer::onDraw();
        BridgeServer::enumAllDrawShapes(eachDrawShape);
      }
      catch(...)
      {
      }
    }
    //---------------------------------------- ORDER SIMULATION ------------------------------------------------
    void simulateUnitStop(const BWAPI::UnitCommand& command, BWAPI::UnitState& state)
    {
      // TODO: add alternation of state
    }

    void simulateUnknownUnitCommand(const BWAPI::UnitCommand& command, BWAPI::UnitState& state)
    {
      throw GeneralException("Unknown command " + Util::Strings::intToString(command.commandId));
    }

    void simulateUnitCommand(const BWAPI::UnitCommand& command, BWAPI::UnitState& state)
    {
      typedef void(*SIMULATOR)(const BWAPI::UnitCommand&, BWAPI::UnitState&);
      static Util::LookupTable<SIMULATOR> simulators;
      static bool isSimulatorsTableInitialized = false;

      // initialize lookup table
      if(!isSimulatorsTableInitialized)
      {
        simulators.setDefaultValue(&simulateUnknownUnitCommand);
        simulators.setValue(UnitCommandTypeIds::Stop, &simulateUnitStop);
        isSimulatorsTableInitialized = true;
      }

      // get command simulator function and call it
      SIMULATOR simulator = simulators.lookUp(command.commandId);
      simulator(command, state);
    }
    //---------------------------------------- ORDER EXECUTION -------------------------------------------------
    void executeSelectOrder(int bwUnitIndex)
    {
      BW::Unit* select;
      select = &BW::BWDATA_UnitNodeTable->unit[bwUnitIndex];
      BW::selectUnits(1, &select);
    }

    void executeUnitStop(int bwUnitIndex, const BWAPI::UnitCommand& command)
    {
      executeSelectOrder(bwUnitIndex);
      BW::issueCommand(BW::Command::Stop(0));
    }

    void executeUnknownUnitCommand(int bwUnitIndex, const BWAPI::UnitCommand& command)
    {
      throw GeneralException("Unknown command " + Util::Strings::intToString(command.commandId));
    }

    void executeUnitCommand(const BWAPI::UnitCommand& command)
    {
      typedef void(*SIMULATOR)(int unitId, const BWAPI::UnitCommand&);
      static Util::LookupTable<SIMULATOR> simulators;
      static bool isSimulatorsTableInitialized = false;

      // initialize lookup table
      if(!isSimulatorsTableInitialized)
      {
        simulators.setDefaultValue(&executeUnknownUnitCommand);
        simulators.setValue(UnitCommandTypeIds::Stop, &executeUnitStop);
        isSimulatorsTableInitialized = true;
      }

      // get command simulator function
      SIMULATOR simulator = simulators.lookUp(command.commandId);

      // find out the unitId.
      // TODO: add a knownunit mirror, lookup the bwunit index in the reflected knownunit entry
      // but for now just search the bwunit mirror for a reference to this knownunit
      int bwUnitIndex = -1;
      for(int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
      {
        if(!bwUnitArrayMirror[i].wasInChain)
          continue;
        if(bwUnitArrayMirror[i].knownUnitIndex == command.unitIndex)
        {
          bwUnitIndex = i;
          break;
        }
      }
      if(bwUnitIndex == -1)
        return; // the unit does not exist anymore;

      simulator(bwUnitIndex, command);
    }
    //---------------------------------------- -----------------------------------------------------------------
  }
};
