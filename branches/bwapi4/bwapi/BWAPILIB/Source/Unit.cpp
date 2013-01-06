#include <BWAPI/Unit.h>
#include <BWAPI/Order.h>
#include <BWAPI/UnitCommand.h>
#include <BWAPI/TechType.h>
#include <BWAPI/UpgradeType.h>
#include <BWAPI/Unitset.h>
#include <BWAPI/Game.h>
#include <BWAPI/WeaponType.h>
#include <BWAPI/Player.h>
#include <BWAPI/Filters.h>
#include <BWAPI/Flag.h>

#include <limits>

namespace BWAPI
{
  //------------------------------------------------ GET UNITS IN RADIUS -------------------------------------
  Unitset Unit::getUnitsInRadius(int radius, const UnitFilter &pred) const
  {
    // Return if this unit does not exist
    if ( !this->exists() )
      return Unitset::none;

    return Broodwar->getUnitsInRectangle(this->getLeft()   - radius,
                                         this->getTop()    - radius,
                                         this->getRight()  + radius,
                                         this->getBottom() + radius,
                                         [&](Unit *u){ return this != u && this->getDistance(u) <= radius && (!pred.isValid() || pred(u)); });
  }

  Unit *Unit::getClosestUnit(const UnitFilter &pred, int radius) const
  {
    // Return if this unit does not exist
    if ( !this->exists() )
      return nullptr;
    
    return Broodwar->getClosestUnitInRectangle(this->getPosition(), 
                                                [&](Unit *u){ return this != u && this->getDistance(u) <= radius && (!pred.isValid() || pred(u)); }, 
                                                this->getLeft()   - radius,
                                                this->getTop()    - radius,
                                                this->getRight()  + radius,
                                                this->getBottom() + radius);
  }
  //--------------------------------------------- GET UNITS IN WEAPON RANGE ----------------------------------
  Unitset Unit::getUnitsInWeaponRange(WeaponType weapon, const UnitFilter &pred) const
  {
    // Return if this unit does not exist
    if ( !this->exists() )
      return Unitset::none;

    int max = this->getPlayer()->weaponMaxRange(weapon);

    return Broodwar->getUnitsInRectangle(this->getLeft()    - max,
                                         this->getTop()     - max,
                                         this->getRight()   + max,
                                         this->getBottom()  + max,
                                         [&](Unit *u)->bool
                                         {
                                          // Unit check and unit status
                                          if ( u == this || u->isInvincible() )
                                            return false;

                                          // Weapon distance check
                                          int dist = this->getDistance(u);
                                          if ( (weapon.minRange() && dist < weapon.minRange()) || dist > max )
                                            return false;

                                          // Weapon behavioural checks
                                          UnitType ut = u->getType();
                                          if ( (( weapon.targetsOwn()          && u->getPlayer() != this->getPlayer() ) ||
                                                ( !weapon.targetsAir()         && !u->isFlying() ) ||
                                                ( !weapon.targetsGround()      && u->isFlying()  ) ||
                                                ( weapon.targetsMechanical()   && ut.isMechanical()                 ) ||
                                                ( weapon.targetsOrganic()      && ut.isOrganic()                    ) ||
                                                ( weapon.targetsNonBuilding()  && !ut.isBuilding()                  ) ||
                                                ( weapon.targetsNonRobotic()   && !ut.isRobotic()                   ) ||
                                                ( weapon.targetsOrgOrMech()    && (ut.isOrganic() || ut.isMechanical()) ))  )
                                            return false;

                                          return pred(u);
                                        });
  }
  //--------------------------------------------- GET TILE POSITION ------------------------------------------
  TilePosition Unit::getTilePosition() const
  {
    return TilePosition( Position(abs(this->getPosition().x - this->getType().tileWidth()  * 32 / 2),
                                  abs(this->getPosition().y - this->getType().tileHeight() * 32 / 2)) );
  }
  //--------------------------------------------- GET DISTANCE -----------------------------------------------
  int Unit::getDistance(PositionOrUnit target) const
  {
    // If this unit does not exist
    if ( !exists() )
      return std::numeric_limits<int>::max();

    // Must be something valid
    if ( !target.getUnit() && !target.getPosition() )
      return std::numeric_limits<int>::max();

    // if target is a unit but doesn't exist
    if ( target.getUnit() && !target.getUnit()->exists() )
      return std::numeric_limits<int>::max();

    // If target is the same as the source
    if ( this == target.getUnit() )
      return std::numeric_limits<int>::max();

    /////// Compute distance

    // retrieve left/top/right/bottom values for calculations
    int left, right, top, bottom;

    if ( target.isPosition() )
    {
      // Retrieve the target position if it is so
      Position targPos = target.getPosition();
      left    = targPos.x - 1;
      top     = targPos.y - 1;
      right   = targPos.x + 1;
      bottom  = targPos.y + 1;
    }
    else
    {
      // Retrieve the target unit if it's not a position
      Unit *pUnit = target.getUnit();

      // return if target is invalid
      if ( !pUnit || pUnit == this )
        return 0;

      left    = pUnit->getLeft() - 1;
      top     = pUnit->getTop() - 1;
      right   = pUnit->getRight() + 1;
      bottom  = pUnit->getBottom() + 1;
    }

    // compute x distance
    int xDist = this->getLeft() - right;
    if ( xDist < 0 )
    {
      xDist = left - this->getRight();
      if ( xDist < 0 )
        xDist = 0;
    }

    // compute y distance
    int yDist = this->getTop() - bottom;
    if ( yDist < 0 )
    {
      yDist = top - this->getBottom();
      if ( yDist < 0 )
        yDist = 0;
    }

    // compute actual distance
    return Positions::Origin.getApproxDistance(Position(xDist, yDist));
  }
  //--------------------------------------------- HAS PATH ---------------------------------------------------
  bool Unit::hasPath(PositionOrUnit target) const
  {
    Broodwar->setLastError();
    // Return error if the position is invalid
    if ( !target.getPosition() )
      return Broodwar->setLastError(Errors::Invalid_Parameter);

    // Return true if this unit is an air unit
    if ( this->isFlying() )
      return true;

    // Return error if either this or the target does not "exist"
    if ( (target.getUnit() && !target.getUnit()->exists()) || !exists() )
      return Broodwar->setLastError(Errors::Unit_Not_Visible);

    // return result of Game::hasPath
    return Broodwar->hasPath(this->getPosition(), target.getPosition());
  }
  //--------------------------------------------- GET REGION -------------------------------------------------
  BWAPI::Region *Unit::getRegion() const
  {
    return Broodwar->getRegionAt(this->getPosition());
  }
  //--------------------------------------------- GET LEFT ---------------------------------------------------
  int Unit::getLeft() const
  {
    return this->getPosition().x - this->getType().dimensionLeft();
  }
  //--------------------------------------------- GET TOP ----------------------------------------------------
  int Unit::getTop() const
  {
    return this->getPosition().y - this->getType().dimensionUp();
  }
  //--------------------------------------------- GET RIGHT --------------------------------------------------
  int Unit::getRight() const
  {
    return this->getPosition().x + this->getType().dimensionRight();
  }
  //--------------------------------------------- GET BOTTOM -------------------------------------------------
  int Unit::getBottom() const
  {
    return this->getPosition().y + this->getType().dimensionDown();
  }
  //--------------------------------------------- IS BEING CONSTRUCTED ---------------------------------------
  bool Unit::isBeingConstructed() const
  {
    if ( this->isMorphing() )
      return true;
    if ( this->isCompleted() )
      return false;
    if ( this->getType().getRace() != Races::Terran )
      return true;
    return this->getBuildUnit() != nullptr;
  }
  // ------------------------------------------ STATUS ---------------------------------------------
  bool Unit::isDefenseMatrixed() const
  {
    return this->getDefenseMatrixTimer() != 0;
  }

