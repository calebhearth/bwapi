#include <BWAPI/Region.h>
#include <BWAPI/Unitset.h>
#include <BWAPI/Game.h>
#include <BWAPI/Unit.h>

namespace BWAPI
{
  int Region::getDistance(BWAPI::Region *other) const
  {
    return this->getCenter().getApproxDistance(other->getCenter());
  }

  Unitset Region::getUnits(const Unit::Filter &pred) const
  {
    return Broodwar->getUnitsInRectangle(this->getBoundsLeft(),
                                         this->getBoundsTop(),
                                         this->getBoundsRight(),
                                         this->getBoundsBottom(),
                                         [&](Unit *u){ return u->getRegion() == this && (!pred || pred(u)); });
  }
}
