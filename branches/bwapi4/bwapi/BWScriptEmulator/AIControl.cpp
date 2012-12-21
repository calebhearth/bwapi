#include "AIControl.h"
#include "UnitInfo.h"

using namespace BWAPI;

void PerformSpecializedUnitControl(BWAPI::Unit *pUnit)
{
  // safety
  if ( !pUnit ) return;

  switch ( pUnit->getClientInfo<int>(UnitInfo::AI_Controller_Type) )
  {
  case 0: // None/Unassigned
    break;
  case 1: // Tracker?
    break;
  case 2: // Worker
    break;
  case 3: // Task
    break;
  case 4: // Military
    break;
  default:
    break;
  }
}

bool HasEnemyTarget(BWAPI::Unit *pUnit)
{
  // Get the target
  BWAPI::Unit *pTarg = pUnit->getOrderTarget();
  if ( !pTarg )
    return false;

  // Ignore if not an enemy
  if ( !Broodwar->self()->isEnemy(pTarg->getPlayer()) )
    return false;

  // Ignore if damaged hallucination
  if ( pTarg->isHallucination() && (HP+Shields)(pTarg) != (MaxHP+MaxShields)(pTarg) )
    return false;
  
  // Ignore if it can be attacked and out of weapon range
  if ( pUnit->canIssueCommand( UnitCommand::attack(pUnit, pTarg)) )
    return pUnit->isInWeaponRange(pTarg);

  // Ignore if it's an unloaded bunker
  return pTarg->getType() == UnitTypes::Terran_Bunker && !pTarg->getLoadedUnits().empty();
}

void DecloakCheck(BWAPI::Unit *pUnit)
{
  // Ignore if not cloaked/cloakable
  if ( !pUnit->isCloaked() || !pUnit->getType().isCloakable() )
    return;

  // Ignore if busy with enemy
  if ( HasEnemyTarget(pUnit) )
    return;

  // Ignore if auto-targetting something?
  //if ( pUnit->AIOrderTargetUnit ) 
  //  return;

  // Ignore if there's a nearby enemy unit
  if ( pUnit->getClosestUnit(IsEnemy && CanAttack, 256) )
    return;

  if ( pUnit->getOrder() != Orders::NukeWait &&
       pUnit->getOrder() != Orders::NukePaint &&
       pUnit->getOrder() != Orders::NukeTrack )
    pUnit->decloak();
}

bool DoUnitBehaviour(BWAPI::Unit *pUnit)
{
  DecloakCheck(pUnit);
  
  UnitType t = pUnit->getType();
  switch ( t )
  {
  case UnitTypes::Enum::Protoss_Carrier:
  case UnitTypes::Enum::Protoss_Reaver:
    // @TODO: Check AI reserves
    pUnit->train();
    return false;
  case UnitTypes::Enum::Terran_Dropship:
    if ( pUnit->getHitPoints() < t.maxHitPoints() ) //&& AssignRepairSCV(pUnit) )
    {
      Unit *pSCV = nullptr;//AssignRepairSCV(pUnit);
      if ( pSCV )   // do unload, then follow SCV
      {
        if ( pUnit->getLoadedUnits().empty() )
          return pUnit->follow(pSCV);
        else
          return pUnit->unloadAll();
      }
    }
    // do what protoss shuttle does after:
  case UnitTypes::Enum::Protoss_Shuttle:
    //if ( AI_TranpoerAction(pUnit) )
    //  return true;
    if ( pUnit->getLoadedUnits().empty() )
      return false;
    return pUnit->unloadAll();
  case UnitTypes::Enum::Zerg_Overlord:
    if ( pUnit->getLoadedUnits().empty() )
      return false;
    return pUnit->unloadAll();
  case UnitTypes::Enum::Terran_Vulture:
    return false;//pUnit->useTech(TechTypes::Spider_Mines, GetSpiderMineLocation(pUnit));
  case UnitTypes::Enum::Terran_Siege_Tank_Tank_Mode:
    // @TODO
    return false;
  case UnitTypes::Enum::Protoss_Shield_Battery:
    if ( pUnit->getEnergy() >= 50 )
    {
      Unitset myUnits(Broodwar->self()->getUnits());
      for ( auto i = myUnits.begin(); i != myUnits.end(); ++i )
      {
        // @TODO
        // if ( i can recharge, not attacking, shields > 40 )
        //  recharge the unit
      }
    }
    return false;
  case UnitTypes::Enum::Protoss_Arbiter:
    // Should be "AIOrderTargetUnit" or autotarget
    Unit *pBest = pUnit->getClosestUnit(GetPlayer == Broodwar->self() && !IsBuilding && OrderTarget != nullptr, 640);
    if ( pBest != nullptr )
      return pUnit->attack(pBest->getPosition());
    return false;
  }
  return false;
}

