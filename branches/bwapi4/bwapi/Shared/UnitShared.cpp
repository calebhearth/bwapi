#include <BWAPI.h>
#include <BWAPI/Client/UnitData.h>
#include <TemplatesImpl.h>
#include <iterator>
#include <set>

#ifndef MAXINT
#include <limits>
#define MAXINT std::numeric_limits<int>::max()
#endif

namespace BWAPI
{
  //--------------------------------------------- CLEAR ------------------------------------------------------
  void UnitImpl::clear()
  {
    initialType      = UnitTypes::None;
    initialResources = 0;
    initialHitPoints = 0;
    initialPosition  = Positions::None;
    lastCommandFrame = 0;
    lastCommand      = UnitCommand();
    clientInfo       = NULL;
    connectedUnits.clear();
  }
  //------------------------------------- INITIAL INFORMATION FUNCTIONS --------------------------------------
  void UnitImpl::saveInitialState()
  {
    initialType      = getType();
    initialPosition  = getPosition();
    initialResources = getResources();
    initialHitPoints = getHitPoints();
  }
  //--------------------------------------------- GET ID -----------------------------------------------------
  int UnitImpl::getID() const
  {
    return id;
  }
  //--------------------------------------------- GET REPLAY ID ----------------------------------------------
  int UnitImpl::getReplayID() const
  {
    return self->replayID;
  }
  //--------------------------------------------- GET PLAYER -------------------------------------------------
  Player* UnitImpl::getPlayer() const
  {
    return Broodwar->getPlayer(self->player);
  }
  //--------------------------------------------- GET TYPE ---------------------------------------------------
  UnitType UnitImpl::getType() const
  {
    return UnitType(self->type);
  }
  //--------------------------------------------- GET POSITION -----------------------------------------------
  Position UnitImpl::getPosition() const
  {
    return Position(self->positionX, self->positionY);
  }
  //--------------------------------------------- GET TILE POSITION ------------------------------------------
  TilePosition UnitImpl::getTilePosition() const
  {
    return TilePosition(Position(abs(self->positionX - getType().tileWidth()  * TILE_SIZE / 2),
                                 abs(self->positionY - getType().tileHeight() * TILE_SIZE / 2)) );
  }
  //--------------------------------------------- GET ANGLE --------------------------------------------------
  double UnitImpl::getAngle() const
  {
    return self->angle;
  }
  //--------------------------------------------- GET VELOCITY X ---------------------------------------------
  double UnitImpl::getVelocityX() const
  {
    return self->velocityX;
  }
  //--------------------------------------------- GET VELOCITY Y ---------------------------------------------
  double UnitImpl::getVelocityY() const
  {
    return self->velocityY;
  }
  //--------------------------------------------- GET REGION -------------------------------------------------
  BWAPI::Region *UnitImpl::getRegion() const
  {
    return Broodwar->getRegionAt(getPosition());
  }
  //--------------------------------------------- GET LEFT ---------------------------------------------------
  int UnitImpl::getLeft() const
  {
    return self->positionX - UnitType(self->type).dimensionLeft();
  }
  //--------------------------------------------- GET TOP ----------------------------------------------------
  int UnitImpl::getTop() const
  {
    return self->positionY - UnitType(self->type).dimensionUp();
  }
  //--------------------------------------------- GET RIGHT --------------------------------------------------
  int UnitImpl::getRight() const
  {
    return self->positionX + UnitType(self->type).dimensionRight();
  }
  //--------------------------------------------- GET BOTTOM -------------------------------------------------
  int UnitImpl::getBottom() const
  {
    return self->positionY + UnitType(self->type).dimensionDown();
  }
  //--------------------------------------------- GET HIT POINTS ---------------------------------------------
  int UnitImpl::getHitPoints() const
  {
    return self->hitPoints;
  }
  //--------------------------------------------- GET SHIELDS ------------------------------------------------
  int UnitImpl::getShields() const
  {
    return self->shields;
  }
  //--------------------------------------------- GET ENERGY -------------------------------------------------
  int UnitImpl::getEnergy() const
  {
    return self->energy;
  }
  //--------------------------------------------- GET RESOURCES ----------------------------------------------
  int UnitImpl::getResources() const
  {
    return self->resources;
  }
  //--------------------------------------------- GET RESOURCE GROUP -----------------------------------------
  int UnitImpl::getResourceGroup() const
  {
    return self->resourceGroup;
  }
  //--------------------------------------------- GET DISTANCE -----------------------------------------------
  int UnitImpl::getDistance(PositionOrUnit target) const
  {
    // If this unit does not exist
    if ( !exists() )
      return MAXINT;

    // Must be something valid
    if ( !target.getUnit() && !target.getPosition() )
      return MAXINT;

    // if target is a unit but doesn't exist
    if ( target.getUnit() && !target.getUnit()->exists() )
      return MAXINT;

    // If target is the same as the source
    if ( this == target.getUnit() )
      return MAXINT;

    // Compute distance
    return computeDistance(this, target);
  }
  //--------------------------------------------- HAS PATH ---------------------------------------------------
  bool UnitImpl::hasPath(PositionOrUnit target) const
  {
    Broodwar->setLastError();
    // Return error if the position is invalid
    if ( !target.getPosition() )
      return Broodwar->setLastError(Errors::Invalid_Parameter);

    // Return true if this unit is an air unit
    if ( this->getType().isFlyer() || this->isLifted() )
      return true;

    // Return error if either this or the target does not "exist"
    if ( (target.getUnit() && !target.getUnit()->exists()) || !exists() )
      return Broodwar->setLastError(Errors::Unit_Not_Visible);

    // return result of Game::hasPath
    return Broodwar->hasPath(this->getPosition(), target.getPosition());
  }
  //--------------------------------------------- GET LAST COMMAND FRAME -------------------------------------
  int UnitImpl::getLastCommandFrame() const
  {
    return lastCommandFrame;
  }
  //--------------------------------------------- GET LAST COMMAND -------------------------------------------
  UnitCommand UnitImpl::getLastCommand() const
  {
    return lastCommand;
  }
  //--------------------------------------------- GET LAST ATTACKING PLAYER ----------------------------------
  BWAPI::Player *UnitImpl::getLastAttackingPlayer() const
  {
    return Broodwar->getPlayer(self->lastAttackerPlayer);
  }
  //--------------------------------------------- GET UPGRADE LEVEL ------------------------------------------
  int UnitImpl::getUpgradeLevel(UpgradeType upgrade) const
  {
    if ( !getPlayer() ||
        getPlayer()->getUpgradeLevel(upgrade) == 0 ||
        upgrade.whatUses().find(getType()) == upgrade.whatUses().end() )
      return 0;
    return getPlayer()->getUpgradeLevel(upgrade);
  }
  //--------------------------------------------- GET INITIAL TYPE -------------------------------------------
  UnitType UnitImpl::getInitialType() const
  {
    return initialType;
  }
  //--------------------------------------------- GET INITIAL POSITION ---------------------------------------
  Position UnitImpl::getInitialPosition() const
  {
    return initialPosition;
  }
  //--------------------------------------------- GET INITIAL TILE POSITION ----------------------------------
  TilePosition UnitImpl::getInitialTilePosition() const
  {
    if (initialPosition == Positions::None)
    {
      return TilePositions::None;
    }
    return TilePosition(Position(initialPosition.x - initialType.tileWidth() * TILE_SIZE / 2,
                                 initialPosition.y - initialType.tileHeight() * TILE_SIZE / 2));
  }
  //--------------------------------------------- GET INITIAL HIT POINTS -------------------------------------
  int UnitImpl::getInitialHitPoints() const
  {
    return initialHitPoints;
  }
  //--------------------------------------------- GET INITIAL RESOURCES --------------------------------------
  int UnitImpl::getInitialResources() const
  {
    return initialResources;
  }
  //--------------------------------------------- GET KILL COUNT ---------------------------------------------
  int UnitImpl::getKillCount() const
  {
    return self->killCount;
  }
  //--------------------------------------------- GET ACID SPORE COUNT ---------------------------------------
  int UnitImpl::getAcidSporeCount() const
  {
    return self->acidSporeCount;
  }
  //--------------------------------------------- GET INTERCEPTOR COUNT --------------------------------------
  int UnitImpl::getInterceptorCount() const
  {
    if (getType() != UnitTypes::Protoss_Carrier && getType() != UnitTypes::Hero_Gantrithor)
      return 0;
    return connectedUnits.size();
  }
  //--------------------------------------------- GET SCARAB COUNT -------------------------------------------
  int UnitImpl::getScarabCount() const
  {
    return self->scarabCount;
  }
  //--------------------------------------------- GET SPIDER MINE COUNT --------------------------------------
  int UnitImpl::getSpiderMineCount() const
  {
    return self->spiderMineCount;
  }
  //--------------------------------------------- GET GROUND WEAPON COOLDOWN ---------------------------------
  int UnitImpl::getGroundWeaponCooldown() const
  {
    return self->groundWeaponCooldown;
  }
  //--------------------------------------------- GET AIR WEAPON COOLDOWN ------------------------------------
  int UnitImpl::getAirWeaponCooldown() const
  {
    return self->airWeaponCooldown;
  }
  //--------------------------------------------- GET SPELL COOLDOWN -----------------------------------------
  int UnitImpl::getSpellCooldown() const
  {
    return self->spellCooldown;
  }
  //--------------------------------------------- GET DEFENSE MATRIX POINTS ----------------------------------
  int UnitImpl::getDefenseMatrixPoints() const
  {
    return self->defenseMatrixPoints;
  }
  //--------------------------------------------- GET DEFENSE MATRIX TIMER -----------------------------------
  int UnitImpl::getDefenseMatrixTimer() const
  {
    return self->defenseMatrixTimer;
  }
  //--------------------------------------------- GET ENSNARE TIMER ------------------------------------------
  int UnitImpl::getEnsnareTimer() const
  {
    return self->ensnareTimer;
  }
  //--------------------------------------------- GET IRRADIATE TIMER ----------------------------------------
  int UnitImpl::getIrradiateTimer() const
  {
    return self->irradiateTimer;
  }
  //--------------------------------------------- GET LOCKDOWN TIMER -----------------------------------------
  int UnitImpl::getLockdownTimer() const
  {
    return self->lockdownTimer;
  }
  //--------------------------------------------- GET MAELSTROM TIMER ----------------------------------------
  int UnitImpl::getMaelstromTimer() const
  {
    return self->maelstromTimer;
  }
  //--------------------------------------------- GET ORDER TIMER --------------------------------------------
  int UnitImpl::getOrderTimer() const
  {
    return self->orderTimer;
  }
  //--------------------------------------------- GET PLAGUE TIMER -------------------------------------------
  int UnitImpl::getPlagueTimer() const
  {
    return self->plagueTimer;
  }
  //--------------------------------------------- GET REMOVE TIMER -------------------------------------------
  int UnitImpl::getRemoveTimer() const
  {
    return self->removeTimer;
  }
  //--------------------------------------------- GET STASIS TIMER -------------------------------------------
  int UnitImpl::getStasisTimer() const
  {
    return self->stasisTimer;
  }
  //--------------------------------------------- GET STIM TIMER ---------------------------------------------
  int UnitImpl::getStimTimer() const
  {
    return self->stimTimer;
  }
  //--------------------------------------------- GET BUILD TYPE ---------------------------------------------
  UnitType UnitImpl::getBuildType() const
  {
    return UnitType(self->buildType);
  }
  //--------------------------------------------- GET TRAINING QUEUE -----------------------------------------
  UnitType::set UnitImpl::getTrainingQueue() const
  {
    return UnitType::set(self->trainingQueue, self->trainingQueueCount);
  }
  //--------------------------------------------- GET TECH ---------------------------------------------------
  TechType UnitImpl::getTech() const
  {
    return TechType(self->tech);
  }
  //--------------------------------------------- GET UPGRADE ------------------------------------------------
  UpgradeType UnitImpl::getUpgrade() const
  {
    return UpgradeType(self->upgrade);
  }
  //--------------------------------------------- GET REMAINING BUILD TIME -----------------------------------
  int UnitImpl::getRemainingBuildTime() const
  {
    return self->remainingBuildTime;
  }
  //--------------------------------------------- GET REMAINING TRAIN TIME -----------------------------------
  int UnitImpl::getRemainingTrainTime() const
  {
    return self->remainingTrainTime;
  }
  //--------------------------------------------- GET REMAINING RESEARCH TIME --------------------------------
  int UnitImpl::getRemainingResearchTime() const
  {
    return self->remainingResearchTime;
  }
  //--------------------------------------------- GET REMAINING UPGRADE TIME ---------------------------------
  int UnitImpl::getRemainingUpgradeTime() const
  {
    return self->remainingUpgradeTime;
  }
  //--------------------------------------------- GET BUILD UNIT ---------------------------------------------
  Unit* UnitImpl::getBuildUnit() const
  {
    return Broodwar->getUnit(self->buildUnit);
  }
  //--------------------------------------------- GET TARGET -------------------------------------------------
  Unit* UnitImpl::getTarget() const
  {
    return Broodwar->getUnit(self->target);
  }
  //--------------------------------------------- GET TARGET POSITION ----------------------------------------
  Position UnitImpl::getTargetPosition() const
  {
    return Position(self->targetPositionX,self->targetPositionY);
  }
  //--------------------------------------------- GET ORDER --------------------------------------------------
  Order UnitImpl::getOrder() const
  {
    return Order(self->order);
  }
  //--------------------------------------------- GET ORDER TARGET -------------------------------------------
  Unit* UnitImpl::getOrderTarget() const
  {
    return Broodwar->getUnit(self->orderTarget);
  }
  //--------------------------------------------- GET ORDER TARGET POSITION ----------------------------------
  Position UnitImpl::getOrderTargetPosition() const
  {
    return Position(self->orderTargetPositionX,self->orderTargetPositionY);
  }
  //--------------------------------------------- GET SECONDARY ORDER ID -------------------------------------
  Order UnitImpl::getSecondaryOrder() const
  {
    return Order(self->secondaryOrder);
  }
  //--------------------------------------------- GET RALLY POSITION -----------------------------------------
  Position UnitImpl::getRallyPosition() const
  {
    return Position(self->rallyPositionX,self->rallyPositionY);
  }
  //--------------------------------------------- GET RALLY UNIT ---------------------------------------------
  Unit* UnitImpl::getRallyUnit() const
  {
    return Broodwar->getUnit(self->rallyUnit);
  }
  //--------------------------------------------- GET ADDON --------------------------------------------------
  Unit* UnitImpl::getAddon() const
  {
    return Broodwar->getUnit(self->addon);
  }
  //--------------------------------------------- GET NYDUS EXIT ---------------------------------------------
  Unit* UnitImpl::getNydusExit() const
  {
    return Broodwar->getUnit(self->nydusExit);
  }
  //--------------------------------------------- GET POWER UP -----------------------------------------------
  Unit* UnitImpl::getPowerUp() const
  {
    return Broodwar->getUnit(self->powerUp);
  }
  //--------------------------------------------- GET TRANSPORT ----------------------------------------------
  Unit* UnitImpl::getTransport() const
  {
    return Broodwar->getUnit(self->transport);
  }
  //--------------------------------------------- GET LOADED UNITS -------------------------------------------
  Unitset UnitImpl::getLoadedUnits() const
  {
    return loadedUnits;
  }
  //--------------------------------------------- GET CARRIER ------------------------------------------------
  Unit* UnitImpl::getCarrier() const
  {
    return Broodwar->getUnit(self->carrier);
  }
  //--------------------------------------------- GET INTERCEPTORS -------------------------------------------
  Unitset UnitImpl::getInterceptors() const
  {
    Unitset nothing;
    if (getType() != UnitTypes::Protoss_Carrier && getType() != UnitTypes::Hero_Gantrithor)
      return nothing;
    return connectedUnits;
  }
  //--------------------------------------------- GET HATCHERY -----------------------------------------------
  Unit* UnitImpl::getHatchery() const
  {
    return Broodwar->getUnit(self->hatchery);
  }
  //--------------------------------------------- GET LARVA --------------------------------------------------
  Unitset UnitImpl::getLarva() const
  {
    Unitset nothing;
    if (!getType().producesLarva())
        return nothing;
    return connectedUnits;
  }
  //--------------------------------------------- GET CLIENT INFO --------------------------------------------
  void* UnitImpl::getClientInfo() const
  {
    return clientInfo;
  }
  //--------------------------------------------- SET CLIENT INFO --------------------------------------------
  void UnitImpl::setClientInfo(void* clientinfo)
  {
    clientInfo = clientinfo;
  }
  //--------------------------------------------- EXISTS -----------------------------------------------------
  bool UnitImpl::exists() const
  {
    return self->exists;
  }
  //--------------------------------------------- HAS NUKE ---------------------------------------------------
  bool UnitImpl::hasNuke() const
  {
    return self->hasNuke;
  }
  //--------------------------------------------- IS ACCELERATING --------------------------------------------
  bool UnitImpl::isAccelerating() const
  {
    return self->isAccelerating;
  }
  //--------------------------------------------- IS ATTACKING -----------------------------------------------
  bool UnitImpl::isAttacking() const
  {
    return self->isAttacking;
  }
  //--------------------------------------------- IS ATTACK FRAME --------------------------------------------
  bool UnitImpl::isAttackFrame() const
  {
    return self->isAttackFrame;
  }
  //--------------------------------------------- IS BEING CONSTRUCTED ---------------------------------------
  bool UnitImpl::isBeingConstructed() const
  {
    if (self->isMorphing)
      return true;
    if (self->isCompleted)
      return false;
    if (getType().getRace() != Races::Terran)
      return true;
    return self->buildUnit != -1;
  }
  //--------------------------------------------- IS BEING GATHERED ------------------------------------------
  bool UnitImpl::isBeingGathered() const
  {
    return self->isBeingGathered;
  }
  //--------------------------------------------- IS BEING HEALED --------------------------------------------
  bool UnitImpl::isBeingHealed() const
  {
    return getType().getRace() == Races::Terran &&
           self->isCompleted &&
           self->hitPoints > self->lastHitPoints;
  }
  //--------------------------------------------- IS BLIND ---------------------------------------------------
  bool UnitImpl::isBlind() const
  {
    return self->isBlind;
  }
  //--------------------------------------------- IS BRAKING -------------------------------------------------
  bool UnitImpl::isBraking() const
  {
    return self->isBraking;
  }
  //--------------------------------------------- IS BURROWED ------------------------------------------------
  bool UnitImpl::isBurrowed() const
  {
    return self->isBurrowed;
  }
  //--------------------------------------------- IS CARRYING GAS --------------------------------------------
  bool UnitImpl::isCarryingGas() const
  {
    return self->carryResourceType == 1;
  }
  //--------------------------------------------- IS CARRYING MINERALS ---------------------------------------
  bool UnitImpl::isCarryingMinerals() const
  {
    return self->carryResourceType == 2;
  }
  //--------------------------------------------- IS CLOAKED -------------------------------------------------
  bool UnitImpl::isCloaked() const
  {
    return self->isCloaked;
  }
  //--------------------------------------------- IS COMPLETED -----------------------------------------------
  bool UnitImpl::isCompleted() const
  {
    return self->isCompleted;
  }
  //--------------------------------------------- IS CONSTRUCTING --------------------------------------------
  bool UnitImpl::isConstructing() const
  {
    return self->isConstructing;
  }
  //--------------------------------------------- IS DETECTED ------------------------------------------------
  bool UnitImpl::isDetected() const
  {
    return self->isDetected;
  }
  //--------------------------------------------- IS GATHERING GAS -------------------------------------------
  bool UnitImpl::isGatheringGas() const
  {
    if (!self->isGathering)
      return false;

    if (self->order != Orders::Harvest1   &&
        self->order != Orders::Harvest2   &&
        self->order != Orders::MoveToGas  &&
        self->order != Orders::WaitForGas &&
        self->order != Orders::HarvestGas &&
        self->order != Orders::ReturnGas  &&
        self->order != Orders::ResetCollision)
      return false;

    if (self->order == Orders::ResetCollision)
      return self->carryResourceType == 1;

    //return true if BWOrder is WaitForGas, HarvestGas, or ReturnGas
    if (self->order == Orders::WaitForGas ||
        self->order == Orders::HarvestGas ||
        self->order == Orders::ReturnGas)
      return true;

    //if BWOrder is MoveToGas, Harvest1, or Harvest2 we need to do some additional checks to make sure the unit is really gathering
    Unit *targ = getTarget();
    if ( targ &&
        targ->exists() && 
        targ->isCompleted() &&
        targ->getPlayer() == getPlayer() &&
        targ->getType() != UnitTypes::Resource_Vespene_Geyser &&
        (targ->getType().isRefinery() || targ->getType().isResourceDepot()) )
      return true;
    targ = getOrderTarget();
    if ( targ &&
        targ->exists() && 
        targ->isCompleted() &&
        targ->getPlayer() == getPlayer() &&
        targ->getType() != UnitTypes::Resource_Vespene_Geyser &&
        (targ->getType().isRefinery() || targ->getType().isResourceDepot()))
      return true;
    return false;
  }
  //--------------------------------------------- IS GATHERING MINERALS --------------------------------------
  bool UnitImpl::isGatheringMinerals() const
  {
    if (!self->isGathering)
      return false;

    if (self->order != Orders::Harvest1        &&
        self->order != Orders::Harvest2        &&
        self->order != Orders::MoveToMinerals  &&
        self->order != Orders::WaitForMinerals &&
        self->order != Orders::MiningMinerals  &&
        self->order != Orders::ReturnMinerals  &&
        self->order != Orders::ResetCollision)
      return false;

    if (self->order == Orders::ResetCollision)
      return self->carryResourceType == 2;

    //return true if BWOrder is WaitForMinerals, MiningMinerals, or ReturnMinerals
    if (self->order == Orders::WaitForMinerals ||
        self->order == Orders::MiningMinerals ||
        self->order == Orders::ReturnMinerals)
      return true;

    //if BWOrder is MoveToMinerals, Harvest1, or Harvest2 we need to do some additional checks to make sure the unit is really gathering
    if (getTarget() &&
        getTarget()->exists() &&
        (getTarget()->getType().isMineralField() ||
         (getTarget()->isCompleted() &&
          getTarget()->getPlayer() == getPlayer() &&
          getTarget()->getType().isResourceDepot())))
      return true;
    if (getOrderTarget() &&
        getOrderTarget()->exists() &&
        (getOrderTarget()->getType().isMineralField() ||
         (getOrderTarget()->isCompleted() &&
          getOrderTarget()->getPlayer() == getPlayer() &&
          getOrderTarget()->getType().isResourceDepot())))
      return true;
    return false;
  }
  //--------------------------------------------- IS HALLUCINATION -------------------------------------------
  bool UnitImpl::isHallucination() const
  {
    return self->isHallucination;
  }
  //--------------------------------------------- IS IDLE ----------------------------------------------------
  bool UnitImpl::isIdle() const
  {
    return self->isIdle;
  }
  //--------------------------------------------- IS INTERRUPTIBLE -------------------------------------------
  bool UnitImpl::isInterruptible() const
  {
    return self->isInterruptible;
  }
  //--------------------------------------------- IS INVINCIBLE ----------------------------------------------
  bool UnitImpl::isInvincible() const
  {
    return self->isInvincible;
  }
  //--------------------------------------------- IS IN WEAPON RANGE -----------------------------------------
  bool UnitImpl::isInWeaponRange(Unit *target) const
  {
    // Preliminary checks
    if ( !exists() || !target || !target->exists() || this == target )
      return false;

    // Store the types as locals
    UnitType thisType = getType();
    UnitType targType = target->getType();

    // Obtain the weapon type
    WeaponType wpn = thisType.groundWeapon();
    if ( targType.isFlyer() || target->isLifted() )
      wpn = thisType.airWeapon();

    // Return if there is no weapon type
    if ( wpn == WeaponTypes::None || wpn == WeaponTypes::Unknown )
      return false;

    // Retrieve the min and max weapon ranges
    int minRange = wpn.minRange();
    int maxRange = getPlayer()->weaponMaxRange(wpn);

    // Check if the distance to the unit is within the weapon range
    int distance = computeDistance(this, target);
    return (minRange ? minRange < distance : true) && maxRange >= distance;
  }
  //--------------------------------------------- IS LIFTED --------------------------------------------------
  bool UnitImpl::isLifted() const
  {
    return self->isLifted;
  }
  //--------------------------------------------- IS MORPHING ------------------------------------------------
  bool UnitImpl::isMorphing() const
  {
    return self->isMorphing;
  }
  //--------------------------------------------- IS MOVING --------------------------------------------------
  bool UnitImpl::isMoving() const
  {
    return self->isMoving;
  }
  //--------------------------------------------- IS PARASITED -----------------------------------------------
  bool UnitImpl::isParasited() const
  {
    return self->isParasited;
  }
  //--------------------------------------------- IS SELECTED ------------------------------------------------
  bool UnitImpl::isSelected() const
  {
    return self->isSelected;
  }
  //--------------------------------------------- IS STARTING ATTACK -----------------------------------------
  bool UnitImpl::isStartingAttack() const
  {
    return self->isStartingAttack;
  }
  //--------------------------------------------- IS STUCK ---------------------------------------------------
  bool UnitImpl::isStuck() const
  {
    return self->isStuck;
  }
  //--------------------------------------------- IS TRAINING ------------------------------------------------
  bool UnitImpl::isTraining() const
  {
    return self->isTraining;
  }
  //--------------------------------------------- IS UNDER ATTACK --------------------------------------------
  bool UnitImpl::isUnderAttack() const
  {
    return self->recentlyAttacked;
  }
  //--------------------------------------------- IS UNDER DARK SWARM ----------------------------------------
  bool UnitImpl::isUnderDarkSwarm() const
  {
    return self->isUnderDarkSwarm;
  }
  //--------------------------------------------- IS UNDER DISRUPTION WEB ------------------------------------
  bool UnitImpl::isUnderDisruptionWeb() const
  {
    return self->isUnderDWeb;
  }
  //--------------------------------------------- IS UNDER STORM ---------------------------------------------
  bool UnitImpl::isUnderStorm() const
  {
    return self->isUnderStorm;
  }
  //--------------------------------------------- IS UNPOWERED -----------------------------------------------
  bool UnitImpl::isUnpowered() const
  {
    return self->isUnpowered;
  }
  //--------------------------------------------- IS VISIBLE -------------------------------------------------
  bool UnitImpl::isVisible(Player* player) const
  {
    if ( player )
      return self->isVisible[player->getID()];

    if ( Broodwar->self() )     // isVisible for current player
      return self->isVisible[Broodwar->self()->getID()];

    for(int i = 0; i < 9; ++i)
    {
      if (self->isVisible[i])
        return true;
    }
    return false;
  }
  //--------------------------------------------- CAN ISSUE COMMAND ------------------------------------------
  bool UnitImpl::canIssueCommand(UnitCommand command) const
  {
    return Templates::canIssueCommand(this,command);
  }
}
