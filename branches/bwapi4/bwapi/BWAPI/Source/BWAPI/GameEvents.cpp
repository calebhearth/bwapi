#include "GameImpl.h"
#include <Util/Foreach.h>
#include <ctime>

#include "Detours.h"

#include <BW/Pathing.h>

#include <BWAPI/PlayerImpl.h>
#include <BWAPI/RegionImpl.h>

#include "../../../svnrev.h"
#include "../../../Debug.h"

#define TOURNAMENT_STR "BWAPI r" SVN_REV_STR " Tournament Mode Engaged!"

namespace BWAPI
{
  //--------------------------------------------- ON GAME START ----------------------------------------------
  void GameImpl::onGameStart()
  {
    // This function is called at the start of every match
    this->initializeData();

    // initialize the variables
    //this->frameCount      = 0;
    this->startTickCount  = GetTickCount();
    this->onStartCalled   = true;
    this->calledMatchEnd  = false;

    // load the map data
    map.load();
    this->savedMapHash = Map::getMapHash();

    // Obtain Broodwar Regions
    if ( *BW::BWDATA::SAIPathing )
    {
      u32 rgnCount = (*BW::BWDATA::SAIPathing)->regionCount;
      // Iterate regions and insert into region list
      for ( u32 i = 0; i < rgnCount; ++i )
        this->regionsList.insert(new BWAPI::RegionImpl(i));

      // Iterate regions again and update neighbor lists
      foreach ( BWAPI::RegionImpl *r, this->regionsList )
        r->UpdateRegionRelations();
    } // if SAI_Pathing

    // roughly identify which players can possibly participate in this game
    // iterate triggers for each player
    for ( int i = 0; i < PLAYABLE_PLAYER_COUNT; ++i )
    {
      // reset participation and resources
      if ( this->players[i] )
      {
        this->players[i]->setParticipating(false);
        this->players[i]->resetResources();
      }

      // First check if player owns a unit at start
      for ( int u = 0; u < UnitTypes::None; ++u )
      {
        if ( BW::BWDATA::AllScores->unitCounts.all[u][i] )
        {
          if ( this->players[i] )
            this->players[i]->setParticipating();
          break;
        }
      }

      // Then iterate each trigger
      // checking if a unit can be created or given to the player later in the game
      for ( BW::BlizzVectorEntry<BW::Triggers::Trigger> *t = BW::BWDATA::TriggerVectors[i].begin; (u32)t != ~(u32)&BW::BWDATA::TriggerVectors[i].end && (u32)t != (u32)&BW::BWDATA::TriggerVectors[i].begin; t = t->next )
      {
        // check if trigger conditions can be met
        if ( t->container.conditionsCanBeMet() )
        {
          // check participation of players
          for ( int p = 0; p < PLAYABLE_PLAYER_COUNT; ++p )
          {
            // Don't bother checking for participation if the player doesn't exist
            // or if the player is already participating (NOT observing)
            if ( !this->players[p] || !this->players[p]->isObserver() )
              continue;
            // Check if trigger actions allow gameplay and set participation if it does.
            if ( t->container.actionsAllowGameplay(i, p) )
              this->players[p]->setParticipating();
          }
        } // conds can be met
      } // trigvector iterator
    } // playercount iterator


    if ( *(BW::BWDATA::InReplay) ) /* set replay flags */
    {
      // Set every cheat flag to true
      for (int i = 0; i < Flag::Max; ++i)
        this->flags[i] = true;
    }
    else
    {
      /* Get the current player */
      BWAPI::PlayerImpl *thisPlayer = this->_getPlayer(_currentPlayerId());
      if ( !thisPlayer )
        return;

      this->BWAPIPlayer = thisPlayer;
      /* find the opponent player */
      for ( int i = 0; i < PLAYABLE_PLAYER_COUNT; ++i )
      {
        if ( (this->players[i]->getType() == PlayerTypes::Computer ||
              this->players[i]->getType() == PlayerTypes::Player   ||
              this->players[i]->getType() == PlayerTypes::EitherPreferComputer) &&
             !this->players[i]->isObserver() &&
             thisPlayer->isEnemy(this->players[i]) )
          this->enemyPlayer = this->players[i];
      }
    }
    
    // get pre-race info
    BYTE bRaceInfo[12] = { 0 };
    BYTE bOwnerInfo[12] = { 0 };

    HANDLE hFile = NULL;
    if ( SFileOpenFileEx(nullptr, "staredit\\scenario.chk", SFILE_FROM_MPQ, &hFile) && hFile )
    {
      DWORD dwFilesize = SFileGetFileSize(hFile, nullptr);
      void *pData = SMAlloc(dwFilesize);
      if ( pData )
      {
        DWORD dwRead = 0;
        if ( SFileReadFile(hFile, pData, dwFilesize, &dwRead, 0) && dwRead == dwFilesize )
        {
          struct _mapchunk
          {
            DWORD dwId;
            DWORD dwSize;
            BYTE  bData[1];
          } *mcptr;
          for ( mcptr = (_mapchunk*)pData; (DWORD)mcptr < (DWORD)pData + dwFilesize; mcptr = (_mapchunk*)&mcptr->bData[mcptr->dwSize] )
          {
            switch ( mcptr->dwId )
            {
            case MAKEFOURCC('S', 'I', 'D', 'E'):
              if ( mcptr->dwSize == 12 )
                memcpy(bRaceInfo, mcptr->bData, 12);
              break;
            case MAKEFOURCC('O', 'W', 'N', 'R'):
              if ( mcptr->dwSize == 12 )
                memcpy(bOwnerInfo, mcptr->bData, 12);
              break;
            }
          }
        }
        SMFree(pData);
      }
      SFileCloseFile(hFile);
    }

    // get the set of start locations
    BW::Position *StartLocs = BW::BWDATA::startPositions;
    // Iterate all players
    for ( int i = 0; i < PLAYABLE_PLAYER_COUNT; ++i )
    {
      // Skip Start Locations that don't exist
      if ( StartLocs[i] == Position(0, 0) )
        continue;

      // If the game is UMS and player is observer and race is not (UserSelect OR invalid player type), skip
      if ( this->getGameType() == GameTypes::Use_Map_Settings && 
           this->players[i]->isObserver() && 
           (bRaceInfo[i] != Races::Enum::Select ||
           (bOwnerInfo[i] != PlayerTypes::Computer &&
            bOwnerInfo[i] != PlayerTypes::Player   &&
            bOwnerInfo[i] != PlayerTypes::EitherPreferComputer &&
            bOwnerInfo[i] != PlayerTypes::EitherPreferHuman)) )
        continue;

      // add start location
      startLocations.push_back( TilePosition(StartLocs[i] - Position(64, 48)) );
    }

    // Get Player Objects
    for ( int i = 0; i < PLAYABLE_PLAYER_COUNT; ++i )
    {
      if ( this->players[i] && 
           BW::BWDATA::Players[i].nType != PlayerTypes::None &&
           BW::BWDATA::Players[i].nType <  PlayerTypes::Closed )
      {
        players[i]->setID(server.getPlayerID(players[i]));
        this->playerSet.insert(this->players[i]);
      }
    }

    if ( this->BWAPIPlayer )
    {
      foreach(Player* p, players)
      {
        if ( p->leftGame() || p->isDefeated() || p == BWAPIPlayer )
          continue;
        if ( BWAPIPlayer->isAlly(p) )
          _allies.insert(p);
        if ( BWAPIPlayer->isEnemy(p) )
          _enemies.insert(p);
        if ( p->isObserver() )
          _observers.insert(p);
      }
    }

    if ( this->players[11] )
      this->playerSet.insert(this->players[11]);

    // Get Force Objects, assign Force<->Player relations
    ForceImpl *pNeutralForce = new ForceImpl(std::string(""));
    pNeutralForce->players.insert(this->players[11]);
    this->players[11]->force = pNeutralForce;

    for ( int f = 1; f <= 4; ++f )
    {
      ForceImpl *newForce = new ForceImpl( std::string(BW::BWDATA::ForceNames[f-1].name) );
      this->forces.insert( newForce );
      for ( int p = 0; p < PLAYABLE_PLAYER_COUNT; ++p )
      {
        if ( this->players[p] && BW::BWDATA::Players[p].nTeam == f )
        {
          this->players[p]->force = newForce;
          if ( BW::BWDATA::Players[p].nType != PlayerTypes::None &&
               BW::BWDATA::Players[p].nType <  PlayerTypes::Closed )
            newForce->players.insert(this->players[p]);
        }
      }
    }

    // Assign neutral force to players that do not have one
    for ( int p = 0; p < PLAYABLE_PLAYER_COUNT; ++p )
    {
      if ( this->players[p] && !this->players[p]->force )
        this->players[p]->force = pNeutralForce;
    }

    // Get info for replay naming
    if ( !this->isReplay() )
    {
      if ( BWAPIPlayer )
      {
        rn_BWAPIName = BWAPIPlayer->getName();
        rn_BWAPIRace = BWAPIPlayer->getRace().getName().substr(0, 1);
      }
      rn_MapName   = mapName().substr(0, 16);
      rn_AlliesNames.clear();
      rn_AlliesRaces.clear();
      rn_EnemiesNames.clear();
      rn_EnemiesRaces.clear();
      foreach ( Player *p, this->_allies )
      {
        if ( p )
        {
          rn_AlliesNames += p->getName().substr(0, 6);
          rn_AlliesRaces += p->getRace().getName().substr(0, 1);
        }
      }
      foreach ( Player *p, this->_enemies )
      {
        if ( p )
        {
          rn_EnemiesNames += p->getName().substr(0, 6);
          rn_EnemiesRaces += p->getRace().getName().substr(0, 1);
        }
      }
    } // !isReplay
  }
  //--------------------------------------------------- ON SAVE ------------------------------------------------
  void GameImpl::onSaveGame(char *name)
  {
    // called when the game is being saved
    events.push_back(Event::SaveGame(name));
  }
  //---------------------------------------------- ON SEND TEXT ----------------------------------------------
  void GameImpl::onSendText(const char* text)
  {
    if ( !text )
      return;

    if ( !parseText(text) && isFlagEnabled(BWAPI::Flag::UserInput) )
    {
      if ( externalModuleConnected )
      {
        events.push_back(Event::SendText());
        events.back().setText(text);
      }
      else
        sendText("%s", text);
    }
  }
  //---------------------------------------------- ON RECV TEXT ----------------------------------------------
  void GameImpl::onReceiveText(int playerId, std::string text)
  {
    if ( !this->bTournamentMessageAppeared && hTournamentModule && text == TOURNAMENT_STR )
      this->bTournamentMessageAppeared = true;

    // Do onReceiveText
    int realId = stormIdToPlayerId(playerId);
    if ( realId != -1 && 
         (!this->BWAPIPlayer ||
          realId != this->BWAPIPlayer->getIndex() ) &&
         this->isFlagEnabled(BWAPI::Flag::UserInput) )
    {
      events.push_back(Event::ReceiveText(this->players[realId]));
      events.back().setText(text.c_str());
    }
  }
  int fixPathString(const char *in, char *out, size_t outLen)
  {
    unsigned int n = 0;
    const unsigned char *_in = (const unsigned char*)in;
    for ( unsigned int i = 0; _in[i] != 0 && n < outLen-1; ++i )
    {
      if ( !iscntrl(_in[i]) &&
           _in[i] != '?'    &&
           _in[i] != '*'    &&
           _in[i] != '<'    &&
           _in[i] != '|'    &&
           _in[i] != '"'    &&
           _in[i] != ':' )
      {
        if ( _in[i] == '/' )
          out[n] = '\\';
        else
          out[n] = _in[i];
        ++n;
      }
    }
    out[n] = 0;
    return n;
  }