  bool Unit::isEnsnared() const
  {
    return this->getEnsnareTimer() != 0;
  }

  bool Unit::isFollowing() const
  {
    return this->getOrder() == Orders::Follow;
  }
  bool Unit::isFlying() const
  {
    return this->getType().isFlyer() || this->isLifted();
  }

  bool Unit::isHoldingPosition() const
  {
    return this->getOrder() == Orders::HoldPosition;
  }

  //--------------------------------------------- IS IN WEAPON RANGE -----------------------------------------
  bool Unit::isInWeaponRange(Unit *target) const
  {
    // Preliminary checks
    if ( !this->exists() || !target || !target->exists() || this == target )
      return false;

    // Store the types as locals
    UnitType thisType = getType();
    UnitType targType = target->getType();

    // Obtain the weapon type
    WeaponType wpn = target->isFlying() ? thisType.airWeapon() : thisType.groundWeapon();

    // Return if there is no weapon type
    if ( wpn == WeaponTypes::None || wpn == WeaponTypes::Unknown )
      return false;

    // Retrieve the min and max weapon ranges
    int minRange = wpn.minRange();
    int maxRange = getPlayer()->weaponMaxRange(wpn);

    // Check if the distance to the unit is within the weapon range
    int distance = this->getDistance(target);
    return (minRange ? minRange < distance : true) && distance <= maxRange;
  }

