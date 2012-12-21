#include "UnitInfo.h"

using namespace BWAPI;

// ----------------------------------------------- GETTERS -----------------------------------------
int GetOrderTimer(BWAPI::Unit *pUnit)
{ return pUnit->getClientInfo<int>(UnitInfo::OrderTimer); }

int GetOrderState(BWAPI::Unit *pUnit)
{ return pUnit->getClientInfo<int>(UnitInfo::OrderState); }

Orders::Enum::Enum GetUnitOrder(BWAPI::Unit *pUnit)
{ return pUnit->getClientInfo<Orders::Enum::Enum>(UnitInfo::Order); }

Orders::Enum::Enum GetQueuedOrder(BWAPI::Unit *pUnit)
{ return pUnit->getClientInfo<Orders::Enum::Enum>(UnitInfo::QueuedOrder); }

Unit *GetAttackTarget(BWAPI::Unit *pUnit)
{ return pUnit->getClientInfo<Unit*>(UnitInfo::AttackTarget); }

// ----------------------------------------------- SETTERS -----------------------------------------
void SetOrderState(BWAPI::Unit *pUnit, int state)
{ pUnit->setClientInfo(state, UnitInfo::OrderState);  }

void SetOrderTimer(BWAPI::Unit *pUnit, int state)
{ pUnit->setClientInfo(state, UnitInfo::OrderTimer);  }

void SetQueuedOrder(BWAPI::Unit *pUnit, Orders::Enum::Enum order)
{ pUnit->setClientInfo(order, UnitInfo::QueuedOrder); }

void SetUnitOrder(BWAPI::Unit *pUnit, Orders::Enum::Enum order)
{
  pUnit->setClientInfo( order, UnitInfo::Order);
  pUnit->setClientInfo( 0, UnitInfo::OrderTimer);
  pUnit->setClientInfo( 0, UnitInfo::OrderState);
}

void SetAttackTarget(BWAPI::Unit *pUnit, BWAPI::Unit *pTarget)
{ pUnit->setClientInfo(pTarget, UnitInfo::AttackTarget); }


