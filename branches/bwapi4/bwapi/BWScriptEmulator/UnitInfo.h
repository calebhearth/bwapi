#pragma once
#include <BWAPI.h>

int GetOrderTimer(BWAPI::Unit *pUnit);
int GetOrderState(BWAPI::Unit *pUnit);
BWAPI::Orders::Enum::Enum GetUnitOrder(BWAPI::Unit *pUnit);
BWAPI::Orders::Enum::Enum GetQueuedOrder(BWAPI::Unit *pUnit);
BWAPI::Unit *GetAttackTarget(BWAPI::Unit *pUnit);

void SetOrderState(BWAPI::Unit *pUnit, int state);
void SetOrderTimer(BWAPI::Unit *pUnit, int state);
void SetUnitOrder(BWAPI::Unit *pUnit, BWAPI::Orders::Enum::Enum order);
void SetQueuedOrder(BWAPI::Unit *pUnit, BWAPI::Orders::Enum::Enum order);
void SetAttackTarget(BWAPI::Unit *pUnit, BWAPI::Unit *pTarget);

namespace UnitInfo
{
  enum Enum
  {
    Nothing,

    AI_Controller_Type,
    AI_Controller_Timer,

    Town,

    Order,
    OrderState,
    OrderTimer,
    QueuedOrder,
    AttackTarget
  };

}
