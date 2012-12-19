#include "Military.h"
using namespace BWAPI;

bool TaskBunkerDefender(BWAPI::Unit *pUnit) // @TODO
{
  return false;
}

bool TaskSpellcaster(BWAPI::Unit *pUnit) // @TODO
{
  return false;
}

bool TaskAggression(BWAPI::Unit *pUnit) // @TODO
{
  return false;
}


void RunMilitaryController(BWAPI::Unit *pUnit)
{
  // Run bunker tasks
  if ( (pUnit->getType() == UnitTypes::Terran_Marine || pUnit->getType() == UnitTypes::Terran_Firebat) &&
        TaskBunkerDefender(pUnit) )
    return;

  // Run spellcaster tasks
  if ( TaskSpellcaster(pUnit) )
    return;

  // Run aggressive unit tasks (large)
  if ( TaskAggression(pUnit) )
    return;

  // @TODO
  Region *targetRgn = nullptr;
  // Get targetRgn from captain destination or slowest unit in group
  
  if ( pUnit->getRegion() == targetRgn )
    pUnit->holdPosition();
  else
    pUnit->attack(targetRgn->getCenter());
}
