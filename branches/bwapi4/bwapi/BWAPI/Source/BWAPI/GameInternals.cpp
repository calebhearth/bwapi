#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used stuff from Windows headers

#include "GameImpl.h"
#include <vector>
#include <string>
#include <Util/Strings.h>

#include "../WMode.h"
#include "../Detours.h"
#include "../DLLMain.h"
#include "../Recording.h"
#include "../Resolution.h"

#include <BWAPI/BWtoBWAPI.h>
#include <BWAPI/UnitImpl.h>
#include <BWAPI/PlayerImpl.h>
#include <BWAPI/BulletImpl.h>
#include <BWAPI/RegionImpl.h>
#include "Command.h"

#include <BW/CBullet.h>
#include <BW/CUnit.h>
#include <BW/Dialog.h>

#include "../../../Debug.h"

/*
  This files holds all functions of the GameImpl class that are not part of the Game interface.
 */

namespace BWAPI
{
  GameImpl BroodwarImpl;

  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  GameImpl::GameImpl()
      : onStartCalled(false)
      , client(nullptr)
      , inGame(false)
      , endTick(0)
      , pathDebug(false)
      , showfps(false)
      , unitDebug(false)
      , grid(false)
      , calledMatchEnd(false)
      , lastAutoMapEntry(0)
      , tournamentAI(nullptr)
      , tournamentController(nullptr)
      , isTournamentCall(false)
      , lastEventTime(0)
      , data(server.data)
      , bTournamentMessageAppeared(false)
      , autoMenuEnemyCount(0)
      , autoMenuMinPlayerCount(0)
      , autoMenuMaxPlayerCount(0)
      , autoMenuWaitPlayerTime(0)
      , externalModuleConnected(false)
      , isHost(false)
  {
    BWAPI::BroodwarPtr = static_cast<Game*>(this);

    BWtoBWAPI_init();
    try
    {
      /* iterate through players and create PlayerImpl for each */
      for (int i = 0; i < PLAYER_COUNT; ++i)
        players[i] = new PlayerImpl((u8)i);

      /* iterate through units and create UnitImpl for each */
      for (int i = 0; i < UNIT_ARRAY_MAX_LENGTH; ++i)
        unitArray[i] = new UnitImpl(&BW::BWDATA::UnitNodeTable[i], (u16)i);

      /* iterate through bullets and create BulletImpl for each */
      for (int i = 0; i < BULLET_ARRAY_MAX_LENGTH; ++i)
        bulletArray[i] = new BulletImpl(&BW::BWDATA::BulletNodeTable[i], (u16)i);
    }
    catch (GeneralException& exception)
    {
      BWAPIError("Exception caught inside Game constructor: %s", exception.getMessage().c_str());
    }
    this->initializeData();
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  GameImpl::~GameImpl()
  {
    /* destroy all UnitImpl */
    for (int i = 0; i < UNIT_ARRAY_MAX_LENGTH; ++i)
    {
      if ( unitArray[i] != nullptr )
      {
        delete unitArray[i];
        unitArray[i] = nullptr;
      }
    }

    /* destroy all PlayerImpl */
    for (int i = 0; i < PLAYER_COUNT; ++i)
    {
      if ( players[i] != nullptr )
      {
        delete players[i];
        players[i] = nullptr;
      }
    }

    /* destroy all bullets */
    for(int i = 0; i < BULLET_ARRAY_MAX_LENGTH; ++i)
    {
      if ( bulletArray[i] != nullptr )
      {
        delete bulletArray[i];
        bulletArray[i] = nullptr;
      }
    }
  }
  //---------------------------------------- REFRESH SELECTION STATES ----------------------------------------
  void GameImpl::refreshSelectionStates()
  {
    for (int i = 0; i < UNIT_ARRAY_MAX_LENGTH; ++i)
      this->unitArray[i]->setSelected(false);

    selectedUnitSet.clear();
    for (int i = 0; i < *BW::BWDATA::ClientSelectionCount && i < 12; ++i)
    {
      if ( BW::BWDATA::ClientSelectionGroup[i] )
      {
        BWAPI::UnitImpl *u = UnitImpl::BWUnitToBWAPIUnit(BW::BWDATA::ClientSelectionGroup[i]);
        u->setSelected(true);
        selectedUnitSet.push_back(u);
      }
    }
  }
  void GameImpl::dropPlayers()
  {
    for ( int i = 0; i < PLAYABLE_PLAYER_COUNT; ++i )
    {
      if ( BW::BWDATA::playerStatusArray[i] & 0x10000 )
      {
        int iplr = this->stormIdToPlayerId(i);
        if ( iplr != -1 && iplr != (int)*BW::BWDATA::g_LocalHumanID )
        {
          this->droppedPlayers.push_back(this->players[iplr]);
          SNetDropPlayer(i, 0x40000006);
        }
      }
    }
  }
  //--------------------------------------------- IS BATTLE NET ----------------------------------------------
  bool GameImpl::_isBattleNet() const
  {
    return *BW::BWDATA::NetMode == 'BNET';
  }
  //-------------------------------------------- IS SINGLE PLAYER --------------------------------------------
  bool GameImpl::_isSinglePlayer() const
  {
    return *BW::BWDATA::NetMode == 0 || *BW::BWDATA::NetMode == -1;
  }
  //------------------------------------------------ IS IN GAME ----------------------------------------------
  bool GameImpl::_isInGame() const
  {
    return *BW::BWDATA::InGame != 0;
  }
  //----------------------------------------------- IN REPLAY ------------------------------------------------
  bool  GameImpl::_isReplay() const
  {
    return *BW::BWDATA::InReplay != 0;
  }
  //------------------------------------------------ MOUSE/KEY INPUT -----------------------------------------
  void GameImpl::pressKey(int key)
  {
    // Don't do anything if key is 0
    // used when auto-menu dialogs are not found, performance
    if ( !key )
      return;

    // Press and release the key
    PostMessage(SDrawGetFrameWindow(), WM_KEYDOWN, (WPARAM)key, NULL);
    PostMessage(SDrawGetFrameWindow(), WM_KEYUP,   (WPARAM)key, 0xC0000000);
  }
  void GameImpl::mouseDown(int x, int y)
  {
    // Press the left mouse button
    PostMessage(SDrawGetFrameWindow(), WM_LBUTTONDOWN, NULL, (LPARAM)MAKELONG(x,y));
  }
  void GameImpl::mouseUp(int x, int y)
  {
    // Release the left mouse button
    PostMessage(SDrawGetFrameWindow(), WM_LBUTTONUP, NULL, (LPARAM)MAKELONG(x,y));
  }
  //------------------------------------------- PLAYER ID CONVERT --------------------------------------------
  int GameImpl::stormIdToPlayerId(int dwStormId)
  {
    /* Translates a storm ID to a player Index */
    for (int i = 0; i < PLAYER_COUNT; ++i)
    {
      if ( BW::BWDATA::Players[i].dwStormId == dwStormId )
        return i;
    }
    return -1;
  }
  //----------------------------------------------- PARSE TEXT -----------------------------------------------
  bool GameImpl::parseText(const char* text)
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
      this->leaveGame();
    }
    else if (parsed[0] == "/speed")
    {
      if (parsed[1] != "")
        setLocalSpeed(atoi(parsed[1].c_str()));
      else
        setLocalSpeed();
      printf("Changed game speed");
    }
    else if (parsed[0] == "/fs")
    {
      if (parsed[1] != "")
        setFrameSkip(atoi(parsed[1].c_str()));
      else
        setFrameSkip();
      printf("Altered frame skip");
    }
    else if (parsed[0] == "/cheats")
    {
      sendText("power overwhelming");
      sendText("operation cwal");
      sendText("the gathering");
      sendText("medieval man");
      sendText("black sheep wall");
      sendText("food for thought");
      sendText("modify the phase variance");
      sendText("something for nothing");
      sendText("something for nothing");
      sendText("something for nothing");
      sendText("show me the money");
      sendText("show me the money");
      sendText("show me the money");
      sendText("show me the money");
      sendText("show me the money");
    }
    else if (parsed[0] == "/restart")
    {
      restartGame();
    }
    else if (parsed[0] == "/nogui")
    {
      setGUI(!data->hasGUI);
      printf("GUI: %s.", data->hasGUI ? "enabled" : "disabled");
    }
    else if (parsed[0] == "/wmode")
    {
      SetWMode(BW::BWDATA::GameScreenBuffer->width(), BW::BWDATA::GameScreenBuffer->height(), !wmode);
      printf("Toggled windowed mode.");
    }
    else if (parsed[0] == "/grid")
    {
      grid = !grid;
      printf("Matrix grid %s.", grid ? "enabled" : "disabled");
    }
    else if (parsed[0] == "/record")
    {
      if ( !StartVideoRecording(640, 480) )
        MessageBox(NULL, "Recording failed to start.", "Recording failed!", MB_OK | MB_ICONHAND);
    }
    else if (parsed[0] == "/stoprecord")
    {
      StopVideoRecording();
    }
    else if ( parsed[0] == "/fps" )
    {
      this->showfps = !this->showfps;
      printf("FPS display %s.", showfps ? "enabled" : "disabled");
    }
#ifdef _DEBUG
    else if (parsed[0] == "/latency")
    {
      printf("latency: %d", getLatency());
      printf("New latency: %u frames; %ums", getLatencyFrames(), getLatencyTime());
    }
// The following commands are knockoffs of Starcraft Beta's developer mode
    else if (parsed[0] == "/pathdebug")
    {
      pathDebug = !pathDebug;
      printf("pathdebug %s", pathDebug ? "ENABLED" : "DISABLED");
    }
    else if (parsed[0] == "/unitdebug")
    {
      unitDebug = !unitDebug;
      printf("unitdebug %s", unitDebug ? "ENABLED" : "DISABLED");
    }
// end knockoffs
    else if (parsed[0] == "/hud")
    {
      hideHUD = !hideHUD;
      printf("Now %s the HUD.", hideHUD ? "hiding" : "showing");
    }
    else if (parsed[0] == "/resize")
    {
      printf("Done");
      SetResolution(1280, 720);
    }
    else if (parsed[0] == "/test")
    {
      //SetResolution(640, 480);
      Unitset sel = this->getSelectedUnits();
      for ( auto i = sel.begin(); i != sel.end(); ++i )
        this->printf("Ground str: %d; Air str: %d", ((UnitImpl*)*i)->getOriginalRawData->groundStrength, ((UnitImpl*)*i)->getOriginalRawData->airStrength);
    }
#endif
    else
    {
      return false;
    }
    return true;
  }
  //------------------------------------------ copy Map To Shared Memory -------------------------------------
  void GameImpl::copyMapToSharedMemory()
  {
    map.copyToSharedMemory();
  }
  //------------------------------------------- GET PLAYER INTERNAL ------------------------------------------
  PlayerImpl *GameImpl::_getPlayer(int id)
  {
    if ( id < 0 || id >= PLAYER_COUNT )
      return players[PLAYER_COUNT - 1];
    return players[id];
  }
  int GameImpl::_currentPlayerId()
  {
    return *BW::BWDATA::g_LocalHumanID;
  }
  bool GameImpl::tournamentCheck(Tournament::ActionID type, void *parameter)
  {
    if ( this->tournamentController && !isTournamentCall )
    {
      isTournamentCall  = true;
      bool allow        = this->tournamentController->onAction(type, parameter);
      isTournamentCall  = false;
      return allow;
    }
    return true;
  }
  void GameImpl::initializeData()
  {
    // Destroy the AI Module client
    if ( this->client )
    {
      delete this->client;
      this->client = nullptr;
    }

    // Unload the AI Module library
    if ( hAIModule )
    {
      FreeLibrary(hAIModule);
      hAIModule = NULL;
    }
    
    this->startedClient = false;

    // Destroy the Tournament Module controller
    if ( this->tournamentController )
    {
      delete this->tournamentController;
      this->tournamentController = nullptr;
    }

    // Destroy the Tournament Module AI
    if ( this->tournamentAI )
    {
      delete this->tournamentAI;
      this->tournamentAI = nullptr;
    }
    
    // Destroy the Tournament Module Library
    if ( hTournamentModule )
    {
      FreeLibrary(hTournamentModule);
      hTournamentModule = NULL;
    }

    this->bTournamentMessageAppeared = false;

    // Delete forces
    for ( Forceset::iterator f = this->forces.begin(); f != this->forces.end(); ++f)
      delete ((ForceImpl*)(*f));
    this->forces.clear();

    // Remove player references
    this->BWAPIPlayer = nullptr;
    this->enemyPlayer = nullptr;

    // Set random seed
    srand(GetTickCount());

    // clear all sets
    this->aliveUnits.clear();
    this->dyingUnits.clear();
    this->discoverUnits.clear();
    this->accessibleUnits.clear();
    this->evadeUnits.clear();
    this->lastEvadedUnits.clear();
    this->selectedUnitSet.clear();
    this->startLocations.clear();
    this->playerSet.clear();
    this->minerals.clear();
    this->geysers.clear();
    this->neutralUnits.clear();
    this->bullets.clear();
    this->pylons.clear();
    this->staticMinerals.clear();
    this->staticGeysers.clear();
    this->staticNeutralUnits.clear();
    this->_allies.clear();
    this->_enemies.clear();
    this->_observers.clear();
    this->invalidIndices.clear();

    // Reset saved selection
    MemZero(this->savedUnitSelection);
    this->wantSelectionUpdate = false;

    // Disable all game flags
    MemZero(flags);

    // Clear the latency buffer
    for(unsigned int j = 0; j < this->commandBuffer.size(); ++j)
      for (unsigned int i = 0; i < this->commandBuffer[j].size(); ++i)
        delete this->commandBuffer[j][i];
    this->commandBuffer.clear();
    this->commandBuffer.reserve(16);

    // Clear the command optimization buffer
    for ( int i = 0; i < UnitCommandTypes::None; ++i )
      commandOptimizer[i].clear();

    // Delete all dead units
    for ( Unitset::iterator d = this->deadUnits.begin(); d != this->deadUnits.end(); ++d )
      delete (UnitImpl*)(*d);
    this->deadUnits.clear();

    // Delete all regions
    for ( Regionset::iterator r = this->regionsList.begin(); r != this->regionsList.end(); ++r )
      delete (RegionImpl*)(*r);
    this->regionsList.clear();

    // Reset game speeds and text size
    this->setLocalSpeed();
    this->setFrameSkip();
    this->setTextSize();
    this->setGUI();
    this->setCommandOptimizationLevel();

    // Reset all Unit objects in the unit array
    for (int i = 0; i < UNIT_ARRAY_MAX_LENGTH; ++i)
    {
      if ( !unitArray[i] )
        continue;
      unitArray[i]->userSelected      = false;
      unitArray[i]->isAlive           = false;
      unitArray[i]->wasAlive          = false;
      unitArray[i]->wasCompleted      = false;
      unitArray[i]->wasAccessible     = false;
      unitArray[i]->wasVisible        = false;
      unitArray[i]->staticInformation = false;
      unitArray[i]->nukeDetected      = false;
      unitArray[i]->lastType          = UnitTypes::Unknown;
      unitArray[i]->lastPlayer        = nullptr;
      unitArray[i]->lastCommandFrame  = 0;
      unitArray[i]->lastCommand       = UnitCommand();
      this->clientInfo.clear();

      unitArray[i]->setID(-1);
    }
    this->cheatFlags  = 0;
    this->bulletCount = 0;
    //this->frameCount  = -1;
    this->frameCount = 0;

    //reload auto menu data (in case the AI set the location of the next map/replay)
    this->loadAutoMenuData();

    //clear everything in the server
    this->server.clearAll();

    // clear messages so they are not stored until the next match
    this->sentMessages.clear();

    // Reset menu activation variables
    this->actRaceSel      = false;
    this->actStartedGame  = false;
    this->autoMapTryCount = 0;

    // Some other variables
    this->botAPMCounter_selects   = 0;
    this->botAPMCounter_noselects = 0;
    this->botAPM_selects = 0;
    this->botAPM_noselects = 0;
    this->fps = 0;
    this->averageFPS = 0;
    this->accumulatedFrames = 0;
  }
};
