#include "ScriptThread.h"
#include "ScriptEnum.h"
#include "BWScriptEmulator.h"
#include "Controller.h"

#include <BWAPI.h>

using namespace BWAPI;


DWORD AISavedCall;
int creep;

void aithread::execute()
{
  this->dwBytesRead = 0;
  if ( !pbAIScriptBinary )
  {
    Broodwar->printf("%cNo binary loaded!", 6);
    return this->killThread();
  }
  if ( this->locationCenter == BWAPI::Positions::None )
  {
    Broodwar->printf("%cInvalid location!", 6);
    return this->killThread();
  }
  for (;;)
  {
    if ( this->dwScriptOffset == 0 )
    {
      Broodwar->printf("%cScript offset is 0 !", 6);
      return this->killThread();
    }

    this->dwBytesRead = 0;
    BYTE bOpcode = read<BYTE>();
    switch ( bOpcode )
    {
    case AISCRIPT::GOTO:  // COMPLETE
      this->dwScriptOffset = this->read<WORD>();
      this->saveDebug(Text::Green, bOpcode, "p_%X", this->dwScriptOffset);
      continue;
    case AISCRIPT::NOTOWNS_JUMP:  // COMPLETE
      {
        UnitType uType    = UnitType(this->read<WORD>());
        WORD     jOffset  = this->read<WORD>();
        this->saveDebug(Text::Green, bOpcode, "%s p_%X", AISCRIPT::getUnitName(uType), jOffset);
        if ( self->allUnitCount(uType) == 0 )
          this->dwScriptOffset = jOffset;
        continue;
      }
    case AISCRIPT::WAIT:  // COMPLETE
      this->dwSleepTime = this->read<WORD>();
      this->saveDebug(Text::Green, bOpcode, "%3u", this->dwSleepTime);
      return;
    case AISCRIPT::START_TOWN:      // incomplete
    case AISCRIPT::START_AREATOWN:  // incomplete
      this->saveDebug(Text::Red, bOpcode);
      /* town *pTown = (bOpcode == AISCRIPT::START_TOWN) ? AI_StartTown(x,y,flags) : AI_StartAreaTown(x,y,flags);
      MainController->pTown = pTown;
      if ( !pTown )
      {
        this->killThread();
        return;
      }*/
      MainController->wFlags |= CONTROLLER_TOWN_STARTED;
      MainController->bIfDif = 1;
      // AI_StartShareTown();
      continue;
    case AISCRIPT::EXPAND:          // not started
      {
        BYTE bExpandMax   = this->read<BYTE>();
        WORD wExpandBlock = this->read<WORD>();
        this->saveDebug(Text::Red, bOpcode, "%3u p_%X", bExpandMax, wExpandBlock);
        // if ( GetExpansionCount() < bExpandMax )
        //   this->Expand(wExpandBlock, bExpandMax);
        continue;
      }
    case AISCRIPT::BUILD: // unfinished
      {
        BYTE bBuildCount    = this->read<BYTE>();
        UnitType wBuildType = UnitType(this->read<WORD>());
        BYTE bPriority      = this->read<BYTE>();

        this->saveDebug(Text::Red, bOpcode, "%3u %s %3u", bBuildCount, AISCRIPT::getUnitName(wBuildType), bPriority);
        //if ( bBuildCount <= 30 && wBuildType < UnitTypes::None )
        //  this->pTown->MacroManagerSet(MACRO_BUILD, bBuildCount, wBuildType, bPriority, false);
        continue;
      }
    case AISCRIPT::UPGRADE: // unfinished
      {
        BYTE bUpgLevel = this->read<BYTE>();
        UpgradeType wUpgType  = UpgradeType(this->read<WORD>());
        BYTE bPriority = this->read<BYTE>();
        this->saveDebug(Text::Red, bOpcode, "%3u %s %3u", bUpgLevel, wUpgType.c_str(), bPriority);
        //this->pTown->MacroManagerSet(MACRO_UPGRADE, bUpgLevel, wUpgType, bPriority, false);
        continue;
      }
    case AISCRIPT::TECH: // unfinished
      {
        TechType wTechType = TechType(this->read<WORD>());
        BYTE bPriority = this->read<BYTE>();
        this->saveDebug(Text::Red, bOpcode, "%s %3u", wTechType.c_str(), bPriority);
        //this->pTown->MacroManagerSet(MACRO_TECH, 1, wTechType, bPriority, false);
        continue;
      }
    case AISCRIPT::WAIT_BUILD: // incomplete
    case AISCRIPT::WAIT_BUILDSTART: // incomplete
      {
        BYTE bCount     = this->read<BYTE>();
        UnitType wType  = UnitType(this->read<WORD>());
        this->saveDebug(Text::Red, bOpcode, "%3u %s", bCount, AISCRIPT::getUnitName(wType) );
        if ( wType == self->getRace().getWorker() )
        {
          // TOWN STUFF
        }
/*
        if ( // AI_BuildSomething >= bCount )
        {
          retryBlock = false;
          continue;
        }
        */
        this->retry();
        return;
      }
    case AISCRIPT::ATTACK_CLEAR:  // unfinished
      this->saveDebug(Text::Yellow, bOpcode);
      memset(MainController->attackGroups, 0, sizeof(MainController->attackGroups));
      MainController->wAttackGroup = 0;
      MainController->dwAttackTime = 0;
      // AttackClear();
      continue;
    case AISCRIPT::ATTACK_ADD:  // COMPLETED
      {
        BYTE bCount     = this->read<BYTE>();
        UnitType wType  = UnitType(this->read<WORD>());
        this->saveDebug(Text::Green, bOpcode, "%3u %s", bCount, AISCRIPT::getUnitName(wType) );
        MainController->AttackAdd(bCount, wType);
        continue;
      }
    case AISCRIPT::ATTACK_PREPARE:  // not started
      this->saveDebug(Text::Red, bOpcode);
      // AI_AttackManager(thisLocation, 1, 0);
      continue;
    case AISCRIPT::ATTACK_DO:  // not started
      this->saveDebug(Text::Red, bOpcode);
      // if ( AI_AttackManager(thisLocation, 1, 0) )
      {
        retryBlock = false;
        continue;
      }
      // this->retry();
      // return;
    case AISCRIPT::WAIT_SECURE:  // COMPLETED
      this->saveDebug(Text::Green, bOpcode);
      if ( MainController->wFlags & CONTROLLER_SECURE_FINISHED )
      {
        retryBlock = false;
        continue;
      }
      this->retry();
      return;
    case AISCRIPT::CAPT_EXPAND:  // not started
      // Stands for capture expansion, run in default expansion block, set by what???
      this->saveDebug(Text::Red, bOpcode);
      continue;
    case AISCRIPT::BUILD_BUNKERS:  // not started
      this->saveDebug(Text::Red, bOpcode);
      // BuildBunkers();
      continue;
    case AISCRIPT::WAIT_BUNKERS:  // not started
      this->saveDebug(Text::Red, bOpcode);
      // if ( AIWaitBunkersFinished() )
      {
        retryBlock = false;
        continue;
      }
      //this->retry();
      //return;
    case AISCRIPT::DEFENSEBUILD_GG:  // COMPLETED
    case AISCRIPT::DEFENSEBUILD_AG:  // COMPLETED
    case AISCRIPT::DEFENSEBUILD_GA:  // COMPLETED
    case AISCRIPT::DEFENSEBUILD_AA:  // COMPLETED
      {
        BYTE     bCount = this->read<BYTE>();
        UnitType wType  = UnitType(this->read<WORD>());
        this->saveDebug(Text::Green, bOpcode, "%3u %s", bCount, AISCRIPT::getUnitName(wType) );
        MainController->DefenseBuild(bOpcode - AISCRIPT::DEFENSEBUILD_GG, bCount, wType);
        continue;
      }
    case AISCRIPT::DEFENSEUSE_GG:  // COMPLETED
    case AISCRIPT::DEFENSEUSE_AG:  // COMPLETED
    case AISCRIPT::DEFENSEUSE_GA:  // COMPLETED
    case AISCRIPT::DEFENSEUSE_AA:  // COMPLETED
      {
        BYTE     bCount = this->read<BYTE>();
        UnitType wType  = UnitType(this->read<WORD>());
        this->saveDebug(Text::Green, bOpcode, "%3u %s", bCount, AISCRIPT::getUnitName(wType) );
        MainController->DefenseUse(bOpcode - AISCRIPT::DEFENSEUSE_GG, bCount, wType);
        continue;
      }
    case AISCRIPT::DEFENSECLEAR_GG:  // COMPLETED
    case AISCRIPT::DEFENSECLEAR_AG:  // COMPLETED
    case AISCRIPT::DEFENSECLEAR_GA:  // COMPLETED
    case AISCRIPT::DEFENSECLEAR_AA:  // COMPLETED
      this->saveDebug(Text::Green, bOpcode);
      MainController->DefenseClear(bOpcode - AISCRIPT::DEFENSECLEAR_GG);
      continue;
    case AISCRIPT::SEND_SUICIDE:  // not started
      {
        BYTE bSuicideType = this->read<BYTE>();
        this->saveDebug(Text::Red, bOpcode, "%3u", bSuicideType);
        memset(MainController->attackGroups, 0, sizeof(MainController->attackGroups));
        MainController->wAttackGroup = 0;
        // MainController->dwAttackTime = 0;
        // AttackClear();
        // SendSuicide(bSuicideType);
        MainController->dwAttackTime = bw->elapsedTime() - 175;
        continue;
      }
    case AISCRIPT::PLAYER_ENEMY:  // COMPLETED
    case AISCRIPT::PLAYER_ALLY:   // COMPLETED
      {
        this->saveDebug(Text::Green, bOpcode);
        // @TODO: BWAPI: Unitset::getPlayers for retrieving set of players owning the units, not important
        /* can become
        unitsInRect( bw->getUnitsInRectangle(locationBounds.left, locationBounds.top, locationBounds.right, locationBounds.bottom, 
                                             GetPlayer != self) ).getPlayers().setAlliance(bOpcode == AISCRIPT::PLAYER_ALLY);
                                             */
        Unitset unitsInRect( bw->getUnitsInRectangle(locationBounds.left, locationBounds.top, locationBounds.right, locationBounds.bottom, GetPlayer != self) );
        for ( Unitset::iterator u = unitsInRect.begin(); u != unitsInRect.end(); ++u )
          bw->setAlliance(u->getPlayer(), bOpcode == AISCRIPT::PLAYER_ALLY);
        continue;
      }
    case AISCRIPT::DEFAULT_MIN:  // COMPLETED
      MainController->bDefaultMin = this->read<BYTE>();
      this->saveDebug(Text::Green, bOpcode, "%3u", MainController->bDefaultMin);
      continue;
    case AISCRIPT::DEFAULTBUILD_OFF:  // COMPLETE
      this->saveDebug(Text::Green, bOpcode);
      MainController->wFlags |= CONTROLLER_DEFAULT_BUILD_OFF;
      continue;
    case AISCRIPT::STOP:    // COMPLETE
      this->saveDebug(Text::Green, bOpcode);
      this->killThread();
      return;
    case AISCRIPT::SWITCH_RESCUE:  // COMPLETED (can't actually emulate)
      this->saveDebug(Text::Yellow, bOpcode);
      bw->enemies().setAlliance(true); // closest thing to becoming neutral+rescuable
      continue;
    case AISCRIPT::MOVE_DT: // WORKAROUND
      {
        this->saveDebug(Text::Yellow, bOpcode);
        Unitset myUnits(self->getUnits());
      
        myUnits.erase_if( !((GetType == UnitTypes::Protoss_Dark_Templar || GetType == UnitTypes::Hero_Dark_Templar)
                            && Exists && IsCompleted) );
        myUnits.move(this->locationCenter);
        // Note: Needs special reassignment (CAIControl, AICaptain)
        continue;
      }
    case AISCRIPT::DEBUG:   // COMPLETE
      {
        WORD wJump = this->read<WORD>();
        Broodwar->sendText("%s", &pbAIScriptBinary[this->dwScriptOffset]);
        this->saveDebug(Text::Green, bOpcode, "p_%X %s", wJump, &pbAIScriptBinary[this->dwScriptOffset]);
        this->dwScriptOffset = wJump;
        continue;
      }
    case AISCRIPT::FATAL_ERROR: // COMPLETED
      Broodwar->sendText("Illegal AI script executed.");
      this->saveDebug(Text::Green, bOpcode);
      continue;
    case AISCRIPT::ENTER_BUNKER: // completed for the most part
      {
        this->saveDebug(Text::Yellow, bOpcode);
        // ignore if there are no bunkers
        if ( !self->completedUnitCount(UnitTypes::Terran_Bunker) )
          continue;

        Unitset unitsForBunker( bw->getUnitsInRectangle(this->locationBounds.left, locationBounds.top, locationBounds.right, locationBounds.bottom, 
                                                                GetPlayer == self && IsCompleted && GetRace == Races::Terran && IsOrganic ) );
      
        // Iterate the units that are to enter the bunkers
        for ( auto u = unitsForBunker.begin(); u != unitsForBunker.end(); ++u )
        {
          // Find a bunker closest to the current unit that has space available
          Unit *pBunker = u->getClosestUnit(GetType == UnitTypes::Terran_Bunker &&
                                            GetPlayer == self && 
                                            SpaceRemaining >= u->getType().spaceRequired() );
          if ( pBunker )  // If a bunker was found
            pBunker->load(*u);
        }
      }
      continue;
    case AISCRIPT::VALUE_AREA: // not started
      this->saveDebug(Text::Red, bOpcode);
      // ValueArea(this->locationCenter);
      continue;
    case AISCRIPT::TRANSPORTS_OFF: // COMPLETE
      this->saveDebug(Text::Green, bOpcode);
      MainController->wFlags |= CONTROLLER_TRANSPORTS_OFF;
      continue;
    case AISCRIPT::CHECK_TRANSPORTS: // COMPLETE
      this->saveDebug(Text::Green, bOpcode);
      MainController->wFlags &= ~CONTROLLER_TRANSPORTS_OFF;
      continue;
    case AISCRIPT::NUKE_RATE: // COMPLETE
      MainController->bNukeRate = this->read<BYTE>();
      this->saveDebug(Text::Green, bOpcode, "%3u", MainController->bNukeRate);
      continue;
    case AISCRIPT::MAX_FORCE: // COMPLETED
      MainController->wMaxForce = UnitType(this->read<WORD>());
      this->saveDebug(Text::Green, bOpcode, "%s", AISCRIPT::getUnitName(MainController->wMaxForce) );
      continue;
    case AISCRIPT::CLEAR_COMBATDATA: // not started
      this->saveDebug(Text::Red, bOpcode);
      // ClearCombatData(this->locationBounds);
      continue;
    case AISCRIPT::RANDOM_JUMP: // COMPLETED
      {
        BYTE bRandChance = this->read<BYTE>();
        WORD wJmpOffset = this->read<WORD>();
        
        this->saveDebug(Text::Green, bOpcode, "%3u p_%X", bRandChance, wJmpOffset);
        if ( LOBYTE(rand()) <= bRandChance )
          this->dwScriptOffset = wJmpOffset;
        continue;
      }
    case AISCRIPT::TIME_JUMP: // COMPLETED
      {
        BYTE bTimePast  = 60 * this->read<BYTE>();
        WORD wJmpOffset = this->read<WORD>();
        this->saveDebug(Text::Green, bOpcode, "%3u p_%X", bTimePast, wJmpOffset);
        if ( bw->elapsedTime() >= bTimePast )
          this->dwScriptOffset = wJmpOffset;
        continue;
      }
    case AISCRIPT::FARMS_NOTIMING: // COMPLETED
      this->saveDebug(Text::Green, bOpcode);
      MainController->wFlags |= CONTROLLER_FARMS_TIMING_OFF;
      continue;
    case AISCRIPT::FARMS_TIMING: // COMPLETED
      this->saveDebug(Text::Green, bOpcode);
      MainController->wFlags &= ~CONTROLLER_FARMS_TIMING_OFF;
      continue;
    case AISCRIPT::BUILD_TURRETS: // not started
      this->saveDebug(Text::Red, bOpcode);
      // BuildTurrets();
      continue;
    case AISCRIPT::WAIT_TURRETS: // not started
      this->saveDebug(Text::Red, bOpcode);
      // if ( AIWaitTurretsFinished() )
      {
        retryBlock = false;
        continue;
      }
      //this->retry();
      //return;
    case AISCRIPT::DEFAULT_BUILD: // COMPLETED
      this->saveDebug(Text::Green, bOpcode);
      MainController->wFlags &= ~CONTROLLER_DEFAULT_BUILD_OFF;
      continue;
    case AISCRIPT::HARASS_FACTOR: // not started
      {
        WORD wUnk = this->read<WORD>();
        this->saveDebug(Text::Red, bOpcode, "%3u", wUnk);
        // HarassFactor(wUnk);
        continue;
      }
    case AISCRIPT::START_CAMPAIGN: // COMPLETE
      this->saveDebug(Text::Green, bOpcode);
      MainController->wFlags |= CONTROLLER_IS_CAMPAIGN;
      continue;
    case AISCRIPT::RACE_JUMP: // WORKAROUND
      {
        WORD t_jmp = this->read<WORD>();
        WORD z_jmp = this->read<WORD>();
        WORD p_jmp = this->read<WORD>();
        
        this->saveDebug(Text::Yellow, bOpcode, "p_%X p_%X p_%X", t_jmp, z_jmp, p_jmp);

        // should be AI_getClosestEnemyRace; gets closest enemy unit to region
        // @TODO: BWAPI: Filters: IsEnemy, IsAlly, IsNeutral?
        Race race = Races::None;
        if ( bw->enemy() )
          race = bw->enemy()->getRace();

        if ( race == Races::Protoss )
          this->dwScriptOffset = p_jmp;
        else if ( race == Races::Zerg )
          this->dwScriptOffset = z_jmp;
        else
          this->dwScriptOffset = t_jmp;
        continue;
      }
    case AISCRIPT::REGION_SIZE: // not started
      {
        BYTE bSize  = this->read<BYTE>();
        WORD wBlock = this->read<WORD>();
        this->saveDebug(Text::Red, bOpcode, "%3u p_%X", bSize, wBlock);
        // @TODO: Region::getTileCount
        // if ( this->pTown->RegionSize() < bSize )
        //   this->dwScriptOffset = wBlock;
        continue;
      }
    case AISCRIPT::GET_OLDPEONS: // not started
      {
        BYTE bCount = this->read<BYTE>();
        this->saveDebug(Text::Red, bOpcode, "%3u", bCount);
        // this->pTown->GetOldPeons(bCount);
        continue;
      }
    case AISCRIPT::GROUNDMAP_JUMP: // COMPLETED
      {
        WORD wJmpOffset = this->read<WORD>();
        this->saveDebug(Text::Green, bOpcode, "p_%X", wJmpOffset);
        
        Unitset enemyUnits( bw->enemies().getUnits() );
        enemyUnits.erase_if( !(Exists && IsBuilding && !IsLifted) );
        for ( auto u = enemyUnits.begin(); u != enemyUnits.end(); ++u )
        {
          if ( u->hasPath(this->locationCenter) )
          {
            this->dwScriptOffset = wJmpOffset;
            break;
          }
        }
        continue;
      }
    case AISCRIPT::PLACE_GUARD: // not started
      {
        UnitType wType = UnitType(this->read<WORD>());
        BYTE bIndex    = this->read<BYTE>();
        this->saveDebug(Text::Red, bOpcode, "%s %3u", AISCRIPT::getUnitName(wType), bIndex);
        // this->pTown->PlaceGuard(bIndex, wType);
        continue;
      }
    case AISCRIPT::GUARD_RESOURCES: // not started
      {
        UnitType wType = UnitType(this->read<WORD>());
        this->saveDebug(Text::Red, bOpcode, "%s", AISCRIPT::getUnitName(wType));
        // GuardResources(wType);
        continue;
      }
    case AISCRIPT::CALL:    // COMPLETE
      AISavedCall           = this->dwScriptOffset;
      this->dwScriptOffset  = this->read<WORD>();
      this->saveDebug(Text::Green, bOpcode, "p_%X", this->dwScriptOffset);
      continue;
    case AISCRIPT::RETURN:    // COMPLETE
      this->saveDebug(Text::Green, bOpcode);
      this->dwScriptOffset = AISavedCall;
      AISavedCall = 0;
      continue;
    case AISCRIPT::EVAL_HARASS: // not started
      {
        WORD wBlock = this->read<WORD>();
        this->saveDebug(Text::Red, bOpcode, "p_%X", wBlock);
        //if ( !EvalHarass(this->locationCenter) )
        //  this->dwScriptOffset = wBlock;
        continue;
      }
    case AISCRIPT::CREEP:   // COMPLETE
      {
        BYTE bCreepFlag = this->read<BYTE>();
        this->saveDebug(Text::Green, bOpcode, "%3u", bCreepFlag);
        if ( bCreepFlag == 4 )
          MainController->wFlags |= CONTROLLER_SPREAD_CREEP;
        else
          MainController->wFlags &= ~CONTROLLER_SPREAD_CREEP;
        continue;
      }
    case AISCRIPT::PANIC: // COMPLETED
      MainController->wPanicBlock = this->read<WORD>();
      this->saveDebug(Text::Green, bOpcode, "p_%X", MainController->wPanicBlock);
      continue;
    case AISCRIPT::PLAYER_NEED: // incomplete
      {
        BYTE bBuildCount    = this->read<BYTE>();
        UnitType wBuildType = UnitType(this->read<WORD>());
        this->saveDebug(Text::Red, bOpcode, "%3u %s", bBuildCount, AISCRIPT::getUnitName(wBuildType));
        //if ( bBuildCount <= 30 && wBuildType < UnitTypes::None )
        //  this->pTown->MacroManagerSet(MACRO_BUILD, bBuildCount, wBuildType, 80, true);
        continue;
      }
    case AISCRIPT::WAIT_UPGRADES: // COMPLETED
      this->saveDebug(Text::Green, bOpcode);
      if ( MainController->wFlags & CONTROLLER_UPGRADES_FINISHED )
      {
        retryBlock = false;
        continue;
      }
      this->retry();
      return;
    case AISCRIPT::MULTIRUN:    // COMPLETE
      {
        WORD wMultirunBlock = this->read<WORD>();
        this->saveDebug(Text::Green, bOpcode, "p_%X", wMultirunBlock);
        AICreateThread(wMultirunBlock, this->locationCenter, this->pTown);
        continue;
      }
    case AISCRIPT::RUSH:  // completed ?
      {
        BYTE bRushCategory = this->read<BYTE>();
        WORD wRushBlock    = this->read<WORD>();

        this->saveDebug(Text::Yellow, bOpcode, "%3u p_%X", bRushCategory, wRushBlock);
        bool isRushed = false;
        
        // Get closest enemy owner
        Unit *pClosest = Broodwar->getClosestUnit(this->locationCenter, IsEnemy && (!IsBuilding || IsLifted) );
        Player *closestEnemy = pClosest ? pClosest->getPlayer() : nullptr;

        /*
        int bestDistance = 99999999;
        Player *closestEnemy = NULL;
        Unitset enemyUnits( Broodwar->enemies().getUnits() );
        enemyUnits.erase_if( !(!IsBuilding || IsLifted) );
        for ( auto u = enemyUnits.begin(); u != enemyUnits.end(); ++u )
        {
          int thisDistance = u->getPosition().getApproxDistance(this->locationCenter);
          if ( thisDistance <= 320 && closestEnemy == u->getPlayer() )
            break;

          if ( thisDistance <= bestDistance )
          {
            bestDistance = thisDistance;
            closestEnemy = u->getPlayer();
          }
        } // iterate units
        */

        if ( !closestEnemy )
          continue;

        switch ( bRushCategory )
        {
        case 0:
          if ( closestEnemy->allUnitCount(UnitTypes::Terran_Barracks)    ||
               closestEnemy->allUnitCount(UnitTypes::Protoss_Gateway)    ||
               closestEnemy->allUnitCount(UnitTypes::Zerg_Spawning_Pool) )
            isRushed = true;
          break;
        case 1:
          if ( GetTerranRushScore(closestEnemy)     > 16 ||
               GetZergGroundRushScore(closestEnemy) > 10 ||
               closestEnemy->completedUnitCount(UnitTypes::Protoss_Zealot) > 6 )
            isRushed = true;
          break;
        case 2:
          if ( GetTerranRushScore(closestEnemy)  > 24 ||
               GetZergAirRushScore(closestEnemy) > 10 )
            isRushed = true;
          break;
        case 3:
          if ( GetTerranRushScore(closestEnemy)     > 5 ||
               closestEnemy->completedUnitCount(UnitTypes::Zerg_Hydralisk_Den) ||
               GetZergGroundRushScore(closestEnemy) > 2 ||
               closestEnemy->completedUnitCount(UnitTypes::Protoss_Zealot) > 1 )
            isRushed = true;
          break;
        case 4:
          if ( GetTerranRushScore(closestEnemy)     > 16 ||
               GetZergGroundRushScore(closestEnemy) > 10 ||
               closestEnemy->completedUnitCount(UnitTypes::Protoss_Zealot) > 8 )
            isRushed = true;
          break;
        case 5:
          if ( GetTerranRushScore(closestEnemy)     > 6 ||
               GetZergGroundRushScore(closestEnemy) > 6 ||
               closestEnemy->completedUnitCount(UnitTypes::Protoss_Zealot) > 3 )
            isRushed = true;
          break;
        case 6:
          if ( GetTerranRushScore(closestEnemy) > 12 ||
               closestEnemy->completedUnitCount(UnitTypes::Zerg_Sunken_Colony) > 1 ||
               closestEnemy->completedUnitCount(UnitTypes::Protoss_Dragoon)    > 1 )
            isRushed = true;
          break;
        case 7:
          if ( closestEnemy->completedUnitCount(UnitTypes::Terran_Siege_Tank_Tank_Mode) > 0 ||
               closestEnemy->completedUnitCount(UnitTypes::Zerg_Queen) ||
               closestEnemy->completedUnitCount(UnitTypes::Protoss_Zealot) > 6 )
            isRushed = true;
          break;
        case 8:
          if ( GetTerranRushScore(closestEnemy)     > 5 ||
               GetZergGroundRushScore(closestEnemy) > 2 ||
               closestEnemy->completedUnitCount(UnitTypes::Protoss_Zealot) > 1 )
            isRushed = true;
          break;
        case 9:
          if ( GetTerranRushScore(closestEnemy)     > 9 ||
               GetZergGroundRushScore(closestEnemy) > 4 ||
               closestEnemy->completedUnitCount(UnitTypes::Protoss_Zealot) > 5 )
            isRushed = true;
          break;
        case 10:
          if ( GetTerranRushScore(closestEnemy)     > 4 ||
               GetZergGroundRushScore(closestEnemy) > 4 ||
               closestEnemy->completedUnitCount(UnitTypes::Protoss_Zealot) > 2 )
            isRushed = true;
          break;
        case 11:
          if ( GetTerranRushScore(closestEnemy)     > 10 ||
               GetZergGroundRushScore(closestEnemy) > 10 ||
               closestEnemy->completedUnitCount(UnitTypes::Protoss_Zealot) > 5 )
            isRushed = true;
          break;
        case 12:
          if ( GetTerranRushScore(closestEnemy)     > 16  ||
               GetZergAirRushScore(closestEnemy)    > 5   ||
               GetProtossAirRushScore(closestEnemy) > 2 )
            isRushed = true;
          break;
        case 13:
          if ( GetTerranRushScore(closestEnemy)     > 24 ||
               GetZergAirRushScore(closestEnemy)    > 10 ||
               GetProtossAirRushScore(closestEnemy) > 7 )
            isRushed = true;
          break;
        default:
          isRushed = false;
          break;
        }
        if ( isRushed )
          this->dwScriptOffset = wRushBlock;
        continue;
      }
    case AISCRIPT::SCOUT_WITH:  // COMPLETE (this function does nothing in Starcraft, may become necessary in emulator)
      {
        UnitType wScout = UnitType(this->read<WORD>());
        this->saveDebug(Text::Green, bOpcode, "%s", AISCRIPT::getUnitName(wScout));
        continue;
      }
    case AISCRIPT::DEFINE_MAX: // COMPLETED
      {
        BYTE     bCount = this->read<BYTE>();
        UnitType wType  = UnitType(this->read<WORD>());
        this->saveDebug(Text::Green, bOpcode, "%3u %s", bCount, AISCRIPT::getUnitName(wType));
        if ( wType < UnitTypes::None )
          MainController->bDefineMax[wType] = bCount;
        continue;
      }
    case AISCRIPT::DO_MORPH:  // COMPLETED
    case AISCRIPT::WAIT_FORCE:  // COMPLETED
    case AISCRIPT::TRAIN: // COMPLETED
      {
        BYTE     bCount = this->read<BYTE>();
        UnitType wType  = UnitType(this->read<WORD>());

        this->saveDebug(Text::Green, bOpcode, "%3u %s", bCount, AISCRIPT::getUnitName(wType));
        if ( GetStandardUnitCount(wType, bOpcode != AISCRIPT::TRAIN) < bCount )
        {
          MainController->wWaitForType = (int)wType + 1;
          if ( bOpcode != AISCRIPT::DO_MORPH )
          {
            this->retry();
            return;
          }
        }
        retryBlock = false;
        continue;
      }
    case AISCRIPT::TARGET_EXPANSION: // COMPLETE
      this->saveDebug(Text::Green, bOpcode);
      MainController->wFlags |= CONTROLLER_TARGET_EXPANSION;
      continue;
    case AISCRIPT::WAIT_TRAIN: // COMPLETE
      {
        BYTE     bCount = this->read<BYTE>();
        UnitType wType  = UnitType(this->read<WORD>());

        this->saveDebug(Text::Green, bOpcode, "%3u %s", bCount, AISCRIPT::getUnitName(wType));
        if ( GetStandardUnitCount(wType) < bCount )
        {
          this->retry();
          return;
        }
        retryBlock = false;
        continue;
      }
    case AISCRIPT::SET_ATTACKS: // COMPLETE
      MainController->bAttacks = this->read<BYTE>();
      this->saveDebug(Text::Green, bOpcode, "%3u", MainController->bAttacks);
      continue;
    case AISCRIPT::SET_GENCMD: // COMPLETED
      this->saveDebug(Text::Green, bOpcode);
      MainController->genCmdTarget = this->locationBounds;
      continue;
    case AISCRIPT::MAKE_PATROL: // COMPLETED
      this->saveDebug(Text::Green, bOpcode);
      bw->getUnitsInRectangle(this->locationBounds.left, this->locationBounds.top, this->locationBounds.right, this->locationBounds.bottom, 
                              GetPlayer == self && IsCompleted).patrol( Position((MainController->genCmdTarget.left + MainController->genCmdTarget.right)/2,
                                                                                 (MainController->genCmdTarget.top + MainController->genCmdTarget.bottom)/2) );
      continue;
    case AISCRIPT::GIVE_MONEY:  // COMPLETE (workaround w/ cheats)
      this->saveDebug(Text::Green, bOpcode);

      // Minerals
      if ( Broodwar->self()->minerals() < 500 )
        for ( int i = 0; i < 2000; i += 500 )
          Broodwar->sendText("whats mine is mine");

      // Gas
      if ( Broodwar->self()->gas() < 500 )
        for ( int i = 0; i < 2000; i += 500 )
          Broodwar->sendText("breathe deep");

      continue;
    case AISCRIPT::PREP_DOWN: // not started
      {
        BYTE bSaveCount    = this->read<BYTE>();
        BYTE bMinimum      = this->read<BYTE>();
        UnitType wMilitary = UnitType(this->read<WORD>());
        this->saveDebug(Text::Red, bOpcode, "%3u %3u %s", bSaveCount, bMinimum, AISCRIPT::getUnitName(wMilitary));

        DWORD dwCountToAdd = GetStandardUnitCount(wMilitary, false) - bSaveCount;
        if ( dwCountToAdd < bMinimum )
          dwCountToAdd = bMinimum;
        // AI_AttackAdd(dwCountToAdd, wMilitary);
        continue;
      }
    case AISCRIPT::RESOURCES_JUMP:  // COMPLETE
      {
        WORD wOre = this->read<WORD>();
        WORD wGas = this->read<WORD>();
        WORD wJmp = this->read<WORD>();
        if ( Broodwar->self()->minerals() >= wOre &&
             Broodwar->self()->gas()      >= wGas )
          this->dwScriptOffset = wJmp;
        this->saveDebug(Text::Green, bOpcode, "%3u %3u p_%X", wOre, wGas, wJmp);
        continue;
      }
    case AISCRIPT::ENTER_TRANSPORT: // completed for the most part
      this->saveDebug(Text::Yellow, bOpcode);
      {
        Unitset unitsForTransport( bw->getUnitsInRectangle(this->locationBounds.left, locationBounds.top, locationBounds.right, locationBounds.bottom, 
                                                                GetPlayer == self && IsCompleted && GetRace == Races::Terran && IsOrganic ) );
      
        // Load units into closest transports
        for ( auto u = unitsForTransport.begin(); u != unitsForTransport.end(); ++u )
        {
          // Obtain the closest valid transport
          Unit *pTrans = u->getClosestUnit(GetType != UnitTypes::Terran_Bunker &&
                                            IsTransport &&
                                            GetPlayer == self &&
                                            SpaceRemaining >= u->getType().spaceRequired() );
          if ( pTrans ) // If transport was found
            u->rightClick(pTrans);
        }
      }
      continue;
    case AISCRIPT::EXIT_TRANSPORT: // COMPLETED
      this->saveDebug(Text::Green, bOpcode);
      bw->getUnitsInRectangle(this->locationBounds.left, locationBounds.top, locationBounds.right, locationBounds.bottom,
                              IsCompleted && GetPlayer == self && !IsHallucination && 
                              (GetType != UnitTypes::Zerg_Overlord || [](Unit*){return self->getUpgradeLevel(UpgradeTypes::Ventral_Sacs);} ) && IsTransport ).unloadAll();
      continue;
    case AISCRIPT::SHAREDVISION_ON: // WORKAROUND (performs reverse vision)
      {
        BYTE bPlayer = this->read<BYTE>();
        this->saveDebug(Text::Yellow, bOpcode, "%3u", bPlayer);
        Player *pl = bw->getPlayer(bPlayer);
        bw->setVision(pl); // players swapped for compatibility
        continue;
      }
    case AISCRIPT::SHAREDVISION_OFF: // WORKAROUND (performs reverse vision)
      {
        BYTE bPlayer = this->read<BYTE>();
        this->saveDebug(Text::Yellow, bOpcode, "%3u", bPlayer);
        Player *pl = bw->getPlayer(bPlayer);
        bw->setVision(pl, false); // players swapped for compatibility
        continue;
      }
    case AISCRIPT::NUKE_LOCATION: // not started
      this->saveDebug(Text::Red, bOpcode);
      continue;
    case AISCRIPT::HARASS_LOCATION: // COMPLETED (no code for this)
      this->saveDebug(Text::Green, bOpcode);
      continue;
    case AISCRIPT::IMPLODE: // not started
      this->saveDebug(Text::Red, bOpcode);
      // Implode();
      continue;
    case AISCRIPT::GUARD_ALL: // not started
      this->saveDebug(Text::Red, bOpcode);
      //for each ( Unit *u in self->getUnits() ) {}
        // MakeGuard(u);
      continue;
    case AISCRIPT::ENEMYOWNS_JUMP:  // COMPLETED (just about)
      {
        UnitType wType = UnitType(this->read<WORD>());
        WORD wJump     = this->read<WORD>();
        this->saveDebug(Text::Green, bOpcode, "%s p_%X", AISCRIPT::getUnitName(wType), wJump);
        
        // @TODO: BWAPI: Playerset::getUnitCount something
        Unitset enemyUnits(bw->enemies().getUnits());
        enemyUnits.erase_if( GetType != wType );
        if ( !enemyUnits.empty() )
          this->dwScriptOffset = wJump;
        continue;
      }
    case AISCRIPT::ENEMYRESOURCES_JUMP:  // completed ?
      {
        WORD wOre = this->read<WORD>();
        WORD wGas = this->read<WORD>();
        WORD wJmp = this->read<WORD>();
        this->saveDebug(Text::Yellow, bOpcode, "%3u %3u p_%X", wOre, wGas, wJmp);

        Playerset enemies( bw->enemies() );
        enemies.erase_if( [](Player *p) { return p->allUnitCount(UnitTypes::AllUnits) == 0; } );

        Player *plr = bw->enemies().front();
        if ( plr                     &&
             plr->minerals() >= wOre &&
             plr->gas()      >= wGas )
          this->dwScriptOffset = wJmp;
        continue;
      }
    case AISCRIPT::IF_DIF:  // COMPLETE
      {
        BYTE bModifier = this->read<BYTE>();
        BYTE bVal      = this->read<BYTE>();
        WORD wBlock    = this->read<WORD>();
        this->saveDebug(Text::Green, bOpcode, "%3u %3u p_%X", bModifier, bVal, wBlock);
        BYTE bSaved = MainController->bIfDif;
        if ( bModifier ? (bSaved > bVal) : (bSaved < bVal) )
          this->dwScriptOffset = wBlock;
        continue;
      }
    case AISCRIPT::EASY_ATTACK: // not started 
      {
        BYTE     bCount = this->read<BYTE>();
        UnitType wUnit  = UnitType(this->read<WORD>());
        this->saveDebug(Text::Red, bOpcode, "%3u %s", bCount, AISCRIPT::getUnitName(wUnit));
        if ( !MainController->bIfDif )
        {
          // AI_AttackAdd 
        }
        continue;
      }
    case AISCRIPT::KILL_THREAD:  // COMPLETE
      {
        for ( std::list<aithread>::iterator i = aiThreadList.begin(); i != aiThreadList.end(); ++i )
        {
          if ( i->getFlags() & AI_THREAD_KILLABLE )
            i->killThread();
        }
        this->saveDebug(Text::Green, bOpcode);
        continue;
      }
    case AISCRIPT::KILLABLE:  // COMPLETE
      this->saveDebug(Text::Green, bOpcode);
      this->setFlags(AI_THREAD_KILLABLE);
      continue;
    case AISCRIPT::WAIT_FINISHATTACK: // not started
      this->saveDebug(Text::Red, bOpcode);
      // if ( AIIsAttacking )
      {
        retryBlock = false;
        continue;
      }
      // this->retry();
      // return;
    case AISCRIPT::QUICK_ATTACK:  // not started
      this->saveDebug(Text::Red, bOpcode);
      // AI_AttackManager(thisLocation, 1, 0);
      MainController->dwAttackTime = bw->elapsedTime() - 175;
      continue;
    case AISCRIPT::JUNKYARD_DOG:  // not started
      for each ( Unit *u in bw->getUnitsInRectangle(locationBounds.left, locationBounds.top, locationBounds.right, locationBounds.bottom, GetPlayer == self) )
      {
        // run junkyard dog
      }
      this->saveDebug(Text::Red, bOpcode);
      continue;
    case AISCRIPT::FAKE_NUKE:   // COMPLETED
      this->saveDebug(Text::Green, bOpcode);
      MainController->dwLastNukeTime = bw->elapsedTime() + 1;
      continue;
    case AISCRIPT::DISRUPTION_WEB:  // COMPLETED
      this->saveDebug(Text::Green, bOpcode);
      {
        Unit *pCorsair = bw->getClosestUnit(this->locationCenter, GetType == UnitTypes::Protoss_Corsair &&
                                                                  IsIdle &&
                                                                  GetPlayer == self &&
                                                                  Energy >= TechTypes::Disruption_Web.energyCost() );
        if ( pCorsair )
          pCorsair->useTech(TechTypes::Disruption_Web, this->locationCenter);
      }
      continue;
    case AISCRIPT::RECALL_LOCATION: // COMPLETED
      this->saveDebug(Text::Green, bOpcode);
      {
        Unit *pArbiter = bw->getClosestUnit(this->locationCenter, GetType == UnitTypes::Protoss_Arbiter &&
                                                                  IsIdle &&
                                                                  GetPlayer == self &&
                                                                  Energy >= TechTypes::Recall.energyCost() );
        if ( pArbiter )
          pArbiter->useTech(TechTypes::Recall, this->locationCenter);
      }
      continue;
    case AISCRIPT::SET_RANDOMSEED:  // COMPLETED
      {
        DWORD dwRandomSeed = this->read<DWORD>();
        srand(dwRandomSeed);
        this->saveDebug(Text::Green, bOpcode, "%X", dwRandomSeed);
        continue;
      }
    case AISCRIPT::IF_OWNED:  // COMPLETED
      {
        UnitType wType = UnitType(this->read<WORD>());
        WORD wJump     = this->read<WORD>();
        this->saveDebug(Text::Green, bOpcode, "%s p_%X", AISCRIPT::getUnitName(wType), wJump);
        if ( self->allUnitCount(wType) )
          this->dwScriptOffset = wJump;
        continue;
      }
    case AISCRIPT::CREATE_NUKE: // not started (can't be emulated)
      this->saveDebug(Text::Red, bOpcode);
      continue;
    case AISCRIPT::CREATE_UNIT: // not started (can't be emulated)
      {
        UnitType wType  = UnitType(this->read<WORD>()); // type
        WORD wX         = this->read<WORD>(); // x
        WORD wY         = this->read<WORD>(); // y
        this->saveDebug(Text::Red, bOpcode, "%s %3u %3u", AISCRIPT::getUnitName(wType), wX, wY);
        continue;
      }
    case AISCRIPT::NUKE_POS:  // not started
      {
        WORD wX         = this->read<WORD>(); // x
        WORD wY         = this->read<WORD>(); // y
        this->saveDebug(Text::Red, bOpcode, "%3u %3u", wX, wY);
        continue;
      }
    case AISCRIPT::HELP_IFTROUBLE:  // not started
      this->saveDebug(Text::Red, bOpcode);
      // HelpIfTrouble();
      continue;
    case AISCRIPT::ALLIES_WATCH: // not started
      {
        BYTE bUnk1  = this->read<BYTE>();
        WORD wBlock = this->read<WORD>();
        this->saveDebug(Text::Red, bOpcode, "%3u p_%X", bUnk1, wBlock);
        continue;
      }
    case AISCRIPT::TRY_TOWNPOINT: // not started
      {
        BYTE bExpansions  = this->read<BYTE>();
        WORD wBlock       = this->read<WORD>();
        this->saveDebug(Text::Red, bOpcode, "%3u p_%X", bExpansions, wBlock);
        // if ( GetExpansionCount() == bExpansions )
        //   this->dwScriptOffset = wJump;
        continue;
      }
    default:
      Broodwar->printf("%cBad script opcode %02X", 6, bOpcode);
      
      return;
    } // switch
  } // loop
}
