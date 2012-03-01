#include "UnitProc.h"
#include "BWScriptEmulator.h"
#include "UType.h"
#include "OrderID.h"
#include "UnitID.h"

UnitProc *getUnitProc(BWAPI::Unit *pUnit)
{
  if ( !pUnit || pUnit->getPlayer() != self )
    return NULL;

  switch ( pUnit->getType() )
  {
  case UnitID::Terran_ScienceVessel:
    break;
  default:
    break;
  }
  return new UnitProc(pUnit);
}

UnitProc::UnitProc()
{ this->Init(); }

UnitProc::UnitProc(Unit *pUnit)
{ this->Init(pUnit); }

void UnitProc::Init()
{
  this->thisUnit        = NULL;
  this->guardLocation   = Positions::None;
  this->destLocation    = Positions::None;
  this->initialized     = false;
  this->dwState         = 0;
  this->iEmulatedOrder  = Orders::Guard;
  this->aiCaptain       = 0;
}
void UnitProc::Init(Unit *pUnit)
{
  this->thisUnit      = pUnit;
  this->guardLocation = pUnit ? pUnit->getPosition() : Positions::None;
  this->destLocation  = Positions::None;
  this->initialized   = false;
  this->dwState       = 0;
  this->aiCaptain     = 0;
}

UnitProc::~UnitProc()
{}

void UnitProc::EmulateIdleOrder()
{
  UType t = thisUnit->getType();
  if ( thisUnit->isIdle() )
    this->iEmulatedOrder = t.aiIdle();
  
  switch ( this->iEmulatedOrder )
  {
  case OrderID::AIPatrol:
    if ( thisUnit->isIdle() )
    {
      // Get the region
      BWAPI::Region *r = thisUnit->getRegion();
      std::vector<BWAPI::Region*> movetoRgnList;

      // iterate all regions neighboring this unit
      for ( auto i = r->getNeighbors().begin(); i != r->getNeighbors().end(); ++i )
      {
        // iterate all units within the bounds of the current region
        for ( Unitset::iterator u = self->getUnits().begin(), uend = self->getUnits().end(); u != uend; ++u )
        {
          UnitType ut = (*u)->getType();
          if ( !ut.isBuilding() )
            continue;

          // Save the region if its center is within sight range of a building
          if ( (*u)->getDistance( Position((*i)->getCenter()) ) <= ut.sightRange() )
          {
            movetoRgnList.push_back(*i);
            break;
          }
        }
      }
      // choose a random region to move to
      if ( !movetoRgnList.empty() )
        thisUnit->move(movetoRgnList[rand() % movetoRgnList.size()]->getCenter());
    }
    break;
  default:
    break;
  }
}

void UnitProc::StandardUnitProc()
{
  this->EmulateIdleOrder();
}

void UnitProc::execute()
{
  switch ( this->aiCaptain )
  {
  case 0:
    this->StandardUnitProc();
    break;
  }
}
