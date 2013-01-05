#include "ScriptThread.h"
#include "ScriptEnum.h"
#include "BWScriptEmulator.h"
#include "Controller.h"
#include "UnitInfo.h"

#include <BWAPI.h>

using namespace BWAPI;


DWORD AISavedCall;
int creep;

void aithread::execute()
{
  this->dwBytesRead = 0;
  if ( !pbAIScriptBinary )
  {
    Broodwar << Text::Red << "No binary loaded!" << std::endl;
    return this->killThread();
  }
  for (;;)
  {
    if ( this->dwScriptOffset == 0 )
    {
      Broodwar << Text::Red << "Script offset is 0!" << std::endl;
      return this->killThread();
    }

    this->dwBytesRead = 0;
    BYTE bOpcode = read<BYTE>();
    switch ( bOpcode )
    {
/*    case AISCRIPT::Enum::GOTO:  // COMPLETE
      this->dwScriptOffset = this->read<WORD>();
      this->saveDebug(Text::Green, bOpcode, "p_%X", this->dwScriptOffset);
      continue;
    case AISCRIPT::Enum::NOTOWNS_JUMP:  // COMPLETE
      {
        UnitType uType    = UnitType(this->read<WORD>());
        WORD     jOffset  = this->read<WORD>();
        this->saveDebug(Text::Green, bOpcode, "%s p_%X", AISCRIPT::getUnitName(uType), jOffset);
        if ( self->allUnitCount(uType) == 0 )
          this->dwScriptOffset = jOffset;
        continue;
      }
    case AISCRIPT::Enum::WAIT:  // COMPLETE
      this->setSleep(this->read<WORD>());
      this->saveDebug(Text::Green, bOpcode, "%3u", this->dwSleepTime);
      return;
    case AISCRIPT::Enum::START_TOWN:      // incomplete
    case AISCRIPT::Enum::START_AREATOWN:  // incomplete
      this->saveDebug(Text::Red, bOpcode);

      MainController.wFlags |= CONTROLLER_TOWN_STARTED;
      MainController.bIfDif = 1;
      // AI_StartShareTown();
      continue;
    case AISCRIPT::Enum::EXPAND:          // not started
      {
        BYTE bExpandMax   = this->read<BYTE>();
        WORD wExpandBlock = this->read<WORD>();
        this->saveDebug(Text::Red, bOpcode, "%3u p_%X", bExpandMax, wExpandBlock);
        // if ( GetExpansionCount() < bExpandMax )
        //   this->Expand(wExpandBlock, bExpandMax);
        continue;
      }
    case AISCRIPT::Enum::BUILD: // unfinished
      {
        BYTE bBuildCount    = this->read<BYTE>();
        UnitType wBuildType = UnitType(this->read<WORD>());
        BYTE bPriority      = this->read<BYTE>();

        this->saveDebug(Text::Red, bOpcode, "%3u %s %3u", bBuildCount, AISCRIPT::getUnitName(wBuildType), bPriority);
        //if ( bBuildCount <= 30 && wBuildType < UnitTypes::None )
        //  this->pTown->MacroManagerSet(MACRO_BUILD, bBuildCount, wBuildType, bPriority, false);
        continue;
      }
    case AISCRIPT::Enum::UPGRADE: // unfinished
      {
        BYTE bUpgLevel = this->read<BYTE>();
        UpgradeType wUpgType  = UpgradeType(this->read<WORD>());
        BYTE bPriority = this->read<BYTE>();
        this->saveDebug(Text::Red, bOpcode, "%3u %s %3u", bUpgLevel, wUpgType.c_str(), bPriority);
        //this->pTown->MacroManagerSet(MACRO_UPGRADE, bUpgLevel, wUpgType, bPriority, false);
        continue;
      }
    case AISCRIPT::Enum::TECH: // unfinished
      {
        TechType wTechType = TechType(this->read<WORD>());
        BYTE bPriority = this->read<BYTE>();
        this->saveDebug(Text::Red, bOpcode, "%s %3u", wTechType.c_str(), bPriority);
        //this->pTown->MacroManagerSet(MACRO_TECH, 1, wTechType, bPriority, false);
        continue;
      }
    case AISCRIPT::Enum::WAIT_BUILD: // incomplete
    case AISCRIPT::Enum::WAIT_BUILDSTART: // incomplete
      {
        BYTE bCount     = this->read<BYTE>();
        UnitType wType  = UnitType(this->read<WORD>());
        this->saveDebug(Text::Red, bOpcode, "%3u %s", bCount, AISCRIPT::getUnitName(wType) );
        if ( wType == self->getRace().getWorker() )
        {
          // TOWN STUFF
        }
        this->retry();
        return;
      }
    case AISCRIPT::Enum::ATTACK_CLEAR:  // unfinished
      this->saveDebug(Text::Yellow, bOpcode);
      memset(MainController.attackGroups, 0, sizeof(MainController.attackGroups));
      MainController.wAttackGroup = 0;
      MainController.dwAttackTime = 0;
      // AttackClear();
      continue;
    case AISCRIPT::Enum::ATTACK_ADD:  // COMPLETED
      {
        BYTE bCount     = this->read<BYTE>();
        UnitType wType  = UnitType(this->read<WORD>());
        this->saveDebug(Text::Green, bOpcode, "%3u %s", bCount, AISCRIPT::getUnitName(wType) );
        MainController.AttackAdd(bCount, wType);
        continue;
      }
    case AISCRIPT::Enum::ATTACK_PREPARE:  // not started
      this->saveDebug(Text::Red, bOpcode);
      // AI_AttackManager(thisLocation, 1, 0);
      continue;
    case AISCRIPT::Enum::ATTACK_DO:  // not started
      this->saveDebug(Text::Red, bOpcode);
      // if ( AI_AttackManager(thisLocation, 1, 0) )
      {
        this->noretry();
        continue;
      }
      // this->retry();
      // return;
    case AISCRIPT::Enum::WAIT_SECURE:  // COMPLETED
      this->saveDebug(Text::Green, bOpcode);
      if ( MainController.wFlags & CONTROLLER_SECURE_FINISHED )
      {
        this->noretry();
        continue;
      }
      this->retry();
      return;
    case AISCRIPT::Enum::CAPT_EXPAND:  // not started
      this->saveDebug(Text::Red, bOpcode);
      continue;
    case AISCRIPT::Enum::BUILD_BUNKERS:  // not started
      this->saveDebug(Text::Red, bOpcode);
      // BuildBunkers();
      continue;
    case AISCRIPT::Enum::WAIT_BUNKERS:  // not started
      this->saveDebug(Text::Red, bOpcode);
      // if ( AIWaitBunkersFinished() )
      {
        this->noretry();
        continue;
      }
      //this->retry();
      //return;
    case AISCRIPT::Enum::DEFENSEBUILD_GG:  // COMPLETED
    case AISCRIPT::Enum::DEFENSEBUILD_AG:  // COMPLETED
    case AISCRIPT::Enum::DEFENSEBUILD_GA:  // COMPLETED
    case AISCRIPT::Enum::DEFENSEBUILD_AA:  // COMPLETED
      {
        BYTE     bCount = this->read<BYTE>();
        UnitType wType  = UnitType(this->read<WORD>());
        this->saveDebug(Text::Green, bOpcode, "%3u %s", bCount, AISCRIPT::getUnitName(wType) );
        MainController.DefenseBuild(bOpcode - AISCRIPT::Enum::DEFENSEBUILD_GG, bCount, wType);
        continue;
      }
    case AISCRIPT::Enum::DEFENSEUSE_GG:  // COMPLETED
    case AISCRIPT::Enum::DEFENSEUSE_AG:  // COMPLETED
    case AISCRIPT::Enum::DEFENSEUSE_GA:  // COMPLETED
    case AISCRIPT::Enum::DEFENSEUSE_AA:  // COMPLETED
      {
        BYTE     bCount = this->read<BYTE>();
        UnitType wType  = UnitType(this->read<WORD>());
        this->saveDebug(Text::Green, bOpcode, "%3u %s", bCount, AISCRIPT::getUnitName(wType) );
        MainController.DefenseUse(bOpcode - AISCRIPT::Enum::DEFENSEUSE_GG, bCount, wType);
        continue;
      }
    case AISCRIPT::Enum::DEFENSECLEAR_GG:  // COMPLETED
    case AISCRIPT::Enum::DEFENSECLEAR_AG:  // COMPLETED
    case AISCRIPT::Enum::DEFENSECLEAR_GA:  // COMPLETED
    case AISCRIPT::Enum::DEFENSECLEAR_AA:  // COMPLETED
      this->saveDebug(Text::Green, bOpcode);
      MainController.DefenseClear(bOpcode - AISCRIPT::Enum::DEFENSECLEAR_GG);
      continue;
    case AISCRIPT::Enum::SEND_SUICIDE:  // not started
      {
        BYTE bSuicideType = this->read<BYTE>();
        this->saveDebug(Text::Red, bOpcode, "%3u", bSuicideType);
        memset(MainController.attackGroups, 0, sizeof(MainController.attackGroups));
        MainController.wAttackGroup = 0;
        // MainController.dwAttackTime = 0;
        // AttackClear();
        // SendSuicide(bSuicideType);
        MainController.dwAttackTime = bw->elapsedTime() - 175;
        continue;
      }
    case AISCRIPT::Enum::PLAYER_ENEMY:  // COMPLETED
    case AISCRIPT::Enum::PLAYER_ALLY:   // COMPLETED
      {
        this->saveDebug(Text::Green, bOpcode);
        // @TODO: BWAPI: Unitset::getPlayers for retrieving set of players owning the units, not important
        Unitset unitsInRect( bw->getUnitsInRectangle(location.topLeft, location.bottomRight, GetPlayer != self) );
        for ( Unitset::iterator u = unitsInRect.begin(); u != unitsInRect.end(); ++u )
          bw->setAlliance(u->getPlayer(), bOpcode == AISCRIPT::Enum::PLAYER_ALLY);
        continue;
      }
    case AISCRIPT::Enum::DEFAULT_MIN:  // COMPLETED
      MainController.bDefaultMin = this->read<BYTE>();
      this->saveDebug(Text::Green, bOpcode, "%3u", MainController.bDefaultMin);
      continue;
    case AISCRIPT::Enum::DEFAULTBUILD_OFF:  // COMPLETE
      this->saveDebug(Text::Green, bOpcode);
      MainController.wFlags |= CONTROLLER_DEFAULT_BUILD_OFF;
      continue;
    case AISCRIPT::Enum::STOP:    // COMPLETE
      this->saveDebug(Text::Green, bOpcode);
      this->killThread();
      return;
    case AISCRIPT::Enum::SWITCH_RESCUE:  // COMPLETED (can't actually emulate)
      this->saveDebug(Text::Yellow, bOpcode);
      bw->enemies().setAlliance(true); // closest thing to becoming neutral+rescuable
      continue;
    case AISCRIPT::Enum::MOVE_DT: // WORKAROUND
      {
        this->saveDebug(Text::Yellow, bOpcode);
        Unitset myUnits(self->getUnits());
      
        myUnits.erase_if( !((GetType == UnitTypes::Protoss_Dark_Templar || GetType == UnitTypes::Hero_Dark_Templar)
                            && Exists && IsCompleted) );
        myUnits.move(this->location.center());
        // Note: Needs special reassignment (CAIControl, AICaptain)
        continue;
      }
    case AISCRIPT::Enum::DEBUG:   // COMPLETE
      {
        WORD wJump = this->read<WORD>();
        Broodwar->sendText("%s", &pbAIScriptBinary[this->dwScriptOffset]);
        this->saveDebug(Text::Green, bOpcode, "p_%X %s", wJump, &pbAIScriptBinary[this->dwScriptOffset]);
        this->dwScriptOffset = wJump;
        continue;
      }
    case AISCRIPT::Enum::FATAL_ERROR: // COMPLETED
      Broodwar->sendText("Illegal AI script executed.");
      this->saveDebug(Text::Green, bOpcode);
      continue;
    case AISCRIPT::Enum::ENTER_BUNKER: // completed for the most part
      {
        this->saveDebug(Text::Yellow, bOpcode);
        // ignore if there are no bunkers
        if ( !self->completedUnitCount(UnitTypes::Terran_Bunker) )
          continue;

        Unitset unitsForBunker( bw->getUnitsInRectangle(location.topLeft, location.bottomRight, 
                                                                GetPlayer == self && IsCompleted && GetRace == Races::Terran && IsOrganic ) );
      
        // Iterate the units that are to enter the bunkers
        for ( auto u = unitsForBunker.begin(); u != unitsForBunker.end(); ++u )
        {
          // Find a bunker closest to the current unit that has space available
          Unit *pBunker = u->getClosestUnit(GetType == UnitTypes::Terran_Bunker &&
                                            GetPlayer == self && 
                                            SpaceRemaining >= u->getType().spaceRequired() );
          
          if ( pBunker )  // If a bunker was found
            u->rightClick(pBunker);
        }
      }
      continue;
    case AISCRIPT::Enum::VALUE_AREA: // not started
      this->saveDebug(Text::Red, bOpcode);
      // ValueArea(this->location.center());
      continue;
    case AISCRIPT::Enum::TRANSPORTS_OFF: // COMPLETE
      this->saveDebug(Text::Green, bOpcode);
      MainController.wFlags |= CONTROLLER_TRANSPORTS_OFF;
      continue;
    case AISCRIPT::Enum::CHECK_TRANSPORTS: // COMPLETE
      this->saveDebug(Text::Green, bOpcode);
      MainController.wFlags &= ~CONTROLLER_TRANSPORTS_OFF;
      continue;
    case AISCRIPT::Enum::NUKE_RATE: // COMPLETE
      MainController.bNukeRate = this->read<BYTE>();
      this->saveDebug(Text::Green, bOpcode, "%3u", MainController.bNukeRate);
      continue;
    case AISCRIPT::Enum::MAX_FORCE: // COMPLETED
      MainController.wMaxForce = UnitType(this->read<WORD>());
      this->saveDebug(Text::Green, bOpcode, "%s", AISCRIPT::getUnitName(MainController.wMaxForce) );
      continue;
    case AISCRIPT::Enum::CLEAR_COMBATDATA: // not started
      this->saveDebug(Text::Red, bOpcode);
      // ClearCombatData(this->location);
      continue;
    case AISCRIPT::Enum::RANDOM_JUMP: // COMPLETED
      {
        BYTE bRandChance = this->read<BYTE>();
        WORD wJmpOffset = this->read<WORD>();
        
        this->saveDebug(Text::Green, bOpcode, "%3u p_%X", bRandChance, wJmpOffset);
        if ( LOBYTE(rand()) <= bRandChance )
          this->dwScriptOffset = wJmpOffset;
        continue;
      }
    case AISCRIPT::Enum::TIME_JUMP: // COMPLETED
      {
        BYTE bTimePast  = 60 * this->read<BYTE>();
        WORD wJmpOffset = this->read<WORD>();
        this->saveDebug(Text::Green, bOpcode, "%3u p_%X", bTimePast, wJmpOffset);
        if ( bw->elapsedTime() >= bTimePast )
          this->dwScriptOffset = wJmpOffset;
        continue;
      }
    case AISCRIPT::Enum::FARMS_NOTIMING: // COMPLETED
      this->saveDebug(Text::Green, bOpcode);
      MainController.wFlags |= CONTROLLER_FARMS_TIMING_OFF;
      continue;
    case AISCRIPT::Enum::FARMS_TIMING: // COMPLETED
      this->saveDebug(Text::Green, bOpcode);
      MainController.wFlags &= ~CONTROLLER_FARMS_TIMING_OFF;
      continue;
    case AISCRIPT::Enum::BUILD_TURRETS: // not started
      this->saveDebug(Text::Red, bOpcode);
      // BuildTurrets();
      continue;
    case AISCRIPT::Enum::WAIT_TURRETS: // not started
      this->saveDebug(Text::Red, bOpcode);
      // if ( AIWaitTurretsFinished() )
      {
        this->noretry();
        continue;
      }
      //this->retry();
      //return;
    case AISCRIPT::Enum::DEFAULT_BUILD: // COMPLETED
      this->saveDebug(Text::Green, bOpcode);
      MainController.wFlags &= ~CONTROLLER_DEFAULT_BUILD_OFF;
      continue;
    case AISCRIPT::Enum::HARASS_FACTOR: // not started
      {
        WORD wUnk = this->read<WORD>();
        this->saveDebug(Text::Red, bOpcode, "%3u", wUnk);
        // HarassFactor(wUnk);
        continue;
      }
    case AISCRIPT::Enum::START_CAMPAIGN: // COMPLETE
      this->saveDebug(Text::Green, bOpcode);
      MainController.wFlags |= CONTROLLER_IS_CAMPAIGN;
      continue;
    case AISCRIPT::Enum::RACE_JUMP: // WORKAROUND
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
    case AISCRIPT::Enum::REGION_SIZE: // not started
      {
        BYTE bSize  = this->read<BYTE>();
        WORD wBlock = this->read<WORD>();
        this->saveDebug(Text::Red, bOpcode, "%3u p_%X", bSize, wBlock);
        // @TODO: Region::getTileCount
        // if ( this->pTown->RegionSize() < bSize )
        //   this->dwScriptOffset = wBlock;
        continue;
      }
    case AISCRIPT::Enum::GET_OLDPEONS: // not started
      {
        BYTE bCount = this->read<BYTE>();
        this->saveDebug(Text::Red, bOpcode, "%3u", bCount);
        // this->pTown->GetOldPeons(bCount);
        continue;
      }
    case AISCRIPT::Enum::GROUNDMAP_JUMP: // COMPLETED
      {
        WORD wJmpOffset = this->read<WORD>();
        this->saveDebug(Text::Green, bOpcode, "p_%X", wJmpOffset);
        
        Unitset enemyUnits( bw->enemies().getUnits() );
        enemyUnits.erase_if( !(Exists && IsBuilding && !IsLifted) );
        for ( auto u = enemyUnits.begin(); u != enemyUnits.end(); ++u )
        {
          if ( u->hasPath(this->location.center()) )
          {
            this->dwScriptOffset = wJmpOffset;
            break;
          }
        }
        continue;
      }
    case AISCRIPT::Enum::PLACE_GUARD: // not started
      {
        UnitType wType = UnitType(this->read<WORD>());
        BYTE bIndex    = this->read<BYTE>();
        this->saveDebug(Text::Red, bOpcode, "%s %3u", AISCRIPT::getUnitName(wType), bIndex);
        // this->pTown->PlaceGuard(bIndex, wType);
        continue;
      }
    case AISCRIPT::Enum::GUARD_RESOURCES: // not started
      {
        UnitType wType = UnitType(this->read<WORD>());
        this->saveDebug(Text::Red, bOpcode, "%s", AISCRIPT::getUnitName(wType));
        // GuardResources(wType);
        continue;
      }
    case AISCRIPT::Enum::CALL:    // COMPLETE
      AISavedCall           = this->dwScriptOffset;
      this->dwScriptOffset  = this->read<WORD>();
      this->saveDebug(Text::Green, bOpcode, "p_%X", this->dwScriptOffset);
      continue;
    case AISCRIPT::Enum::RETURN:    // COMPLETE
      this->saveDebug(Text::Green, bOpcode);
      this->dwScriptOffset = AISavedCall;
      AISavedCall = 0;
      continue;*/
    case AISCRIPT::Enum::EVAL_HARASS: // not started
      {
        WORD wBlock = this->read<WORD>();
        this->saveDebug(Text::Red, bOpcode, "p_%X", wBlock);
        //if ( !EvalHarass(this->location.center()) )
        //  this->dwScriptOffset = wBlock;
        continue;
      }
    case AISCRIPT::Enum::CREEP:   // COMPLETE
      {
        BYTE bCreepFlag = this->read<BYTE>();
        this->saveDebug(Text::Green, bOpcode, "%3u", bCreepFlag);
        if ( bCreepFlag == 4 )
          MainController.wFlags |= CONTROLLER_SPREAD_CREEP;
        else
          MainController.wFlags &= ~CONTROLLER_SPREAD_CREEP;
        continue;
      }
    case AISCRIPT::Enum::PANIC: // COMPLETED
      MainController.wPanicBlock = this->read<WORD>();
      this->saveDebug(Text::Green, bOpcode, "p_%X", MainController.wPanicBlock);
      continue;
    case AISCRIPT::Enum::PLAYER_NEED: // incomplete
      {
        BYTE bBuildCount    = this->read<BYTE>();
        UnitType wBuildType = UnitType(this->read<WORD>());
        this->saveDebug(Text::Red, bOpcode, "%3u %s", bBuildCount, AISCRIPT::getUnitName(wBuildType));
        //if ( bBuildCount <= 30 && wBuildType < UnitTypes::None )
        //  this->pTown->MacroManagerSet(MACRO_BUILD, bBuildCount, wBuildType, 80, true);
        continue;
      }
    case AISCRIPT::Enum::WAIT_UPGRADES: // COMPLETED
      this->saveDebug(Text::Green, bOpcode);
      if ( MainController.wFlags & CONTROLLER_UPGRADES_FINISHED )
      {
        this->noretry();
        continue;
      }
      this->retry();
      return;
    case AISCRIPT::Enum::MULTIRUN:    // COMPLETE
      {
        WORD wMultirunBlock = this->read<WORD>();
        this->saveDebug(Text::Green, bOpcode, "p_%X", wMultirunBlock);
        AICreateThread(wMultirunBlock, this->location.center(), this->pTown);
        continue;
      }
    case AISCRIPT::Enum::RUSH:  // completed ?
      {
        BYTE bRushCategory = this->read<BYTE>();
        WORD wRushBlock    = this->read<WORD>();

        this->saveDebug(Text::Yellow, bOpcode, "%3u p_%X", bRushCategory, wRushBlock);
        bool isRushed = false;
        
        // Get closest enemy owner
        Unit *pClosest = Broodwar->getClosestUnit(this->location.center(), IsEnemy && (!IsBuilding || IsLifted) );
        Player *closestEnemy = pClosest ? pClosest->getPlayer() : nullptr;

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
    case AISCRIPT::Enum::SCOUT_WITH:  // COMPLETE (this function does nothing in Starcraft, may become necessary in emulator)
      {
        UnitType wScout = UnitType(this->read<WORD>());
        this->saveDebug(Text::Green, bOpcode, "%s", AISCRIPT::getUnitName(wScout));
        continue;
      }
    case AISCRIPT::Enum::DEFINE_MAX: // COMPLETED
      {
        BYTE     bCount = this->read<BYTE>();
        UnitType wType  = UnitType(this->read<WORD>());
        this->saveDebug(Text::Green, bOpcode, "%3u %s", bCount, AISCRIPT::getUnitName(wType));
        if ( wType < UnitTypes::None )
          MainController.bDefineMax[wType] = bCount;
        continue;
      }
    case AISCRIPT::Enum::DO_MORPH:  // COMPLETED
    case AISCRIPT::Enum::WAIT_FORCE:  // COMPLETED
    case AISCRIPT::Enum::TRAIN: // COMPLETED
      {
        BYTE     bCount = this->read<BYTE>();
        UnitType wType  = UnitType(this->read<WORD>());

        this->saveDebug(Text::Green, bOpcode, "%3u %s", bCount, AISCRIPT::getUnitName(wType));
        if ( GetStandardUnitCount(wType, bOpcode != AISCRIPT::Enum::TRAIN) < bCount )
        {
          MainController.wWaitForType = (int)wType + 1;
          if ( bOpcode != AISCRIPT::Enum::DO_MORPH )
          {
            this->retry();
            return;
          }
        }
        this->noretry();
        continue;
      }
    case AISCRIPT::Enum::TARGET_EXPANSION: // COMPLETE
      this->saveDebug(Text::Green, bOpcode);
      MainController.wFlags |= CONTROLLER_TARGET_EXPANSION;
      continue;
    case AISCRIPT::Enum::WAIT_TRAIN: // COMPLETE
      {
        BYTE     bCount = this->read<BYTE>();
        UnitType wType  = UnitType(this->read<WORD>());

        this->saveDebug(Text::Green, bOpcode, "%3u %s", bCount, AISCRIPT::getUnitName(wType));
        if ( GetStandardUnitCount(wType) < bCount )
        {
          this->retry();
          return;
        }
        this->noretry();
        continue;
      }
    case AISCRIPT::Enum::SET_ATTACKS: // COMPLETE
      MainController.bAttacks = this->read<BYTE>();
      this->saveDebug(Text::Green, bOpcode, "%3u", MainController.bAttacks);
      continue;
    case AISCRIPT::Enum::SET_GENCMD: // COMPLETED
      this->saveDebug(Text::Green, bOpcode);
      MainController.genCmdTarget = this->location;
      continue;
    case AISCRIPT::Enum::MAKE_PATROL: // COMPLETED
      this->saveDebug(Text::Green, bOpcode);
      bw->getUnitsInRectangle(location.topLeft, location.bottomRight, GetPlayer == self && IsCompleted).patrol( MainController.genCmdTarget.center() );
      continue;
    case AISCRIPT::Enum::GIVE_MONEY:  // COMPLETE (workaround w/ cheats)
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
    case AISCRIPT::Enum::PREP_DOWN: // not started
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
    case AISCRIPT::Enum::RESOURCES_JUMP:  // COMPLETE
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
      /*
    case AISCRIPT::Enum::ENTER_TRANSPORT: // completed for the most part
      this->saveDebug(Text::Yellow, bOpcode);
      {
        Unitset unitsForTransport( bw->getUnitsInRectangle(location.topLeft, location.bottomRight, GetPlayer == self && IsCompleted && SpaceRequired < 8 ) );
      
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
    case AISCRIPT::Enum::EXIT_TRANSPORT: // COMPLETED
      this->saveDebug(Text::Green, bOpcode);
      bw->getUnitsInRectangle(location.topLeft, location.bottomRight, IsCompleted && GetPlayer == self && !IsHallucination && 
                              (GetType != UnitTypes::Zerg_Overlord || [](Unit*){return self->getUpgradeLevel(UpgradeTypes::Ventral_Sacs);} ) && IsTransport ).unloadAll();
      continue;*/
    case AISCRIPT::Enum::SHAREDVISION_ON: // WORKAROUND (performs reverse vision)
      {
        BYTE bPlayer = this->read<BYTE>();
        this->saveDebug(Text::Yellow, bOpcode, "%3u", bPlayer);
        Player *pl = bw->getPlayer(bPlayer);
        bw->setVision(pl); // players swapped for compatibility
        continue;
      }
    case AISCRIPT::Enum::SHAREDVISION_OFF: // WORKAROUND (performs reverse vision)
      {
        BYTE bPlayer = this->read<BYTE>();
        this->saveDebug(Text::Yellow, bOpcode, "%3u", bPlayer);
        Player *pl = bw->getPlayer(bPlayer);
        bw->setVision(pl, false); // players swapped for compatibility
        continue;
      }
    case AISCRIPT::Enum::NUKE_LOCATION: // not started
      this->saveDebug(Text::Red, bOpcode);
      continue;
    case AISCRIPT::Enum::HARASS_LOCATION: // COMPLETED (no code for this)
      this->saveDebug(Text::Green, bOpcode);
      continue;
    case AISCRIPT::Enum::IMPLODE: // not started
      this->saveDebug(Text::Red, bOpcode);
      // Implode();
      continue;
    case AISCRIPT::Enum::GUARD_ALL: // not started
      this->saveDebug(Text::Red, bOpcode);
      //for each ( Unit *u in self->getUnits() ) {}
        // MakeGuard(u);
      continue;
    case AISCRIPT::Enum::ENEMYOWNS_JUMP:  // COMPLETED (just about)
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
    case AISCRIPT::Enum::ENEMYRESOURCES_JUMP:  // completed ?
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
    case AISCRIPT::Enum::IF_DIF:  // COMPLETE
      {
        BYTE bModifier = this->read<BYTE>();
        BYTE bVal      = this->read<BYTE>();
        WORD wBlock    = this->read<WORD>();
        this->saveDebug(Text::Green, bOpcode, "%3u %3u p_%X", bModifier, bVal, wBlock);
        BYTE bSaved = MainController.bIfDif;
        if ( bModifier ? (bSaved > bVal) : (bSaved < bVal) )
          this->dwScriptOffset = wBlock;
        continue;
      }
    case AISCRIPT::Enum::EASY_ATTACK: // not started 
      {
        BYTE     bCount = this->read<BYTE>();
        UnitType wUnit  = UnitType(this->read<WORD>());
        this->saveDebug(Text::Red, bOpcode, "%3u %s", bCount, AISCRIPT::getUnitName(wUnit));
        if ( !MainController.bIfDif )
        {
          // AI_AttackAdd 
        }
        continue;
      }
    case AISCRIPT::Enum::KILL_THREAD:  // COMPLETE
      for ( std::list<aithread>::iterator i = aiThreadList.begin(); i != aiThreadList.end(); ++i )
      {
        if ( i->getFlags() & AI_THREAD_KILLABLE )
          i->killThread();
      }
      this->saveDebug(Text::Green, bOpcode);
      continue;
    case AISCRIPT::Enum::KILLABLE:  // COMPLETE
      this->saveDebug(Text::Green, bOpcode);
      this->setFlags(AI_THREAD_KILLABLE);
      continue;
    case AISCRIPT::Enum::WAIT_FINISHATTACK: // not started
      this->saveDebug(Text::Red, bOpcode);
      // if ( AIIsAttacking )
      {
        this->noretry();
        continue;
      }
      // this->retry();
      // return;
    case AISCRIPT::Enum::QUICK_ATTACK:  // not started
      this->saveDebug(Text::Red, bOpcode);
      // AI_AttackManager(thisLocation, 1, 0);
      MainController.dwAttackTime = bw->elapsedTime() - 175;
      continue;
    case AISCRIPT::Enum::JUNKYARD_DOG:  // COMPLETED
      {
        Unitset junkyardUnits( bw->getUnitsInRectangle(location.topLeft, location.bottomRight, GetPlayer == self) );
        for each ( Unit *u in junkyardUnits )
          UnitWrap(u).SetVirtualUnitOrder(Orders::Enum::JunkYardDog);
        this->saveDebug(Text::Red, bOpcode);
      }
      continue;
    case AISCRIPT::Enum::FAKE_NUKE:   // COMPLETED
      this->saveDebug(Text::Green, bOpcode);
      MainController.dwLastNukeTime = bw->elapsedTime() + 1;
      continue;
    case AISCRIPT::Enum::DISRUPTION_WEB:  // COMPLETED
      this->saveDebug(Text::Green, bOpcode);
      {
        Unit *pCorsair = bw->getClosestUnit(this->location.center(), GetType == UnitTypes::Protoss_Corsair &&
                                                                  IsIdle &&
                                                                  GetPlayer == self &&
                                                                  Energy >= TechTypes::Disruption_Web.energyCost() );
        if ( pCorsair )
          pCorsair->useTech(TechTypes::Disruption_Web, this->location.center());
      }
      continue;
    case AISCRIPT::Enum::RECALL_LOCATION: // COMPLETED
      this->saveDebug(Text::Green, bOpcode);
      {
        Unit *pArbiter = bw->getClosestUnit(this->location.center(), GetType == UnitTypes::Protoss_Arbiter &&
                                                                  IsIdle &&
                                                                  GetPlayer == self &&
                                                                  Energy >= TechTypes::Recall.energyCost() );
        if ( pArbiter )
          pArbiter->useTech(TechTypes::Recall, this->location.center());
      }
      continue;
    case AISCRIPT::Enum::SET_RANDOMSEED:  // COMPLETED
      {
        DWORD dwRandomSeed = this->read<DWORD>();
        srand(dwRandomSeed);
        this->saveDebug(Text::Green, bOpcode, "%X", dwRandomSeed);
        continue;
      }
    case AISCRIPT::Enum::IF_OWNED:  // COMPLETED
      {
        UnitType wType = UnitType(this->read<WORD>());
        WORD wJump     = this->read<WORD>();
        this->saveDebug(Text::Green, bOpcode, "%s p_%X", AISCRIPT::getUnitName(wType), wJump);
        if ( self->allUnitCount(wType) )
          this->dwScriptOffset = wJump;
        continue;
      }
    case AISCRIPT::Enum::CREATE_NUKE: // not started (can't be emulated)
      this->saveDebug(Text::Red, bOpcode);
      continue;
    case AISCRIPT::Enum::CREATE_UNIT: // not started (can't be emulated)
      {
        UnitType wType  = UnitType(this->read<WORD>()); // type
        WORD wX         = this->read<WORD>(); // x
        WORD wY         = this->read<WORD>(); // y
        this->saveDebug(Text::Red, bOpcode, "%s %3u %3u", AISCRIPT::getUnitName(wType), wX, wY);
        continue;
      }
    case AISCRIPT::Enum::NUKE_POS:  // not started
      {
        WORD wX         = this->read<WORD>(); // x
        WORD wY         = this->read<WORD>(); // y
        this->saveDebug(Text::Red, bOpcode, "%3u %3u", wX, wY);
        continue;
      }
    case AISCRIPT::Enum::HELP_IFTROUBLE:  // not started
      this->saveDebug(Text::Red, bOpcode);
      // HelpIfTrouble();
      continue;
    case AISCRIPT::Enum::ALLIES_WATCH: // not started
      {
        BYTE bUnk1  = this->read<BYTE>();
        WORD wBlock = this->read<WORD>();
        this->saveDebug(Text::Red, bOpcode, "%3u p_%X", bUnk1, wBlock);
        continue;
      }
    case AISCRIPT::Enum::TRY_TOWNPOINT: // not started
      {
        BYTE bExpansions  = this->read<BYTE>();
        WORD wBlock       = this->read<WORD>();
        this->saveDebug(Text::Red, bOpcode, "%3u p_%X", bExpansions, wBlock);
        // if ( GetExpansionCount() == bExpansions )
        //   this->dwScriptOffset = wJump;
        continue;
      }
    default:
      Broodwar << Text::Red << "Bad script opcode: " << bOpcode << std::endl;
      
      return;
    } // switch
  } // loop
}
