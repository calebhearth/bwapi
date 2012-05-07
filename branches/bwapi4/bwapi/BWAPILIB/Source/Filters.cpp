#include <BWAPI/Filters.h>
#include <functional>
#include <BWAPI/Unit.h>

namespace BWAPI
{
  Filter Workers         = [](Unit *u){ return u->getType().isWorker(); };
  Filter Transports      = [](Unit *u){ return u->getType().spaceProvided() > 0; };
  Filter ResourceDepots  = [](Unit *u){ return u->getType().isResourceDepot(); };

}
/*
BWAPI::Filter operator&&(const BWAPI::Filter &p1, const BWAPI::Filter &p2)
{
  return [&](BWAPI::Unit *u){ return p1(u) && p2(u); };
}

BWAPI::Filter operator||(const BWAPI::Filter &p1, const BWAPI::Filter &p2)
{
  return [&](BWAPI::Unit *u){ return p1(u) || p2(u); };
}

BWAPI::Filter operator~(const BWAPI::Filter &p1)
{
  return [&](BWAPI::Unit *u){ return !p1(u); };
}

*/