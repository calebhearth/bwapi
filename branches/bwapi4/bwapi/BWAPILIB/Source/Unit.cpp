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
                                         [&](Unit *u){ return this != u && this->getDistance(u) <= radius && (!pred || pred(u)); });
  }

  Unit *Unit::getClosestUnit(const UnitFilter &pred, int radius) const
  {
    // Return if this unit does not exist
    if ( !this->exists() )
      return NULL;
    
    return Broodwar->getClosestUnitInRectangle(this->getPosition(), 
                                                [&](Unit *u){ return this != u && this->getDistance(u) <= radius && (!pred || pred(u)); }, 
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
                                                ( !weapon.targetsAir()         && (!u->isLifted() && !ut.isFlyer()) ) ||
                                                ( !weapon.targetsGround()      && (u->isLifted() || ut.isFlyer())   ) ||
                                                ( weapon.targetsMechanical()   && ut.isMechanical()                 ) ||
                                                ( weapon.targetsOrganic()      && ut.isOrganic()                    ) ||
                                                ( weapon.targetsNonBuilding()  && !ut.isBuilding()                  ) ||
                                                ( weapon.targetsNonRobotic()   && !ut.isRobotic()                   ) ||
                                                ( weapon.targetsOrgOrMech()    && (ut.isOrganic() || ut.isMechanical()) ))  )
                                            return false;

                                          return pred(u);
                                        });
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

  bool Unit::isHoldingPosition() const
  {
    return this->getOrder() == Orders::HoldPosition;
  }

  bool Unit::isIrradiated() const
  {
    return this->getIrradiateTimer() != 0;
  }

  bool Unit::isLoaded() const
  {
    return this->getTransport() != NULL;
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
    if ( target.isUnit() && target.getUnit() == NULL )
      return this->issueCommand(UnitCommand::useTech(this,tech));
    return this->issueCommand(UnitCommand::useTech(this,tech,target));
  }
  //--------------------------------------------- PLACE COP --------------------------------------------------
  bool Unit::placeCOP(TilePosition target)
  {
    return this->issueCommand(UnitCommand::placeCOP(this, target));
  }
};
