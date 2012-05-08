#include <BWAPI/Filters.h>
#include <BWAPI/Unit.h>
#include <BWAPI/UnitType.h>
#include <typeinfo>

namespace BWAPI
{
  const Filter Workers([](Unit *u){ return u->getType().isWorker(); });
  const Filter Larvae([](Unit *u){ return u->getType() == BWAPI::UnitTypes::Zerg_Larva; });
  const Filter Transports([](Unit *u){ return u->getType().spaceProvided() > 0; });
  const Filter ResourceDepots([](Unit *u){ return u->getType().isResourceDepot(); });
}

