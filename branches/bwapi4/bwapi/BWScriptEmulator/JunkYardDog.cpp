#include "OrderEmulate.h"
#include "UnitInfo.h"

using namespace BWAPI;

// random number within [min,max]
int randBetween(int min, int max)
{
  return (rand()%((max-min)+1)) + min;
}

void RunJunkYardDog(BWAPI::Unit *pUnit)
{
  if ( GetOrderState(pUnit) == 0 )
  {
    // Retrieve a random target position
    BWAPI::Position targ = directionPositions[ randBetween(0,255) ];
    BWAPI::Position p1, p2, p3, p4;

    // Modify the target position to be a random larger vector
    int distance = randBetween(1024,2048);
    targ *= distance;
    targ /= 256;
    
    // Add the unit's position and make the position valid
    targ += pUnit->getPosition();
    targ.makeValid();

    // Issue move order and set the order state to 1
    pUnit->move(targ);
    SetOrderState(pUnit, 1);
  }
  else
  {
    BWAPI::Unit *pAutoTarget = GetAttackTarget(pUnit);
    if ( pAutoTarget == nullptr ||
      !Broodwar->self()->isEnemy(pAutoTarget->getPlayer())  ||
      !pAutoTarget->isVisible() ||
      pAutoTarget->isInvincible() ||
      !pAutoTarget->isDetected() )
    {
      if ( GetOrderTimer(pUnit) == 0 )
      {
        SetOrderTimer(pUnit, 15);
        if ( ( pUnit->getTargetPosition() != pUnit->getPosition() /* || !unmovable */ ) /*&&
               (pUnit->getSpellCooldown() != 0 || (pUnit->getType().isFlyer() || pUnit->isLifted()) )*/  )
        {
          pAutoTarget = FindNewAttackTarget(pUnit); // Look for new closest target!
          if ( pAutoTarget != nullptr )
          {
            if ( pUnit->attack(pAutoTarget) )
              SetUnitOrder(pUnit, Orders::Enum::Guard);
            // queue Junk Yard Dog
            //pUnit->holdPosition();
          }
        }
        else
        {
          SetOrderState(pUnit, 0);
        }
      } // if order timer^
    }
    else
    {
      if ( pUnit->attack(pAutoTarget) )
        SetUnitOrder(pUnit, Orders::Enum::Guard);
      // queue Junk Yard Dog
      SetAttackTarget(pUnit, nullptr);
      //pUnit->holdPosition();
    }
  }

}
