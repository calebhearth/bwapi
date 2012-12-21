#include "OrderEmulate.h"
#include "ScriptEnum.h"
#include "UnitInfo.h"

using namespace BWAPI;

void UpdateOrderTimers(BWAPI::Unit *pUnit)
{
  int timer;

  // Order timer
  timer = GetOrderTimer(pUnit);
  if ( timer > 0 )
    SetOrderTimer(pUnit, timer-1);

}

void EmulateOrder(BWAPI::Unit *pUnit)
{
  UpdateOrderTimers(pUnit);

  // Set to desired idle order if assigned order is not set
  if ( pUnit->isIdle() && GetUnitOrder(pUnit) == 0 )
    SetUnitOrder(pUnit, AISCRIPT::getUnitIdleOrder(pUnit->getType()));

  switch ( GetUnitOrder(pUnit) )
  {
  case Orders::Enum::JunkYardDog:
    RunJunkYardDog(pUnit);
    break;
  }
}

BWAPI::Unit *FindNewAttackTarget(BWAPI::Unit *pUnit)
{
  UnitType t = pUnit->getType();
  
  Unit *pTarg = pUnit->getClosestUnit( IsEnemy && 
                                      [pUnit](Unit *u){ return pUnit->isInWeaponRange(u);} &&
                                      IsVisible &&
                                      !IsInvincible,  std::max(t.seekRange(),t.sightRange())*32 );
  // Should be greatly expanded, but for now something simple
  return pTarg;
}
