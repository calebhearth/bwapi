#include <BWAPI/Filters.h>
#include <BWAPI/Unit.h>
#include <BWAPI/UnitType.h>
#include <BWAPI/Player.h>

#define ULAMBDA(x) [](Unit *u){ return (x); }

#define U_UFILTER(n,x) const UnaryFilterBase<Unit*> n( ULAMBDA(x) )
#define U_CFILTER(n,x) const CompareFilterBase<Unit*> n( ULAMBDA(x) )

namespace BWAPI
{
  U_UFILTER(Workers, u->getType().isWorker() );
  U_UFILTER(Transports, u->getType().spaceProvided() > 0 );
  U_UFILTER(ResourceDepots, u->getType().isResourceDepot() );

  U_CFILTER(HP, u->getHitPoints() );
  U_CFILTER(HP_Percent, (u->getHitPoints()*100) / u->getType().maxHitPoints() );
  U_CFILTER(Shields, u->getShields() );
  U_CFILTER(Shields_Percent, (u->getShields()*100) / u->getType().maxShields() );
  U_CFILTER(Energy, u->getEnergy() );
  U_CFILTER(Energy_Percent, (u->getEnergy()*100) / u->getPlayer()->maxEnergy(u->getType()) );

  U_CFILTER(GetType, u->getType() );

  U_CFILTER(Resources, u->getResources() );
}