  bool Unit::isIrradiated() const
  {
    return this->getIrradiateTimer() != 0;
  }

  bool Unit::isLoaded() const
  {
    return this->getTransport() != nullptr;
  }

  bool Unit::isLockedDown() const
  {
    return this->getLockdownTimer() != 0;
  }

  bool Unit::isMaelstrommed() const
  {
    return this->getMaelstromTimer() != 0;
  }

  bool Unit::isPatrolling() const
  {
    return this->getOrder() == Orders::Patrol;
  }

  bool Unit::isPlagued() const
  {
    return this->getPlagueTimer() != 0;
  }

  bool Unit::isRepairing() const
  {
    return this->getOrder() == Orders::Repair;
  }

  bool Unit::isResearching() const
  {
    return this->getOrder() == Orders::ResearchTech;
  }

  bool Unit::isSieged() const
  {
    UnitType t(this->getType());
    return  t == UnitTypes::Terran_Siege_Tank_Siege_Mode || 
            t == UnitTypes::Hero_Edmund_Duke_Siege_Mode;
  }

  bool Unit::isStasised() const
  {
    return this->getStasisTimer() != 0;
  }

  bool Unit::isStimmed() const
  {
    return this->getStimTimer() != 0;
  }

  bool Unit::isUpgrading() const
  {
    return this->getOrder() == Orders::Upgrade;
  }