  void ignore_invalid_parameter(const wchar_t*, const wchar_t*, const wchar_t*, unsigned int, uintptr_t)
  {}
  //---------------------------------------------- ON GAME END -----------------------------------------------
  void GameImpl::onGameEnd()
  {
    //this is called at the end of every match
    if ( !this->onStartCalled )
      return;

    if ( autoMenuSaveReplay != "" && !this->isReplay() )
    {
      // Set replay envvars
      SetEnvironmentVariable("BOTNAME",    rn_BWAPIName.c_str());
      SetEnvironmentVariable("BOTNAME6",   rn_BWAPIName.substr(0,6).c_str());
      SetEnvironmentVariable("BOTRACE",    rn_BWAPIRace.c_str());
      SetEnvironmentVariable("MAP",        rn_MapName.c_str());
      SetEnvironmentVariable("ALLYNAMES",  rn_AlliesNames.c_str());
      SetEnvironmentVariable("ALLYRACES",  rn_AlliesRaces.c_str());
      SetEnvironmentVariable("ENEMYNAMES", rn_EnemiesNames.c_str());
      SetEnvironmentVariable("ENEMYRACES", rn_EnemiesRaces.c_str());

      // Expand environment strings to szInterPath
      char szTmpPath[MAX_PATH] = { 0 };
      ExpandEnvironmentStrings(autoMenuSaveReplay.c_str(), szTmpPath, MAX_PATH);

      std::string pathStr(szTmpPath);

      // Double any %'s remaining in the string so that strftime executes correctly
      size_t tmp = std::string::npos;
      while ( tmp = pathStr.find_last_of('%', tmp-1), tmp != std::string::npos )
        pathStr.insert(tmp, "%");

      // Replace the placeholder $'s with %'s for the strftime call
      std::replace(pathStr.begin(), pathStr.end(), '$', '%');

      // Get time
      time_t tmpTime = time(nullptr);
      tm *timeInfo = localtime(&tmpTime);
      
      // Expand time strings
      _invalid_parameter_handler old = _set_invalid_parameter_handler(&ignore_invalid_parameter);
        strftime(szTmpPath, sizeof(szTmpPath), pathStr.c_str(), timeInfo);
      _set_invalid_parameter_handler(old);
      pathStr = szTmpPath;

      // Remove illegal characters
      pathStr.erase(std::remove_if(pathStr.begin(), pathStr.end(), [](char c){ return iscntrl(c) ||  c == '?' || c == '*' ||
                                                                                c == '<' ||  c == '|' || c == '>' || c == '"' ||
                                                                                c == ':';}), pathStr.end());
      // Create the directory tree
      size_t pos = 0;
      while ( pos = pathStr.find_first_of("/\\", pos+1), pos != std::string::npos )
        CreateDirectory(pathStr.substr(0,pos).c_str(), nullptr);

      // Copy to global desired replay name
      gDesiredReplayName = pathStr;
    }

    if ( !this->calledMatchEnd )
    {
      this->calledMatchEnd = true;
      events.push_back(Event::MatchFrame());
      events.push_back(Event::MatchEnd(false));
      server.update();
      this->inGame = false;
      events.push_back(Event::MenuFrame());
      server.update();
    }

    // player-specific game end
    for (int i = 0 ; i < PLAYER_COUNT; ++i)
      if ( this->players[i] )
        this->players[i]->onGameEnd();

    this->endTick       = GetTickCount();
    this->onStartCalled = false;

    this->initializeData();
  }
  //---------------------------------------------- SEND EVENTS TO CLIENT
  void GameImpl::SendClientEvent(BWAPI::AIModule *module, Event &e)
  {
    EventType::Enum et = e.getType();
    switch (et)
    {
    case EventType::MatchStart:
      module->onStart();
      break;
    case EventType::MatchEnd:
      module->onEnd(e.isWinner());
      break;
    case EventType::MatchFrame:
      module->onFrame();
      break;
    case EventType::MenuFrame:
      break;
    case EventType::SendText:
      module->onSendText(e.getText());
      break;
    case EventType::ReceiveText:
      module->onReceiveText(e.getPlayer(), e.getText());
      break;
    case EventType::PlayerLeft:
      module->onPlayerLeft(e.getPlayer());
      break;
    case EventType::NukeDetect:
      module->onNukeDetect(e.getPosition());
      break;
    case EventType::UnitDiscover:
      module->onUnitDiscover(e.getUnit());
      break;
    case EventType::UnitEvade:
      module->onUnitEvade(e.getUnit());
      break;
    case EventType::UnitCreate:
      module->onUnitCreate(e.getUnit());
      break;
    case EventType::UnitDestroy:
      module->onUnitDestroy(e.getUnit());
      break;
    case EventType::UnitMorph:
      module->onUnitMorph(e.getUnit());
      break;
    case EventType::UnitShow:
      module->onUnitShow(e.getUnit());
      break;
    case EventType::UnitHide:
      module->onUnitHide(e.getUnit());
      break;
    case EventType::UnitRenegade:
      module->onUnitRenegade(e.getUnit());
      break;
    case EventType::SaveGame:
      module->onSaveGame(e.getText());
      break;
    case EventType::UnitComplete:
      module->onUnitComplete(e.getUnit());
      break;
    default:
      break;
    }
  }
  //---------------------------------------------- PROCESS EVENTS
  void GameImpl::processEvents()
  {
    //This function translates events into AIModule callbacks
    if ( !client || server.isConnected() )
      return;
    foreach(Event e, events)
    {
      static DWORD dwLastEventTime = 0;

      // Reset event stopwatch
      if ( tournamentAI )
      {
        this->lastEventTime = 0;
        dwLastEventTime     = GetTickCount();
      }

      // Send event to the AI Client module
      SendClientEvent(client, e);

      // continue if the tournament is not loaded
      if ( !tournamentAI )
        continue;

      // Save the last event time
      this->lastEventTime = GetTickCount() - dwLastEventTime;

      // Send same event to the Tournament module for post-processing
      isTournamentCall = true;
      SendClientEvent(tournamentAI, e);
      isTournamentCall = false;
    } // foreach event
  }
}

