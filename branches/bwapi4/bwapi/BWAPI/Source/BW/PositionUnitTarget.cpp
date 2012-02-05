#include "PositionUnitTarget.h"
#include "Unit.h"

#include "../../../Debug.h"

namespace BW
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  PositionUnitTarget::PositionUnitTarget(const Position& position)
      : position(position)
  {
  }
  PositionUnitTarget::PositionUnitTarget(int x, int y)
      : position((u16)x,(u16)y)
  {
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  PositionUnitTarget::PositionUnitTarget(const UnitTarget& target)
      : target(target)
  {
    int index = 336 * ((target.getTarget() & 0x7FF) - 1);
    Unit* unit = (Unit*)((int)BWDATA_UnitNodeTable + index);
    this->position = unit->position;
  }
  //----------------------------------------------------------------------------------------------------------
}