  int Unit::getSpaceRemaining() const
  {
    // Get the space that the current Unit type provides
    int space = this->getType().spaceProvided();

    // Obtain the set of loaded units
    Unitset loaded(this->getLoadedUnits());

    // Decrease the space for each loaded unit
    for ( auto u = loaded.begin(); u != loaded.end(); ++u )
      space -= u->getType().spaceRequired();

    // Return the space available
    return std::max(space, 0);
  }
  //--------------------------------------------- ATTACK MOVE ------------------------------------------------
  bool Unit::attack(PositionOrUnit target, bool shiftQueueCommand)
  {
    return this->issueCommand(UnitCommand::attack(this, target, shiftQueueCommand));
  }
  //--------------------------------------------- BUILD ------------------------------------------------------
  bool Unit::build(UnitType type, TilePosition target)
  {
    if ( target == TilePositions::None )
      return this->train(type);
    return this->issueCommand(UnitCommand::build(this, target, type));
  }
  //--------------------------------------------- BUILD ADDON ------------------------------------------------
  bool Unit::buildAddon(UnitType type)
  {
    return this->issueCommand(UnitCommand::buildAddon(this,type));
  }
  //--------------------------------------------- TRAIN ------------------------------------------------------
  bool Unit::train(UnitType type)
  {
    // Carrier/Reaver specialization
    if ( type == UnitTypes::None )
    {
      switch ( this->getType() )
      {
      case UnitTypes::Enum::Protoss_Carrier:
      case UnitTypes::Enum::Hero_Gantrithor:
        type = UnitTypes::Protoss_Interceptor;
        break;
      case UnitTypes::Enum::Protoss_Reaver:
      case UnitTypes::Enum::Hero_Warbringer:
        type = UnitTypes::Protoss_Scarab;
        break;
      }
    }
    return this->issueCommand(UnitCommand::train(this,type));
  }
  //--------------------------------------------- MORPH ------------------------------------------------------
  bool Unit::morph(UnitType type)
  {
    return this->issueCommand(UnitCommand::morph(this,type));
  }
  //--------------------------------------------- RESEARCH ---------------------------------------------------
  bool Unit::research(TechType tech)
  {
    return this->issueCommand(UnitCommand::research(this,tech));
  }
  //--------------------------------------------- UPGRADE ----------------------------------------------------
  bool Unit::upgrade(UpgradeType upgrade)
  {
    return this->issueCommand(UnitCommand::upgrade(this,upgrade));
  }
  //--------------------------------------------- SET RALLY POSITION -----------------------------------------
  bool Unit::setRallyPoint(PositionOrUnit target)
  {
    return this->issueCommand( UnitCommand::setRallyPoint(this, target) );
  }
  //--------------------------------------------- MOVE -------------------------------------------------------
  bool Unit::move(Position target, bool shiftQueueCommand)
  {
    return this->issueCommand(UnitCommand::move(this,target, shiftQueueCommand));
  }
  //--------------------------------------------- PATROL -----------------------------------------------------
  bool Unit::patrol(Position target, bool shiftQueueCommand)
  {
    return this->issueCommand(UnitCommand::patrol(this,target, shiftQueueCommand));
  }
  //--------------------------------------------- HOLD POSITION ----------------------------------------------
  bool Unit::holdPosition(bool shiftQueueCommand)
  {
    return this->issueCommand(UnitCommand::holdPosition(this, shiftQueueCommand));
  }
  //--------------------------------------------- STOP -------------------------------------------------------
  bool Unit::stop(bool shiftQueueCommand)
  {
    return this->issueCommand(UnitCommand::stop(this, shiftQueueCommand));
  }
  //--------------------------------------------- FOLLOW -----------------------------------------------------
  bool Unit::follow(Unit* target, bool shiftQueueCommand)
  {
    return this->issueCommand(UnitCommand::follow(this,target, shiftQueueCommand));
  }
  //--------------------------------------------- GATHER -----------------------------------------------------
  bool Unit::gather(Unit* target, bool shiftQueueCommand)
  {
    return this->issueCommand(UnitCommand::gather(this,target, shiftQueueCommand));
  }
  //--------------------------------------------- RETURN CARGO -----------------------------------------------
  bool Unit::returnCargo(bool shiftQueueCommand)
  {
    return this->issueCommand(UnitCommand::returnCargo(this, shiftQueueCommand));
  }
  //--------------------------------------------- REPAIR -----------------------------------------------------
  bool Unit::repair(Unit* target, bool shiftQueueCommand)
  {
    return this->issueCommand(UnitCommand::repair(this,target, shiftQueueCommand));
  }
  //--------------------------------------------- BURROW -----------------------------------------------------
  bool Unit::burrow()
  {
    return this->issueCommand(UnitCommand::burrow(this));
  }
  //--------------------------------------------- UNBURROW ---------------------------------------------------
  bool Unit::unburrow()
  {
    return this->issueCommand(UnitCommand::unburrow(this));
  }
  //--------------------------------------------- CLOAK ------------------------------------------------------
  bool Unit::cloak()
  {
    return this->issueCommand(UnitCommand::cloak(this));
  }
  //--------------------------------------------- DECLOAK ----------------------------------------------------
  bool Unit::decloak()
  {
    return this->issueCommand(UnitCommand::decloak(this));
  }
  //--------------------------------------------- SIEGE ------------------------------------------------------
  bool Unit::siege()
  {
    return this->issueCommand(UnitCommand::siege(this));
  }
  //--------------------------------------------- UNSIEGE ----------------------------------------------------
  bool Unit::unsiege()
  {
    return this->issueCommand(UnitCommand::unsiege(this));
  }
  //--------------------------------------------- LIFT -------------------------------------------------------
  bool Unit::lift()
  {
    return this->issueCommand(UnitCommand::lift(this));
  }
  //--------------------------------------------- LAND -------------------------------------------------------
  bool Unit::land(TilePosition target)
  {
    return this->issueCommand(UnitCommand::land(this,target));
  }
  //--------------------------------------------- LOAD -------------------------------------------------------
  bool Unit::load(Unit* target, bool shiftQueueCommand)
  {
    return this->issueCommand(UnitCommand::load(this,target, shiftQueueCommand));
  }
  //--------------------------------------------- UNLOAD -----------------------------------------------------
  bool Unit::unload(Unit* target)
  {
    return this->issueCommand(UnitCommand::unload(this,target));
  }
  //--------------------------------------------- UNLOAD ALL -------------------------------------------------
  bool Unit::unloadAll(bool shiftQueueCommand)
  {
    return this->issueCommand(UnitCommand::unloadAll(this, shiftQueueCommand));
  }
  //--------------------------------------------- UNLOAD ALL -------------------------------------------------
  bool Unit::unloadAll(Position target, bool shiftQueueCommand)
  {
    return this->issueCommand(UnitCommand::unloadAll(this,target, shiftQueueCommand));
  }
  //--------------------------------------------- RIGHT CLICK ------------------------------------------------
  bool Unit::rightClick(PositionOrUnit target, bool shiftQueueCommand)
  {
    return this->issueCommand(UnitCommand::rightClick(this,target, shiftQueueCommand));
  }
  //--------------------------------------------- HALT CONSTRUCTION ------------------------------------------
  bool Unit::haltConstruction()
  {
    return this->issueCommand(UnitCommand::haltConstruction(this));
  }
  //--------------------------------------------- CANCEL CONSTRUCTION ----------------------------------------
  bool Unit::cancelConstruction()
  {
    return this->issueCommand(UnitCommand::cancelConstruction(this));
  }
  //--------------------------------------------- CANCEL ADDON -----------------------------------------------
  bool Unit::cancelAddon()
  {
    return this->issueCommand(UnitCommand::cancelAddon(this));
  }
  //--------------------------------------------- CANCEL TRAIN -----------------------------------------------
  bool Unit::cancelTrain(int slot)
  {
    return this->issueCommand(UnitCommand::cancelTrain(this, slot));
  }
  //--------------------------------------------- CANCEL MORPH -----------------------------------------------
  bool Unit::cancelMorph()
  {
    return this->issueCommand(UnitCommand::cancelMorph(this));
  }
  //--------------------------------------------- CANCEL RESEARCH --------------------------------------------
  bool Unit::cancelResearch()
  {
    return this->issueCommand(UnitCommand::cancelResearch(this));
  }
  //--------------------------------------------- CANCEL UPGRADE ---------------------------------------------
  bool Unit::cancelUpgrade()
  {
    return this->issueCommand(UnitCommand::cancelUpgrade(this));
  }
  //--------------------------------------------- USE TECH ---------------------------------------------------
  bool Unit::useTech(TechType tech, PositionOrUnit target)
  {
    if ( target.isUnit() && target.getUnit() == nullptr )
      return this->issueCommand(UnitCommand::useTech(this,tech));
    return this->issueCommand(UnitCommand::useTech(this,tech,target));
  }
  //--------------------------------------------- PLACE COP --------------------------------------------------
  bool Unit::placeCOP(TilePosition target)
  {
    return this->issueCommand(UnitCommand::placeCOP(this, target));
  }
};
