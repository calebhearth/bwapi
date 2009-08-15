#include "UnitImpl.h"

#include <math.h>
#include <limits>

#include <Util/Logger.h>

#include <BWAPI/Player.h>
#include <BWAPI/Order.h>
#include "BWAPI/GameImpl.h"
#include <BWAPI/WeaponType.h>
#include "Globals.h"
#include "CommandTrain.h"
#include "CommandBuild.h"
#include "CommandAttackMove.h"
#include "CommandAttackUnit.h"
#include "CommandPatrol.h"
#include "CommandHoldPosition.h"
#include "CommandStop.h"
#include "CommandRightClick.h"
#include "CommandInvent.h"
#include "CommandUpgrade.h"
#include "CommandRepair.h"
#include "CommandMorphUnit.h"
#include "CommandMorphBuilding.h"
#include "CommandBurrow.h"
#include "CommandUnburrow.h"
#include "CommandCloak.h"
#include "CommandDecloak.h"
#include "CommandSiege.h"
#include "CommandUnsiege.h"
#include "CommandLift.h"
#include "CommandLand.h"
#include "CommandLoad.h"
#include "CommandUnload.h"
#include "CommandUnloadAll.h"
#include "CommandCancelResearch.h"
#include "CommandCancelUpgrade.h"
#include "CommandCancelConstruction.h"
#include "CommandCancelTrain.h"
#include "CommandCancelMorph.h"
#include "CommandCancelAddon.h"
#include "CommandFollow.h"
#include "CommandSetRally.h"

#include <BW/UnitType.h>
#include <BW/Unit.h>
#include <BW/Offsets.h>
#include <BW/UnitType.h>

namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  UnitImpl::UnitImpl(BW::Unit* unitData,
                     BW::Unit* originalUnit,
                     BW::Unit* unitDataLocal,
                     u16 index)
      : bwUnit(unitData)
      , bwOriginalUnit(originalUnit)
      , bwUnitLocal(unitDataLocal)
      , index(index)
      , userSelected(false)
      , buildUnit(NULL)
      , alive(false)
      , dead(false)
      , savedPlayer(NULL)
      , savedUnitType(UnitTypes::None)
  {
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  UnitImpl::~UnitImpl()
  {
  }
  //------------------------------------------- GET HEALTH POINTS --------------------------------------------
  int UnitImpl::getHitPoints() const
  {
    if (!this->attemptAccess()) return 0;
    return this->getRawDataLocal()->healthPoints;
  }
  //------------------------------------------- GET HEALTH POINTS --------------------------------------------
  int UnitImpl::getShields() const
  {
    if (!this->attemptAccess()) return 0;
    if (this->getBWType().maxShields()>0)
    {
      return this->getRawDataLocal()->shieldPoints;
    }
    return 0;
  }
  //------------------------------------------- GET ENERGY POINTS --------------------------------------------
  int UnitImpl::getEnergy() const
  {
    if (!this->attemptAccess()) return 0;
    if (this->getBWType().isSpellcaster())
    {
      return this->getRawDataLocal()->energy;
    }
    return 0;
  }
  //----------------------------------------------- RESOURCES ------------------------------------------------
  int UnitImpl::getResources() const
  {
    if (!this->attemptAccess()) return 0;
    if (this->getBWType() != BW::UnitID::Resource_MineralPatch1 &&
        this->getBWType() != BW::UnitID::Resource_MineralPatch2 &&
        this->getBWType() != BW::UnitID::Resource_MineralPatch3 &&
        this->getBWType() != BW::UnitID::Resource_VespeneGeyser &&
        this->getBWType() != BW::UnitID::Terran_Refinery &&
        this->getBWType() != BW::UnitID::Protoss_Assimilator &&
        this->getBWType() != BW::UnitID::Zerg_Extractor)
    {
      return 0;
    }
    return this->getRawDataLocal()->unitUnion1.unitUnion1Sub.resourceUnitUnionSub.resourceContained;
  }
  //-------------------------------------------- GET KILL COUNT ----------------------------------------------
  int UnitImpl::getKillCount() const
  {
    if (!this->attemptAccess()) return 0;
    return this->getRawDataLocal()->killCount;
  }
  //----------------------------------------- GROUND WEAPON COOLDOWN -----------------------------------------
  int UnitImpl::getGroundWeaponCooldown() const
  {
    if (!this->attemptAccess()) return 0;
    return this->getRawDataLocal()->groundWeaponCooldown;
  }
  //------------------------------------------ AIR WEAPON COOLDOWN -------------------------------------------
  int UnitImpl::getAirWeaponCooldown() const
  {
    if (!this->attemptAccess()) return 0;
    return this->getRawDataLocal()->airWeaponCooldown;
  }
  //--------------------------------------------- SPELL COOLDOWN ---------------------------------------------
  int UnitImpl::getSpellCooldown() const
  {
    if (!this->attemptAccess()) return 0;
    return this->getRawDataLocal()->spellCooldown;
  }
  //------------------------------------------ DEFENSE MATRIX POINTS -----------------------------------------
  int UnitImpl::getDefenseMatrixPoints() const
  {
    if (!this->attemptAccess()) return 0;
    return this->getRawDataLocal()->defenseMatrixDamage;
  }
  //------------------------------------------ DEFENSE MATRIX TIMER ------------------------------------------
  int UnitImpl::getDefenseMatrixTimer() const
  {
    if (!this->attemptAccess()) return 0;
    return this->getRawDataLocal()->defenseMatrixTimer;
  }
  //----------------------------------------------- STIM TIMER -----------------------------------------------
  int UnitImpl::getStimTimer() const
  {
    if (!this->attemptAccess()) return 0;
    return this->getRawDataLocal()->stimTimer;
  }
  //---------------------------------------------- ENSNARE TIMER ---------------------------------------------
  int UnitImpl::getEnsnareTimer() const
  {
    if (!this->attemptAccess()) return 0;
    return this->getRawDataLocal()->ensareTimer;
  }
  //--------------------------------------------- LOCKDOWN TIMER ---------------------------------------------
  int UnitImpl::getLockdownTimer() const
  {
    if (!this->attemptAccess()) return 0;
    return this->getRawDataLocal()->lockdownTimer;
  }
  //--------------------------------------------- IRRADIATE TIMER --------------------------------------------
  int UnitImpl::getIrradiateTimer() const
  {
    if (!this->attemptAccess()) return 0;
    return this->getRawDataLocal()->irradiateTimer;
  }
  //---------------------------------------------- STASIS TIMER ----------------------------------------------
  int UnitImpl::getStasisTimer() const
  {
    if (!this->attemptAccess()) return 0;
    return this->getRawDataLocal()->stasisTimer;
  }
  //---------------------------------------------- PLAGUE TIMER ----------------------------------------------
  int UnitImpl::getPlagueTimer() const
  {
    if (!this->attemptAccess()) return 0;
    return this->getRawDataLocal()->plagueTimer;
  }
  //--------------------------------------------- MAELSTROM TIMER --------------------------------------------
  int UnitImpl::getMaelstromTimer() const
  {
    if (!this->attemptAccess()) return 0;
    return this->getRawDataLocal()->maelstromTimer;
  }
  //---------------------------------------------- REMOVE TIMER ----------------------------------------------
  int UnitImpl::getRemoveTimer() const
  {
    if (!this->attemptAccess()) return 0;
    return this->getRawDataLocal()->removeTimer;
  }
  //----------------------------------------------- GET OWNER ------------------------------------------------
  Player* UnitImpl::getPlayer() const
  {
    if (!this->attemptAccessSpecial()) return (Player*)BroodwarImpl.players[11];
    if (!this->_exists()) return this->savedPlayer;
    if (this->getRawDataLocal()->playerID < 12)
      return (Player*)BroodwarImpl.players[this->bwUnit->playerID];
    else
      return NULL;
  }
  //----------------------------------------------- GET OWNER ------------------------------------------------
  Player* UnitImpl::_getPlayer() const
  {
    if (!this->_exists()) return this->savedPlayer;
    return (Player*)BroodwarImpl.players[this->bwUnit->playerID];
  }
  //------------------------------------------------- EXISTS -------------------------------------------------
  bool UnitImpl::exists() const
  {
    return this->alive;
  }
  //------------------------------------------------- EXISTS -------------------------------------------------
  bool UnitImpl::died() const
  {
    return this->dead;
  }
  //------------------------------------------------- EXISTS -------------------------------------------------
  bool UnitImpl::_exists() const
  {
    return this->alive;
  }
  //--------------------------------------------- IS ACCELERATING --------------------------------------------
  bool UnitImpl::isAccelerating() const
  {
    if (!this->attemptAccess()) return false;
    return this->getRawDataLocal()->movementFlags.getBit(BW::MovementFlags::Accelerating);
  }
  //-------------------------------------------- IS BEING CONSTRUCTED ----------------------------------------
  bool UnitImpl::isBeingConstructed() const
  {
    if (!this->attemptAccess()) return false;
    if (!this->isCompleted() && this->getType().isBuilding())
      return this->buildUnit != NULL || this->getType().getRace() != Races::Terran;
    return false;
  }
  //--------------------------------------------- IS BEING HEALED --------------------------------------------
  bool UnitImpl::isBeingHealed() const
  {
    if (!this->attemptAccess()) return false;
    return this->getRawDataLocal()->isBeingHealed != 0;
  }
  //------------------------------------------------ IS BLIND ------------------------------------------------
  bool UnitImpl::isBlind() const
  {
    if (!this->attemptAccess()) return false;
    return this->getRawDataLocal()->isBlind != 0;
  }
  //----------------------------------------------- IS BRAKING -----------------------------------------------
  bool UnitImpl::isBraking() const
  {
    if (!this->attemptAccess()) return false;
    return this->getRawDataLocal()->movementFlags.getBit(BW::MovementFlags::Braking);
  }
  //----------------------------------------------- IS BURROWED ----------------------------------------------
  bool UnitImpl::isBurrowed() const
  {
    if (!this->attemptAccess()) return false;
    return this->getRawDataLocal()->status.getBit(BW::StatusFlags::Burrowed);
  }
  //----------------------------------------------- IS BURROWED ----------------------------------------------
  bool UnitImpl::_isBurrowed() const
  {
    if (!this->_exists()) return false;
    return this->getRawDataLocal()->status.getBit(BW::StatusFlags::Burrowed);
  }
  //------------------------------------------------ IS CLOAKED ----------------------------------------------
  bool UnitImpl::isCloaked() const
  {
    if (!this->attemptAccess()) return false;
    return this->getRawDataLocal()->status.getBit(BW::StatusFlags::Cloaked);
  }
  //------------------------------------------------ IS CLOAKED ----------------------------------------------
  bool UnitImpl::_isCloaked() const
  {
    if (!this->_exists()) return false;
    return this->getRawDataLocal()->status.getBit(BW::StatusFlags::Cloaked);
  }
  //---------------------------------------------- IS COMPLETED ----------------------------------------------
  bool UnitImpl::isCompleted() const
  {
    if (!this->attemptAccess()) return false;
    return this->getRawDataLocal()->status.getBit(BW::StatusFlags::Completed);
  }
  //---------------------------------------------- IS COMPLETED ----------------------------------------------
  bool UnitImpl::_isCompleted() const
  {
    if (!this->_exists()) return false;
    return this->getRawDataLocal()->status.getBit(BW::StatusFlags::Completed);
  }
  //--------------------------------------------- IS CONSTRUCTING --------------------------------------------
  bool UnitImpl::isConstructing() const
  {
    if (!this->attemptAccess()) return false;
    return this->getBWOrder() == BW::OrderID::ConstructingBuilding || 
           this->getBWOrder() == BW::OrderID::BuildTerran ||
           this->getBWOrder() == BW::OrderID::DroneBuild ||
           this->getBWOrder() == BW::OrderID::DroneStartBuild ||
           this->getBWOrder() == BW::OrderID::DroneLand ||
           this->getBWOrder() == BW::OrderID::BuildProtoss1 ||
           this->getBWOrder() == BW::OrderID::BuildProtoss2;
  }
  //---------------------------------------------- IS FOLLOWING ----------------------------------------------
  bool UnitImpl::isFollowing() const
  {
    if (!this->attemptAccess()) return false;
    return this->getBWOrder() == BW::OrderID::Follow;
  }
  //-------------------------------------------- IS GATHERING GAS --------------------------------------------
  bool UnitImpl::isGatheringGas() const
  {
    if (!this->attemptAccess()) return false;
    return (this->getBWOrder()==BW::OrderID::MoveToGas ||
            this->getBWOrder()==BW::OrderID::WaitForGas ||
            this->getBWOrder()==BW::OrderID::HarvestGas ||
            this->getBWOrder()==BW::OrderID::ReturnGas);
  }
  //----------------------------------------- IS GATHERING MINERALS ------------------------------------------
  bool UnitImpl::isGatheringMinerals() const
  {
    if (!this->attemptAccess()) return false;
    return (this->getBWOrder()==BW::OrderID::MoveToMinerals ||
            this->getBWOrder()==BW::OrderID::WaitForMinerals ||
            this->getBWOrder()==BW::OrderID::MiningMinerals ||
            this->getBWOrder()==BW::OrderID::ReturnMinerals);
  }
  //-------------------------------------------- IS HALLUCINATION --------------------------------------------
  bool UnitImpl::isHallucination() const
  {
    if (!this->attemptAccess()) return false;
    return this->getRawDataLocal()->status.getBit(BW::StatusFlags::IsHallucination);
  }
  //---------------------------------------------- IS IDLE ---------------------------------------------------
  bool UnitImpl::isIdle() const
  {
    if (!this->attemptAccess()) return false;
    return ((this->getBWOrder() == BW::OrderID::PlayerGuard ||
            this->getBWOrder() == BW::OrderID::Guard ||
            this->getBWOrder() == BW::OrderID::Stop ||
            this->getBWOrder() == BW::OrderID::Pickup1 ||
            this->getBWOrder() == BW::OrderID::Nothing2 ||
            this->getBWOrder() == BW::OrderID::Medic ||
            this->getBWOrder() == BW::OrderID::Carrier ||
            this->getBWOrder() == BW::OrderID::Critter ||
            this->getBWOrder() == BW::OrderID::NukeTrain ||
            this->getBWOrder() == BW::OrderID::Larva) &&
            !this->isTraining());
  }
  //------------------------------------------------ IS LIFTED -----------------------------------------------
  bool UnitImpl::isLifted() const
  {
    if (!this->attemptAccess()) return false;
    return this->getRawDataLocal()->status.getBit(BW::StatusFlags::InAir) &&
           this->getRawDataLocal()->unitID.isBuilding();
  }
  //------------------------------------------------ IS LOADED -----------------------------------------------
  bool UnitImpl::isLoaded() const
  {
    if (!this->attemptAccess()) return false;
    return this->getRawDataLocal()->status.getBit(BW::StatusFlags::InTransport)
           || this->getRawDataLocal()->status.getBit(BW::StatusFlags::InBuilding);
  }
  //---------------------------------------------- IS LOCKED DOWN --------------------------------------------
  bool UnitImpl::isLockedDown() const
  {
    if (!this->attemptAccess()) return false;
    return this->getLockdownTimer() > 0;
  }
  //----------------------------------------------- IS MORPHING ----------------------------------------------
  bool UnitImpl::isMorphing() const
  {
    if (!this->attemptAccess()) return false;
    return this->getBWOrder() == BW::OrderID::Morph1 || this->getBWOrder() == BW::OrderID::Morph2 || this->getBWOrder() == BW::OrderID::ZergBuildSelf;
  }
  //------------------------------------------------ IS MOVING -----------------------------------------------
  bool UnitImpl::isMoving() const
  {
    if (!this->attemptAccess()) return false;
    return this->getRawDataLocal()->movementFlags.getBit(BW::MovementFlags::Moving);
  }
  //---------------------------------------------- IS PATROLLING ---------------------------------------------
  bool UnitImpl::isPatrolling() const
  {
    if (!this->attemptAccess()) return false;
    return this->getBWOrder() == BW::OrderID::Patrol;
  }
  //----------------------------------------------- IS REPAIRING ---------------------------------------------
  bool UnitImpl::isRepairing() const
  {
    if (!this->attemptAccess()) return false;
    return this->getBWOrder() == BW::OrderID::Repair1 || this->getBWOrder() == BW::OrderID::Repair2;
  }
  //---------------------------------------------- IS RESEARCHING --------------------------------------------
  bool UnitImpl::isResearching() const
  {
    if (!this->attemptAccess()) return false;
    return this->getBWOrder() == BW::OrderID::ResearchTech;
  }
  //---------------------------------------------- IS SELECTED -----------------------------------------------
  bool UnitImpl::isSelected() const
  {
    if (!this->attemptAccess()) return false;
    if (BWAPI::BroodwarImpl.isFlagEnabled(BWAPI::Flag::UserInput) == false)
      return false;
    return this->userSelected;
  }
  //---------------------------------------------- IS SELECTED -----------------------------------------------
  bool UnitImpl::isSieged() const
  {
    if (!this->attemptAccess()) return false;
    return this->getBWType().getID() == BW::UnitID::Terran_SiegeTankSiegeMode;
  }
  //------------------------------------------- IS STARTING ATTACK -------------------------------------------
  bool UnitImpl::isStartingAttack() const
  {
    if (!this->attemptAccess()) return false;
    return this->getRawDataLocal()->movementFlags.getBit(BW::MovementFlags::StartingAttack);
  }
  //----------------------------------------------- IS STASISED ----------------------------------------------
  bool UnitImpl::isStasised() const
  {
    if (!this->attemptAccess()) return false;
    return this->getStasisTimer() > 0;
  }
  //---------------------------------------------- IS TRAINING -----------------------------------------------
  bool UnitImpl::isTraining() const
  {
    if (!this->attemptAccess()) return false;
    return !this->hasEmptyBuildQueue();
  }
  //---------------------------------------------- IS UNDER STORM --------------------------------------------
  bool UnitImpl::isUnderStorm() const
  {
    if (!this->attemptAccess()) return false;
    return this->getRawDataLocal()->isUnderStorm != 0;
  }
  //------------------------------------------------ IS UNPOWERED --------------------------------------------
  bool UnitImpl::isUnpowered() const
  {
    if (!this->attemptAccess()) return false;
    if (this->getBWType().getRace() == BW::Race::Protoss && this->getBWType().isBuilding())
    {
      return this->getRawDataLocal()->status.getBit(BW::StatusFlags::DoodadStatesThing);
    }
    return false;
  }
  //----------------------------------------------- IS UPGRADING ---------------------------------------------
  bool UnitImpl::isUpgrading() const
  {
    if (!this->attemptAccess()) return false;
    return this->getBWOrder() == BW::OrderID::Upgrade;
  }
  //----------------------------------------------- IS VISIBLE -----------------------------------------------
  bool UnitImpl::isVisible() const
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->_exists())
    {
      if (this->savedPlayer==BroodwarImpl.self())
        BroodwarImpl.setLastError(Errors::Unit_Does_Not_Exist);
      return false;
    }
    if (this->_getPlayer() == BWAPI::BroodwarImpl.self())
      return true;
    return (this->getRawData()->sprite->visibilityFlags & (1 << Broodwar->self()->getID())) > 0;
  }
  //--------------------------------------------- SET SELECTED -----------------------------------------------
  void UnitImpl::setSelected(bool selectedState)
  {
    if (!this->_exists()) return;
    this->userSelected = selectedState;
  }
  //---------------------------------------------- GET POSITION ----------------------------------------------
  Position UnitImpl::getPosition() const
  {
    if (!this->attemptAccess()) return BWAPI::Positions::Unknown;
    return BWAPI::Position(this->getRawDataLocal()->position.x, this->getRawDataLocal()->position.y);
  }
  //---------------------------------------------- GET POSITION ----------------------------------------------
  Position UnitImpl::_getPosition() const
  {
    if (!this->_exists()) return BWAPI::Positions::Unknown;
    return BWAPI::Position(this->getRawDataLocal()->position.x, this->getRawDataLocal()->position.y);
  }
  //------------------------------------------- GET TILE POSITION --------------------------------------------
  TilePosition UnitImpl::getTilePosition() const
  {
    if (!this->attemptAccess()) return BWAPI::TilePositions::Unknown;
    return TilePosition(Position(this->getPosition().x() - this->getType().tileWidth() * BW::TILE_SIZE / 2,
                                 this->getPosition().y() - this->getType().tileHeight() * BW::TILE_SIZE / 2));
  }
  //------------------------------------------- GET TILE POSITION --------------------------------------------
  TilePosition UnitImpl::_getTilePosition() const
  {
    if (!this->_exists()) return BWAPI::TilePositions::Unknown;
    return TilePosition(Position(this->_getPosition().x() - this->_getType().tileWidth() * BW::TILE_SIZE / 2,
                                 this->_getPosition().y() - this->_getType().tileHeight() * BW::TILE_SIZE / 2));
  }
  //---------------------------------------------- GET DISTANCE ----------------------------------------------
  double UnitImpl::getDistance(Unit* target) const
  {
    if (!this->attemptAccess()) return std::numeric_limits<double>::infinity();
    if (!((UnitImpl*)target)->attemptAccess()) return std::numeric_limits<double>::infinity();
    if (this == target)
      return 0;
    const Unit* i=this;
    const Unit* j=target;
    double result = 0;
    if (i->getPosition().y() - i->getType().dimensionUp() <= j->getPosition().y() + j->getType().dimensionDown())
      if (i->getPosition().y() + i->getType().dimensionDown() >= j->getPosition().y() - j->getType().dimensionUp())
        if (i->getPosition().x() > j->getPosition().x())
          result = i->getPosition().x() - i->getType().dimensionLeft()  - j->getPosition().x() - j->getType().dimensionRight();
        else
          result = j->getPosition().x() - j->getType().dimensionRight() - i->getPosition().x() - i->getType().dimensionLeft();

    if (i->getPosition().x() - i->getType().dimensionLeft() <= j->getPosition().x() + j->getType().dimensionRight())
      if (i->getPosition().x() + i->getType().dimensionRight() >= j->getPosition().x() - j->getType().dimensionLeft())
        if (i->getPosition().y() > j->getPosition().y())
          result = i->getPosition().y() - i->getType().dimensionUp()   - j->getPosition().y() - j->getType().dimensionDown();
        else
          result = j->getPosition().y() - j->getType().dimensionDown() - i->getPosition().y() - i->getType().dimensionUp();

    if (i->getPosition().x() > j->getPosition().x())
    {
      if (i->getPosition().y() > j->getPosition().y())
        result = BWAPI::Position(i->getPosition().x() - i->getType().dimensionLeft(),
                                 i->getPosition().y() - i->getType().dimensionUp()).getDistance(
                 BWAPI::Position(j->getPosition().x() + j->getType().dimensionRight(),
                                 j->getPosition().y() + j->getType().dimensionDown()));
      else
        result = BWAPI::Position(i->getPosition().x() - i->getType().dimensionLeft(),
                                 i->getPosition().y() + i->getType().dimensionDown()).getDistance(
                 BWAPI::Position(j->getPosition().x() + j->getType().dimensionRight(),
                                 j->getPosition().y() - j->getType().dimensionUp()));
    }
    else
    {
      if (i->getPosition().y() > j->getPosition().y())
        result = BWAPI::Position(i->getPosition().x() + i->getType().dimensionRight(),
                                 i->getPosition().y() - i->getType().dimensionUp()).getDistance(
                 BWAPI::Position(j->getPosition().x() - j->getType().dimensionLeft(),
                                 j->getPosition().y() + j->getType().dimensionDown()));
      else
        result = BWAPI::Position(i->getPosition().x() + i->getType().dimensionRight(),
                                 i->getPosition().y() + i->getType().dimensionDown()).getDistance(
                 BWAPI::Position(j->getPosition().x() - j->getType().dimensionLeft(),
                                 j->getPosition().y() - j->getType().dimensionUp()));
    }
    if (result > 0)
      return result;
    return 0;
  }
  //---------------------------------------------- GET DISTANCE ----------------------------------------------
  double UnitImpl::getDistance(Position target) const
  {
    if (!this->attemptAccess()) return std::numeric_limits<double>::infinity();
    double result = 0;
    if (getPosition().y() - getType().dimensionUp() <= target.y())
      if (getPosition().y() + getType().dimensionDown() >= target.y())
        if (getPosition().x() > target.x())
          result = getPosition().x() - getType().dimensionLeft()  - target.x();
        else
          result = target.x() - getPosition().x() - getType().dimensionLeft();

    if (getPosition().x() - getType().dimensionLeft() <= target.x())
      if (getPosition().x() + getType().dimensionRight() >= target.x())
        if (getPosition().y() > target.y())
          result = getPosition().y() - getType().dimensionUp()   - target.y();
        else
          result = target.y() - getPosition().y() - getType().dimensionUp();

    if (this->getPosition().x() > target.x())
    {
      if (this->getPosition().y() > target.y())
        result = BWAPI::Position(getPosition().x() - getType().dimensionLeft(),
                                 getPosition().y() - getType().dimensionUp()).getDistance(target);
      else
        result = BWAPI::Position(getPosition().x() - getType().dimensionLeft(),
                                 getPosition().y() + getType().dimensionDown()).getDistance(target);
    }
    else
    {
      if (this->getPosition().y() > target.y())
        result = BWAPI::Position(getPosition().x() + getType().dimensionRight(),
                                 getPosition().y() - getType().dimensionUp()).getDistance(target);
      else
        result = BWAPI::Position(getPosition().x() + getType().dimensionRight(),
                                 getPosition().y() + getType().dimensionDown()).getDistance(target);
    }
    if (result > 0)
      return result;
    return 0;
  }
  //----------------------------------------------- GET TARGET -----------------------------------------------
  Unit* UnitImpl::getTarget() const
  {
    if (!this->attemptAccess()) return NULL;
    return UnitImpl::BWUnitToBWAPIUnit(this->getRawData()->targetUnit);
  }
  //-------------------------------------------- GET ORDER TARGET --------------------------------------------
  Unit* UnitImpl::getOrderTarget() const
  {
    if (!this->isVisible()) return NULL;
    return UnitImpl::BWUnitToBWAPIUnit(this->getRawData()->orderTargetUnit);
  }
  //-------------------------------------------- GET ORDER TARGET --------------------------------------------
  Unit* UnitImpl::_getOrderTarget() const
  {
    if (!this->_exists()) return NULL;
    return UnitImpl::BWUnitToBWAPIUnit(this->getRawData()->orderTargetUnit);
  }
  //--------------------------------------------- GET BUILD UNIT ---------------------------------------------
  Unit* UnitImpl::getBuildUnit() const
  {
    if (!this->attemptAccess()) return NULL;
    if (this->getRawDataLocal()->currentBuildUnit)
      return UnitImpl::BWUnitToBWAPIUnit(this->getRawDataLocal()->currentBuildUnit);
    return (Unit*)this->buildUnit;
  }
  //----------------------------------------------- GET CHILD ------------------------------------------------
  Unit* UnitImpl::getChild() const
  {
    if (!this->attemptAccess()) return NULL;
    return UnitImpl::BWUnitToBWAPIUnit(this->getRawDataLocal()->childInfoUnion.childUnit1);
  }
  //------------------------------------------ GET TARGET POSITION -------------------------------------------
  Position UnitImpl::getTargetPosition() const
  {
    if (!this->attemptAccess()) return BWAPI::Positions::Unknown;
    return BWAPI::Position(this->getRawDataLocal()->moveToPos.x, this->getRawDataLocal()->moveToPos.y);
  }
  //-------------------------------------------- CURRENT DIRECTION -------------------------------------------
  int UnitImpl::getCurrentDirection() const
  {
    if (!this->attemptAccess()) return 0;
    return this->getRawDataLocal()->currentDirection;
  }
  //---------------------------------------------- GET RAW DATA ----------------------------------------------
  BW::Unit* UnitImpl::getRawData() const
  {
    return this->bwUnit;
  }
  //------------------------------------------- GET RAW DATA LOCAL -------------------------------------------
  BW::Unit* UnitImpl::getRawDataLocal() const
  {
    return this->bwUnitLocal;
  }
  //----------------------------------------- GET ORIGINAL RAW DATA ------------------------------------------
  BW::Unit* UnitImpl::getOriginalRawData() const
  {
    return this->bwOriginalUnit;
  }
  //------------------------------------------------ GET ORDER -----------------------------------------------
  Order UnitImpl::getOrder() const
  {
    if (!this->attemptAccess()) return BWAPI::Orders::None;
    return BWAPI::Order(this->getRawDataLocal()->orderID);
  }
  //------------------------------------------------ GET ORDER -----------------------------------------------
  BW::OrderID::Enum UnitImpl::getBWOrder() const
  {
    if (!this->_exists()) return BW::OrderID::Nothing2;
    return this->getRawDataLocal()->orderID;
  }
  //----------------------------------------- GET SECONDARY ORDER ID -----------------------------------------
  Order UnitImpl::getSecondaryOrder() const
  {
    if (!this->attemptAccess()) return BWAPI::Orders::None;
    return BWAPI::Order(this->getRawDataLocal()->secondaryOrderID);
  }
  //------------------------------------------ GET TRAINING QUEUE --------------------------------------------
  std::list<UnitType > UnitImpl::getTrainingQueue() const
  {
    std::list<UnitType > trainList;
    if (!this->attemptAccess()) return trainList;
    if (this->hasEmptyBuildQueue()) return trainList;
    int i = this->getBuildQueueSlot() % 5;
    int starti = i;
    trainList.push_front(BWAPI::UnitType(this->getBuildQueue()[i].id));
    i = (i + 1) % 5;
    while(this->getBuildQueue()[i] != BW::UnitID::None && i != starti)
    {
      trainList.push_back(BWAPI::UnitType(this->getBuildQueue()[i].id));
      i = (i + 1) % 5;
    }
    return trainList;
  }
  //-------------------------------------------- GET TRANSPORT -----------------------------------------------
  Unit* UnitImpl::getTransport() const
  {
    if (!this->attemptAccess()) return NULL;
    if (!this->isLoaded()) return NULL;
    return (Unit*)(UnitImpl::BWUnitToBWAPIUnit(this->getRawDataLocal()->connectedUnit));
  }
  //------------------------------------------- GET LOADED UNITS ---------------------------------------------
  std::list<Unit*> UnitImpl::getLoadedUnits() const
  {
    std::list<Unit*> unitList;
    if (!this->attemptAccess()) return unitList;
    for(int i = 0; i < 8; i++)
    {
      if (this->getRawDataLocal()->loadedUnitIndex[i] != 0)
      {
        BW::Unit* bwunit = (BW::Unit*)(0x4F4B58 + this->getRawDataLocal()->loadedUnitIndex[i] * BW::UNIT_SIZE_IN_BYTES);
        UnitImpl* unit = BWUnitToBWAPIUnit(bwunit);
        if (unit!=NULL)
        {
          unitList.push_back((Unit*)unit);
        }
      }
    }
    return unitList;
  }
  //----------------------------------------------- GET TECH -------------------------------------------------
  TechType UnitImpl::getTech() const
  {
    if (!this->attemptAccess()) return TechTypes::None;
    int techID = this->getRawDataLocal()->childUnitUnion2.unitIsNotScarabInterceptor.subChildUnitUnion1.techID;
    return TechType(techID);
  }
  //---------------------------------------------- GET UPGRADE -----------------------------------------------
  UpgradeType UnitImpl::getUpgrade() const
  {
    if (!this->attemptAccess()) return UpgradeTypes::None;
    int upgradeID = this->getRawDataLocal()->childUnitUnion2.unitIsNotScarabInterceptor.subChildUnitUnion2.upgradeID;
    return UpgradeType(upgradeID);
  }
  //-------------------------------------- GET REMAINING RESEARCH TIME ---------------------------------------
  int UnitImpl::getRemainingResearchTime() const
  {
    if (!this->attemptAccess()) return 0;
    if (this->isResearching())
      return this->getRawDataLocal()->childUnitUnion1.unitIsBuilding.upgradeResearchTime;
    return 0;
  }
  //-------------------------------------- GET REMAINING UPGRADE TIME ----------------------------------------
  int UnitImpl::getRemainingUpgradeTime() const
  {
    if (!this->attemptAccess()) return 0;
    if (this->isUpgrading())
      return this->getRawDataLocal()->childUnitUnion1.unitIsBuilding.upgradeResearchTime;
    return 0;
  }
  //------------------------------------------ GET RALLY POSITION --------------------------------------------
  Position UnitImpl::getRallyPosition() const
  {
    if (!this->attemptAccess()) return BWAPI::Positions::None;
    if (this->getBWType().canProduce())
      return Position(this->getRawDataLocal()->rallyPsiProviderUnion.rally.rallyX,
                      this->getRawDataLocal()->rallyPsiProviderUnion.rally.rallyY);
    return Positions::None;
  }
  //-------------------------------------------- GET RALLY UNIT ----------------------------------------------
  Unit* UnitImpl::getRallyUnit() const
  {
    if (!this->attemptAccess()) return NULL;
    if (this->getBWType().canProduce())
      return (Unit*)UnitImpl::BWUnitToBWAPIUnit(this->getRawDataLocal()->rallyPsiProviderUnion.rally.rallyUnit);
    return NULL;
  }
  //----------------------------------------------- GET ADDON ------------------------------------------------
  Unit* UnitImpl::getAddon() const
  {
    if (!this->attemptAccess()) return NULL;
    if (this->getType().isBuilding())
    {
      if (this->getRawDataLocal()->currentBuildUnit != NULL)
      {
        if (UnitImpl::BWUnitToBWAPIUnit(this->getRawDataLocal()->currentBuildUnit)->getBWType().isAddon())
        {
          return  (Unit*)UnitImpl::BWUnitToBWAPIUnit(this->getRawDataLocal()->currentBuildUnit);
        }
      }
      if (this->getRawDataLocal()->childInfoUnion.childUnit1 != NULL)
      {
        return (Unit*)UnitImpl::BWUnitToBWAPIUnit(this->getRawDataLocal()->childInfoUnion.childUnit1);
      }
    }
    return NULL;
  }
  //-------------------------------------------- HAS EMPTY QUEUE ---------------------------------------------
  bool UnitImpl::hasEmptyBuildQueueSync() const
  {
    if (!this->_exists()) true;
    return this->getBuildQueueSync()[this->getBuildQueueSlotSync()] == BW::UnitID::None;
  }
  //----------------------------------------- HAS EMPTY QUEUE LOCAL ------------------------------------------
  bool UnitImpl::hasEmptyBuildQueue() const
  {
    if (!this->_exists()) true;
    if (this->getBuildQueueSlot() < 5)
      return this->getBuildQueue()[this->getBuildQueueSlot()] == BW::UnitID::None;
    else
      return false;
  }
  //----------------------------------------- HAS FULL QUEUE LOCAL -------------------------------------------
  bool UnitImpl::hasFullBuildQueue() const
  {
    if (!this->_exists()) false;
    return this->getBuildQueue()[(this->getBuildQueueSlot() + 1) % 5] != BW::UnitID::None;
  }
  //------------------------------------------- ORDER Attack Location ----------------------------------------
  bool UnitImpl::attackMove(Position position)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->attemptAccess()) return false;
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position(position.x(), position.y()), BW::OrderID::AttackMove), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new CommandAttackMove(this, BW::Position(position.x(), position.y())));
    return true;
  }
  //--------------------------------------------- ORDER Attack Unit ------------------------------------------
  bool UnitImpl::attackUnit(Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->attemptAccess()) return false;
    if (target==NULL) return false;
    if (!((UnitImpl*)target)->attemptAccess()) return false;
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    const WeaponType* weapon=this->getType().groundWeapon();
    if (target->isLifted() || target->getType().isFlyer())
      weapon=this->getType().airWeapon();

    if (*weapon==WeaponTypes::None)
    {
      BroodwarImpl.setLastError(Errors::Unable_To_Hit);
      return false;
    }
    if (!this->getType().canMove())
    {
      if (this->getDistance(target)>weapon->maxRange() ||
          this->getDistance(target)<weapon->minRange())
      {
        BroodwarImpl.setLastError(Errors::Out_Of_Range);
        return false;
      }
    }
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::AttackUnit), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new CommandAttackUnit(this, (UnitImpl*)target));
    return true;
  }
  //------------------------------------------- ORDER RIGHT CLICK --------------------------------------------
  bool UnitImpl::rightClick(Position position)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->attemptAccess()) return false;
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::RightClick(BW::Position(position.x(), position.y())), sizeof(BW::Orders::RightClick));
    BroodwarImpl.addToCommandBuffer(new CommandRightClick(this, BW::Position(position.x(), position.y())));
    return true;
  }
  //------------------------------------------- ORDER RIGHT CLICK --------------------------------------------
  bool UnitImpl::rightClick(Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->attemptAccess()) return false;
    if (target==NULL) return false;
    if (!((UnitImpl*)target)->attemptAccess()) return false;
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (!target->getPlayer()->isNeutral() && !target->getPlayer()->isAlliesWith(this->getPlayer()))
    {
      const WeaponType* weapon=this->getType().groundWeapon();
      if (target->isLifted() || target->getType().isFlyer())
        weapon=this->getType().airWeapon();

      if (*weapon==WeaponTypes::None)
      {
        BroodwarImpl.setLastError(Errors::Unable_To_Hit);
        return false;
      }
      if (!this->getType().canMove())
      {
        if (this->getDistance(target)>weapon->maxRange() ||
            this->getDistance(target)<weapon->minRange())
        {
          BroodwarImpl.setLastError(Errors::Out_Of_Range);
          return false;
        }
      }
    }
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::RightClick((UnitImpl*)target), sizeof(BW::Orders::RightClick));
    BroodwarImpl.addToCommandBuffer(new CommandRightClick(this, (UnitImpl*)target));
    return true;
  }
  //----------------------------------------------- TRAIN UNIT -----------------------------------------------
  bool UnitImpl::train(UnitType type1)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->attemptAccess()) return false;
    if (!Broodwar->canMake(this,type1)) return false;
    if (type1.isBuilding())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if (this->isLifted() || !this->isIdle() || !this->isCompleted())
    {
      BroodwarImpl.setLastError(Errors::Unit_Busy);
      return false;
    }

    BW::UnitType type((BW::UnitID::Enum)type1.getID());
    if (this->getType() == BWAPI::UnitTypes::Zerg_Larva ||
        this->getType() == BWAPI::UnitTypes::Zerg_Mutalisk ||
        this->getType() == BWAPI::UnitTypes::Zerg_Hydralisk)
    {
      this->orderSelect();
      BroodwarImpl.addToCommandBuffer(new CommandTrain(this, type));
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::UnitMorph(type), 0x3);
    }
    else if (this->getType() == BWAPI::UnitTypes::Zerg_Hatchery ||
             this->getType() == BWAPI::UnitTypes::Zerg_Lair ||
             this->getType() == BWAPI::UnitTypes::Zerg_Spire ||
             this->getType() == BWAPI::UnitTypes::Zerg_Creep_Colony)
    {
      this->orderSelect();
      BroodwarImpl.addToCommandBuffer(new CommandTrain(this, type));
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::BuildingMorph(type), 0x3);
    }
    else
    {
      this->orderSelect();
      BroodwarImpl.addToCommandBuffer(new CommandTrain(this, type));
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::TrainUnit(type), 0x3);
    }
    return true;
  }
  //------------------------------------------------- BUILD --------------------------------------------------
  bool UnitImpl::build(TilePosition position, UnitType type1)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->attemptAccess()) return false;
    if (!Broodwar->canMake(this,type1)) return false;
    if (!type1.isBuilding())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if (this->isConstructing() || !this->isCompleted())
    {
      BroodwarImpl.setLastError(Errors::Unit_Busy);
      return false;
    }
    if (!type1.isAddon() && !Broodwar->canBuildHere(this,position,type1)) return false;

    BW::UnitType type(BW::UnitID::Enum(type1.getID()));
    this->orderSelect();
    if (!type.isAddon())
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::MakeBuilding(BW::TilePosition(position.x(), position.y()), type), sizeof(BW::Orders::MakeBuilding));
    else
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::MakeAddon(BW::TilePosition(position.x(), position.y()), type), sizeof(BW::Orders::MakeAddon));
    BroodwarImpl.addToCommandBuffer(new CommandBuild(this, type, BW::TilePosition(position.x(), position.y())));
    return true;
  }
  //----------------------------------------------- BUILD ADDON ----------------------------------------------
  bool UnitImpl::buildAddon(UnitType type1)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->attemptAccess()) return false;
    if (!type1.isAddon())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    return this->build(TilePosition(this->getTilePosition().x()+4,this->getTilePosition().y()+1),type1);    
  }
  //------------------------------------------------ RESEARCH ------------------------------------------------
  bool UnitImpl::research(TechType tech)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->attemptAccess()) return false;
    if (!Broodwar->canResearch(this,tech)) return false;
    if (this->isLifted() || !this->isIdle() || !this->isCompleted())
    {
      BroodwarImpl.setLastError(Errors::Unit_Busy);
      return false;
    }

    this->orderSelect();
    BW::TechID::Enum techenum = static_cast<BW::TechID::Enum>(tech.getID());
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Invent(BW::TechType(techenum)), sizeof(BW::Orders::Invent));
    BroodwarImpl.addToCommandBuffer(new CommandInvent(this, BW::TechType(techenum)));
    return true;
  }
  //------------------------------------------------- UPGRADE ------------------------------------------------
  bool UnitImpl::upgrade(UpgradeType upgrade)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->attemptAccess()) return false;
    if (!Broodwar->canUpgrade(this,upgrade)) return false;
    if (this->isLifted() || !this->isIdle() || !this->isCompleted())
    {
      BroodwarImpl.setLastError(Errors::Unit_Busy);
      return false;
    }

    this->orderSelect();
    BW::UpgradeID::Enum upgradeenum = static_cast<BW::UpgradeID::Enum>(upgrade.getID());
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Upgrade(BW::UpgradeType(upgradeenum)), sizeof(BW::Orders::Upgrade));
    BroodwarImpl.addToCommandBuffer(new CommandUpgrade(this, BW::UpgradeType(upgradeenum)));
    return true;
  }
  //-------------------------------------------------- STOP --------------------------------------------------
  bool UnitImpl::stop()
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->attemptAccess()) return false;
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Stop(0), sizeof(BW::Orders::Stop));
    this->getRawDataLocal()->orderID = BW::OrderID::Stop;
    BroodwarImpl.addToCommandBuffer(new CommandStop(this));
    return true;
  }
  //---------------------------------------------- HOLD POSITION ---------------------------------------------
  bool UnitImpl::holdPosition()
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->attemptAccess()) return false;
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::HoldPosition(0), sizeof(BW::Orders::HoldPosition));
    switch (this->getBWType().getID())
    {
      case BW::UnitID::Protoss_Carrier:
      case BW::UnitID::Protoss_Hero_Gantrithor:
        this->getRawDataLocal()->orderID = BW::OrderID::HoldPosition1;
        break;
      case BW::UnitID::Zerg_Queen:
      case BW::UnitID::Zerg_Hero_Matriarch:
        this->getRawDataLocal()->orderID = BW::OrderID::HoldPosition3;
        break;
      case BW::UnitID::Zerg_InfestedTerran:
      case BW::UnitID::Zerg_Scourge:
        this->getRawDataLocal()->orderID = BW::OrderID::HoldPosition4;
        break;
      case BW::UnitID::Terran_Medic:
        this->getRawDataLocal()->orderID = BW::OrderID::MedicHoldPosition;
        break;
      case BW::UnitID::Protoss_Reaver:
        this->getRawDataLocal()->orderID = BW::OrderID::ReaverHold;
        break;
      default:
        this->getRawDataLocal()->orderID = BW::OrderID::HoldPosition2;
    }
    BroodwarImpl.addToCommandBuffer(new CommandHoldPosition(this));
    return true;
  }
  //-------------------------------------------------- PATROL ------------------------------------------------
  bool UnitImpl::patrol(Position position)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->attemptAccess()) return false;
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->getType().isBuilding())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position(position.x(), position.y()), BW::OrderID::Patrol), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new CommandPatrol(this, BW::Position(position.x(), position.y())));
    return true;
  }
  //-------------------------------------------------- FOLLOW ------------------------------------------------
  bool UnitImpl::follow(Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->attemptAccess()) return false;
    if (target==NULL) return false;
    if (!((UnitImpl*)target)->attemptAccess()) return false;
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->getType().isBuilding())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::Follow), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new CommandFollow(this, (UnitImpl*)target));
    return true;
  }
  //------------------------------------------------- SET RALLY ----------------------------------------------
  bool UnitImpl::setRallyPosition(Position target)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->attemptAccess()) return false;
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (!this->getType().canProduce())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position(target.x(), target.y()), BW::OrderID::RallyPoint2), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new CommandSetRally(this, BW::Position(target.x(), target.y())));
    return true;
  }
  //------------------------------------------------- SET RALLY ----------------------------------------------
  bool UnitImpl::setRallyUnit(Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->attemptAccess()) return false;
    if (target==NULL) return false;
    if (!((UnitImpl*)target)->attemptAccess()) return false;
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (!this->getType().canProduce())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::RallyPoint1), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new CommandSetRally(this, (UnitImpl*)target));
    return true;
  }
  //-------------------------------------------------- REPAIR ------------------------------------------------
  bool UnitImpl::repair(Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->attemptAccess()) return false;
    if (target==NULL) return false;
    if (!((UnitImpl*)target)->attemptAccess()) return false;
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->getType() != UnitTypes::Terran_SCV || target->getType().isOrganic())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::Repair1), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new CommandRepair(this, (UnitImpl*)target));
    return true;
  }
  //-------------------------------------------------- MORPH -------------------------------------------------
  bool UnitImpl::morph(UnitType type)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->attemptAccess()) return false;
    if (!Broodwar->canMake(this,type)) return false;
    if (type.isBuilding()!=this->getType().isBuilding())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if (!this->isIdle() || !this->isCompleted())
    {
      BroodwarImpl.setLastError(Errors::Unit_Busy);
      return false;
    }

    this->orderSelect();
    BW::UnitType rawtype(((BW::UnitID::Enum)type.getID()));
    if(type.isBuilding())
    {
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::BuildingMorph(rawtype), sizeof(BW::Orders::BuildingMorph));
      BroodwarImpl.addToCommandBuffer(new CommandMorphBuilding(this, rawtype));
    }
    else
    {
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::UnitMorph(rawtype), sizeof(BW::Orders::UnitMorph));
      BroodwarImpl.addToCommandBuffer(new CommandMorphUnit(this, rawtype));
    }
    return true;
  }
  //-------------------------------------------------- BURROW ------------------------------------------------
  bool UnitImpl::burrow()
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->attemptAccess()) return false;
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (!this->getType().isBurrowable())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if (this->getType()!=UnitTypes::Zerg_Lurker && !Broodwar->self()->researched(TechTypes::Burrowing))
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Tech);
      return false;
    }

    if(!this->isBurrowed())
    {
      this->orderSelect();
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Burrow(), sizeof(BW::Orders::Burrow));
      BroodwarImpl.addToCommandBuffer(new CommandBurrow(this));
    }
    return true;
  }
  //------------------------------------------------- UNBURROW -----------------------------------------------
  bool UnitImpl::unburrow()
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->attemptAccess()) return false;
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (!this->getType().isBurrowable())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if (this->getType()!=UnitTypes::Zerg_Lurker && !Broodwar->self()->researched(TechTypes::Burrowing))
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Tech);
      return false;
    }

    if(this->isBurrowed())
    {
      this->orderSelect();
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Unburrow(), sizeof(BW::Orders::Unburrow));
      BroodwarImpl.addToCommandBuffer(new CommandUnburrow(this));
    }
    return true;
  }
  //-------------------------------------------------- SIEGE -------------------------------------------------
  bool UnitImpl::siege()
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->attemptAccess()) return false;
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->getType() != UnitTypes::Terran_Siege_Tank_Tank_Mode && this->getType() != UnitTypes::Terran_Siege_Tank_Siege_Mode)
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if (!Broodwar->self()->researched(TechTypes::Tank_Siege_Mode))
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Tech);
      return false;
    }

    if (!this->isSieged())
    {
      this->orderSelect();
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Siege(), sizeof(BW::Orders::Siege));
      BroodwarImpl.addToCommandBuffer(new CommandSiege(this));
    }
    return true;
  }
  //------------------------------------------------- UNSIEGE ------------------------------------------------
  bool UnitImpl::unsiege()
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->attemptAccess()) return false;
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->getType() != UnitTypes::Terran_Siege_Tank_Tank_Mode && this->getType() != UnitTypes::Terran_Siege_Tank_Siege_Mode)
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if (!Broodwar->self()->researched(TechTypes::Tank_Siege_Mode))
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Tech);
      return false;
    }

    if (this->isSieged())
    {
      this->orderSelect();
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Unsiege(), sizeof(BW::Orders::Unsiege));
      BroodwarImpl.addToCommandBuffer(new CommandUnsiege(this));
    }
    return true;
  }
  //-------------------------------------------------- CLOAK -------------------------------------------------
  bool UnitImpl::cloak()
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->attemptAccess()) return false;
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->getType()!=UnitTypes::Terran_Wraith && this->getType()!=UnitTypes::Terran_Ghost)
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if (this->getType()==UnitTypes::Terran_Wraith)
    {
      if (!Broodwar->self()->researched(TechTypes::Cloaking_Field))
      {
        BroodwarImpl.setLastError(Errors::Insufficient_Tech);
        return false;
      }
      if (this->getEnergy()<TechTypes::Cloaking_Field.energyUsed())
      {
        BroodwarImpl.setLastError(Errors::Insufficient_Energy);
        return false;
      }
    }
    if (this->getType()==UnitTypes::Terran_Ghost)
    {
      if (!Broodwar->self()->researched(TechTypes::Personnel_Cloaking))
      {
        BroodwarImpl.setLastError(Errors::Insufficient_Tech);
        return false;
      }
      if (this->getEnergy()<TechTypes::Personnel_Cloaking.energyUsed())
      {
        BroodwarImpl.setLastError(Errors::Insufficient_Energy);
        return false;
      }
    }
    if(!this->isCloaked())
    {
      this->orderSelect();
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Cloak(), sizeof(BW::Orders::Cloak));
      BroodwarImpl.addToCommandBuffer(new CommandCloak(this));
    }
    return true;
  }
  //------------------------------------------------- DECLOAK ------------------------------------------------
  bool UnitImpl::decloak()
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->attemptAccess()) return false;
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->getType()!=UnitTypes::Terran_Wraith && this->getType()!=UnitTypes::Terran_Ghost)
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if ((this->getType()==UnitTypes::Terran_Wraith && !Broodwar->self()->researched(TechTypes::Cloaking_Field))
      || (this->getType()==UnitTypes::Terran_Ghost && !Broodwar->self()->researched(TechTypes::Personnel_Cloaking)))
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Tech);
      return false;
    }

    if(this->isCloaked())
    {
      this->orderSelect();
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Decloak(), sizeof(BW::Orders::Decloak));
      BroodwarImpl.addToCommandBuffer(new CommandDecloak(this));
    }
    return true;
  }
  //--------------------------------------------------- LIFT -------------------------------------------------
  bool UnitImpl::lift()
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->attemptAccess()) return false;
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (!this->getType().isFlyingBuilding())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if(!this->isLifted())
    {
      this->orderSelect();
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Lift(), sizeof(BW::Orders::Lift));
      BroodwarImpl.addToCommandBuffer(new CommandLift(this));
    }
    return true;
  }
  //--------------------------------------------------- LAND -------------------------------------------------
  bool UnitImpl::land(TilePosition position)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->attemptAccess()) return false;
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (!this->getType().isFlyingBuilding())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if(this->isLifted())
    {
      this->orderSelect();
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Land(BW::TilePosition(position.x(), position.y()), this->getBWType()), sizeof(BW::Orders::Land));
      BroodwarImpl.addToCommandBuffer(new CommandLand(this, BW::TilePosition(position.x(), position.y())));
    }
    return true;
  }
  //--------------------------------------------------- LOAD -------------------------------------------------
  bool UnitImpl::load(Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->attemptAccess()) return false;
    if (target==NULL) return false;
    if (!((UnitImpl*)target)->attemptAccess()) return false;
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    this->orderSelect();
    bool loaded = false;
    if (this->getType() == UnitTypes::Terran_Bunker)
    {
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::Pickup3), sizeof(BW::Orders::Attack));
      loaded = true;
    }
    else if (this->getType() == UnitTypes::Terran_Dropship || this->getType() == UnitTypes::Protoss_Shuttle || this->getType() == UnitTypes::Zerg_Overlord)
    {
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::Pickup2), sizeof(BW::Orders::Attack));
      loaded = true;
    }
    else if (target->getType() == UnitTypes::Terran_Bunker || target->getType() == UnitTypes::Terran_Dropship || target->getType() == UnitTypes::Protoss_Shuttle || target->getType() == UnitTypes::Zerg_Overlord)
    {
      this->rightClick(target);
      loaded = true;
    }
    if (loaded)
    {
      BroodwarImpl.addToCommandBuffer(new CommandLoad(this, (UnitImpl*)target));
      return true;
    }
    //if neither this unit nor the target unit is a bunker, dropship, shuttle, or overlord, return false.
    BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
    return false;
  }
  //-------------------------------------------------- UNLOAD ------------------------------------------------
  bool UnitImpl::unload(Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->attemptAccess()) return false;
    if (target==NULL) return false;
    if (!((UnitImpl*)target)->attemptAccess()) return false;
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::UnloadUnit((UnitImpl*)target), sizeof(BW::Orders::UnloadUnit));
    BroodwarImpl.addToCommandBuffer(new CommandUnload(this, (UnitImpl*)target));
    return true;
  }
  //------------------------------------------------- UNLOADALL ----------------------------------------------
  bool UnitImpl::unloadAll()
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->attemptAccess()) return false;
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->getType() == UnitTypes::Terran_Dropship || this->getType() == UnitTypes::Protoss_Shuttle || this->getType() == UnitTypes::Zerg_Overlord)
    {
      return this->unloadAll(this->getPosition());
    }
    if (this->getType() != UnitTypes::Terran_Bunker)
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::UnloadAll(), sizeof(BW::Orders::UnloadAll));
    BroodwarImpl.addToCommandBuffer(new CommandUnloadAll(this));
    return true;
  }
  //------------------------------------------------- UNLOADALL ----------------------------------------------
  bool UnitImpl::unloadAll(Position position)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->attemptAccess()) return false;
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->getType() == UnitTypes::Terran_Bunker)
    {
      this->unloadAll();
    }
    if (this->getType() != UnitTypes::Terran_Dropship && this->getType() != UnitTypes::Protoss_Shuttle && this->getType() != UnitTypes::Zerg_Overlord)
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position(position.x(), position.y()), BW::OrderID::MoveUnload), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new CommandUnloadAll(this, BW::Position(position.x(), position.y())));
    return true;
  }
  //-------------------------------------------- CANCEL CONSTRUCTION -----------------------------------------
  bool UnitImpl::cancelConstruction()
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->attemptAccess()) return false;
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->isCompleted()) return false;
    if (!this->getType().isBuilding())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::CancelConstruction(), sizeof(BW::Orders::CancelConstruction));
    BroodwarImpl.addToCommandBuffer(new CommandCancelConstruction(this));
    return true;
  }
  //--------------------------------------------- HALT CONSTRUCTION ------------------------------------------
  bool UnitImpl::haltConstruction()
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->attemptAccess()) return false;
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->getOrder() != Orders::ConstructingBuilding) return false;
    return this->stop();
  }
  //----------------------------------------------- CANCEL MORPH ---------------------------------------------
  bool UnitImpl::cancelMorph()
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->attemptAccess()) return false;
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->getType().isBuilding())
    {
      return this->cancelConstruction();
    }
    if (this->isMorphing())
    {
      this->orderSelect();
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::CancelUnitMorph(), sizeof(BW::Orders::CancelUnitMorph));
      BroodwarImpl.addToCommandBuffer(new CommandCancelMorph(this));
    }
    return true;
  }
  //----------------------------------------------- CANCEL TRAIN ---------------------------------------------
  bool UnitImpl::cancelTrain()
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->attemptAccess()) return false;
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->isTraining())
    {
      this->orderSelect();
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::CancelTrainLast(), sizeof(BW::Orders::CancelTrainLast));
      BroodwarImpl.addToCommandBuffer(new CommandCancelTrain(this));
    }
    return true;
  }
  //----------------------------------------------- CANCEL TRAIN ---------------------------------------------
  bool UnitImpl::cancelTrain(int slot)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->attemptAccess()) return false;
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->isTraining() && (int)(this->getTrainingQueue().size()) > slot)
    {
      this->orderSelect();
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::CancelTrain(slot), sizeof(BW::Orders::CancelTrain));
      BroodwarImpl.addToCommandBuffer(new CommandCancelTrain(this, slot));
    }
    return true;
  }
  //----------------------------------------------- CANCEL ADDON ---------------------------------------------
  bool UnitImpl::cancelAddon()
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->attemptAccess()) return false;
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::CancelAddon(), sizeof(BW::Orders::CancelAddon));
    BroodwarImpl.addToCommandBuffer(new CommandCancelAddon(this));
    return true;
  }
  //---------------------------------------------- CANCEL RESEARCH -------------------------------------------
  bool UnitImpl::cancelResearch()
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->attemptAccess()) return false;
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->isResearching())
    {
      this->orderSelect();
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::CancelResearch(), sizeof(BW::Orders::CancelResearch));
      BroodwarImpl.addToCommandBuffer(new CommandCancelResearch(this));
    }
    return true;
  }
  //---------------------------------------------- CANCEL UPGRADE --------------------------------------------
  bool UnitImpl::cancelUpgrade()
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->attemptAccess()) return false;
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->isUpgrading())
    {
      this->orderSelect();
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::CancelUpgrade(), sizeof(BW::Orders::CancelUpgrade));
      BroodwarImpl.addToCommandBuffer(new CommandCancelUpgrade(this));
    }
    return true;
  }
  //------------------------------------------------- USE TECH -----------------------------------------------
  bool UnitImpl::useTech(TechType tech)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->attemptAccess()) return false;
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (!Broodwar->self()->researched(tech))
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Tech);
      return false;
    }
    if (this->getEnergy()<tech.energyUsed())
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Energy);
      return false;
    }
    bool found=false;
    for(std::set<const UnitType*>::const_iterator i=tech.whatUses().begin();i!=tech.whatUses().end();i++)
      if (*(*i)==this->getType())
        found=true;
    if (!found)
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    switch (tech.getID())
    {
      case BW::TechID::Stimpacks:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::UseStimPack(), sizeof(BW::Orders::UseStimPack));
        return true;
      case BW::TechID::TankSiegeMode:
        if (this->isSieged())
          this->unsiege();
        else
          this->siege();
        return true;
      case BW::TechID::PersonnelCloaking:
      case BW::TechID::CloakingField:
        if(this->isCloaked())
          this->decloak();
        else
          this->cloak();
        return true;
      case BW::TechID::Burrowing:
        if(this->isBurrowed())
          this->unburrow();
        else
          this->burrow();
        return true;
      case BW::TechID::ArchonWarp:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::MergeArchon(), sizeof(BW::Orders::MergeArchon));
        return true;
      case BW::TechID::DarkArchonMeld:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::MergeDarkArchon(), sizeof(BW::Orders::MergeDarkArchon));
        return true;
      default:
        BroodwarImpl.setLastError(Errors::Incompatible_TechType);
        return false;
    }
    return true;
  }
  //------------------------------------------------- USE TECH -----------------------------------------------
  bool UnitImpl::useTech(TechType tech, Position position)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->attemptAccess()) return false;
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (!Broodwar->self()->researched(tech))
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Tech);
      return false;
    }
    if (this->getEnergy()<tech.energyUsed())
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Energy);
      return false;
    }
    bool found=false;
    for(std::set<const UnitType*>::const_iterator i=tech.whatUses().begin();i!=tech.whatUses().end();i++)
      if (*(*i)==this->getType())
        found=true;
    if (!found)
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    switch (tech.getID())
    {
      case BW::TechID::DarkSwarm:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position(position.x(), position.y()), BW::OrderID::DarkSwarm), sizeof(BW::Orders::Attack));
        return true;
      case BW::TechID::DisruptionWeb:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position(position.x(), position.y()), BW::OrderID::CastDisruptionWeb), sizeof(BW::Orders::Attack));
        return true;
      case BW::TechID::EMPShockwave:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position(position.x(), position.y()), BW::OrderID::EmpShockwave), sizeof(BW::Orders::Attack));
        return true;
      case BW::TechID::Ensnare:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position(position.x(), position.y()), BW::OrderID::Ensnare), sizeof(BW::Orders::Attack));
        return true;
      case BW::TechID::Maelstorm:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position(position.x(), position.y()), BW::OrderID::CastMaelstrom), sizeof(BW::Orders::Attack));
        return true;
      case BW::TechID::NuclearStrike:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position(position.x(), position.y()), BW::OrderID::NukePaint), sizeof(BW::Orders::Attack));
        return true;
      case BW::TechID::Plague:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position(position.x(), position.y()), BW::OrderID::Plague), sizeof(BW::Orders::Attack));
        return true;
      case BW::TechID::PsionicStorm:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position(position.x(), position.y()), BW::OrderID::PsiStorm), sizeof(BW::Orders::Attack));
        return true;
      case BW::TechID::Recall:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position(position.x(), position.y()), BW::OrderID::Teleport), sizeof(BW::Orders::Attack));
        return true;
      case BW::TechID::ScannerSweep:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position(position.x(), position.y()), BW::OrderID::PlaceScanner), sizeof(BW::Orders::Attack));
        return true;
      case BW::TechID::SpiderMines:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position(position.x(), position.y()), BW::OrderID::PlaceMine), sizeof(BW::Orders::Attack));
        return true;
      case BW::TechID::StasisField:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position(position.x(), position.y()), BW::OrderID::StasisField), sizeof(BW::Orders::Attack));
        return true;
      default:
        BroodwarImpl.setLastError(Errors::Incompatible_TechType);
        return false;
    }
    return true;
  }
  //------------------------------------------------- USE TECH -----------------------------------------------
  bool UnitImpl::useTech(TechType tech, Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!this->attemptAccess()) return false;
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (!Broodwar->self()->researched(tech))
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Tech);
      return false;
    }
    if (this->getEnergy()<tech.energyUsed())
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Energy);
      return false;
    }
    bool found=false;
    for(std::set<const UnitType*>::const_iterator i=tech.whatUses().begin();i!=tech.whatUses().end();i++)
      if (*(*i)==this->getType())
        found=true;
    if (!found)
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    switch (tech.getID())
    {
      case BW::TechID::Consume:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::Consume), sizeof(BW::Orders::Attack));
        return true;
      case BW::TechID::DefensiveMatrix:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::DefensiveMatrix), sizeof(BW::Orders::Attack));
        return true;
      case BW::TechID::Feedback:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::CastFeedback), sizeof(BW::Orders::Attack));
        return true;
      case BW::TechID::Hallucination:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::Hallucination1), sizeof(BW::Orders::Attack));
        return true;
      case BW::TechID::Healing:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::MedicHeal1), sizeof(BW::Orders::Attack));
        return true;
      case BW::TechID::Infestation:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::InfestMine2), sizeof(BW::Orders::Attack));
        return true;
      case BW::TechID::Irradiate:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::Irradiate), sizeof(BW::Orders::Attack));
        return true;
      case BW::TechID::Lockdown:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::MagnaPulse), sizeof(BW::Orders::Attack));
        return true;
      case BW::TechID::MindControl:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::CastMindControl), sizeof(BW::Orders::Attack));
        return true;
      case BW::TechID::OpticalFlare:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::CastOpticalFlare), sizeof(BW::Orders::Attack));
        return true;
      case BW::TechID::Parasite:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::CastParasite), sizeof(BW::Orders::Attack));
        return true;
      case BW::TechID::Restoration:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::Restoration), sizeof(BW::Orders::Attack));
        return true;
      case BW::TechID::SpawnBroodlings:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::SummonBroodlings), sizeof(BW::Orders::Attack));
        return true;
      case BW::TechID::YamatoGun:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::FireYamatoGun1), sizeof(BW::Orders::Attack));
        return true;
      default:
        BroodwarImpl.setLastError(Errors::Incompatible_TechType);
        return false;
    }
    return true;
  }
  //---------------------------------------------- ORDER SELECT ----------------------------------------------
  void UnitImpl::orderSelect()
  {
    if (BroodwarImpl.quietSelect)
    {
      BW::Unit * * select = new BW::Unit * [1];
      select[0] = this->getOriginalRawData();
      BW::selectUnits(1, select);
      delete [] select;
      //Broodwar.IssueCommand((PBYTE)&BW::Orders::SelectSingle(this),sizeof(BW::Orders::SelectSingle));
    }
    else
    {
      BW::Unit * * list = new BW::Unit * [2];
      list[0] = this->getOriginalRawData();
      list[1] = NULL;
      int one = 1;
      BW::selectUnitsHelperSTD(one, list, true, true);
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  BWAPI::UnitType UnitImpl::getType() const
  {
    if (!this->attemptAccessSpecial()) return UnitTypes::Unknown;
    if (!this->_exists()) return this->savedUnitType;

    if ( this->getRawDataLocal()->unitID.id == BW::UnitID::Resource_MineralPatch1
         || this->getRawDataLocal()->unitID.id == BW::UnitID::Resource_MineralPatch2
         || this->getRawDataLocal()->unitID.id == BW::UnitID::Resource_MineralPatch3)
    {
      return BWAPI::UnitTypes::Resource_Mineral_Field;
    }
    else
    {
      return BWAPI::UnitType(this->getRawDataLocal()->unitID.id);
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  BWAPI::UnitType UnitImpl::_getType() const
  {
    if (!this->_exists()) return this->savedUnitType;
    if ( this->getRawDataLocal()->unitID.id == BW::UnitID::Resource_MineralPatch1
         || this->getRawDataLocal()->unitID.id == BW::UnitID::Resource_MineralPatch2
         || this->getRawDataLocal()->unitID.id == BW::UnitID::Resource_MineralPatch3)
    {
      return BWAPI::UnitTypes::Resource_Mineral_Field;
    }
    else
    {
      return BWAPI::UnitType(this->getRawDataLocal()->unitID.id);
    }
  }
  //------------------------------------------------ GET BW TYPE ---------------------------------------------
  BW::UnitType UnitImpl::getBWType() const
  {
    if (!this->_exists()) return BW::UnitType(BW::UnitID::None);
    return this->getRawDataLocal()->unitID;
  }
  //----------------------------------------------- GET QUEUE ------------------------------------------------
  BW::UnitType* UnitImpl::getBuildQueueSync() const
  {
    return this->getRawData()->buildQueue;
  }
  //-------------------------------------------- GET QUEUE LOCAL  --------------------------------------------
  BW::UnitType* UnitImpl::getBuildQueue() const
  {
    return this->getRawDataLocal()->buildQueue;
  }
  //--------------------------------------------- GET QUEUE SLOT ---------------------------------------------
  u8 UnitImpl::getBuildQueueSlotSync() const
  {
    return this->getRawData()->buildQueueSlot;
  }
  //------------------------------------------ GET QUEUE SLOT LOCAL ------------------------------------------
  u8 UnitImpl::getBuildQueueSlot() const
  {
    return this->getOriginalRawData()->buildQueueSlot;
  }
  //----------------------------------------------------------------------------------------------------------
#pragma warning(push)
#pragma warning(disable:4311)
  UnitImpl* UnitImpl::BWUnitToBWAPIUnit(BW::Unit* unit)
  {
    if (unit == NULL)
      return NULL;
    int index=(((int)unit - (int)BW::BWDATA_UnitNodeTable) / 336) & 2047;
    if (index<0 || index>=BW::UNIT_ARRAY_MAX_LENGTH)
    {
      if (BroodwarImpl.invalidIndices.find(index)==BroodwarImpl.invalidIndices.end())
      {
        BroodwarImpl.newUnitLog->log("Error: Found new invalid unit index: %d, broodwar address: 0x%x",index,unit);
        BroodwarImpl.invalidIndices.insert(index);
      }
      return NULL;
    }
    return BroodwarImpl.getUnit(((int)unit - (int)BW::BWDATA_UnitNodeTable) / 336);
  }
#pragma warning (pop)
  void UnitImpl::die()
  {
    if (!this->alive) return;
    this->savedPlayer=this->_getPlayer();
    this->savedUnitType=this->_getType();
    this->bwUnit=NULL;
    this->bwUnitLocal=NULL;
    this->bwOriginalUnit=NULL;
    this->index=-1;
    this->userSelected=false;
    this->visible=false;
    this->alive=false;
    this->dead=true;
  }
  bool UnitImpl::canAccess() const
  {
    if (this->isVisible()) return true;
    if (this->_exists())
    {
      if (BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation)) return true;
      if (BroodwarImpl.flagsLocked==false)
      {
        if (this->getBWType().isNeutral() || this->getBWType().isNeutralAccesories()) return true;
      }
    }
    if (BroodwarImpl.inUpdate==true) return true;
    return false;
  }
  bool UnitImpl::canAccessSpecial() const
  {
    if (this->isVisible()) return true;
    if (this->_exists())
    {
      if (BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation)) return true;
      if (BroodwarImpl.flagsLocked==false)
      {
        if (this->getBWType().isNeutral() || this->getBWType().isNeutralAccesories()) return true;
      }
      if (BroodwarImpl.inUpdate==true) return true;
    }
    if (!this->_exists() && this->savedPlayer==BroodwarImpl.self()) return true;
    return false;
  }
  bool UnitImpl::attemptAccess() const
  {
    if (!BroodwarImpl.inUpdate) BroodwarImpl.setLastError(Errors::None);
    if (this->canAccess()) return true;
    if (!this->_exists() && this->savedPlayer==BroodwarImpl.self())
    {
      if (!BroodwarImpl.inUpdate) BroodwarImpl.setLastError(Errors::Unit_Does_Not_Exist);
      return false;
    }
    if (!BroodwarImpl.inUpdate) BroodwarImpl.setLastError(Errors::Unit_Not_Visible);
    return false;      
  }
  bool UnitImpl::attemptAccessSpecial() const
  {
    if (!BroodwarImpl.inUpdate) BroodwarImpl.setLastError(Errors::None);
    if (this->canAccessSpecial()) return true;
    if (!BroodwarImpl.inUpdate) BroodwarImpl.setLastError(Errors::Unit_Not_Visible);
    return false;      
  }
  char position[100];
  char indexName[50];
  char targetIndex[50];
  char orderTargetIndex[50];
  char owner[100];
  char unitName[100];
  char orderName[100];
  char connectedUnit[100];
  char message[400];
  //----------------------------------------------------------------------------------------------------------
  std::string UnitImpl::getName() const
  {
    sprintf_s(position, 100, "Position = (%d,%d)", this->getPosition().x(),
              this->getPosition().y());

    sprintf_s(indexName, 50, "[%d]", this->getIndex());

    if (this->getTarget() == NULL)
      strcpy_s(targetIndex, 50, "Target:[NULL]");
    else
      sprintf_s(targetIndex, 50, "Target:[%d](%s)", (int)this->getTarget(), this->getTarget()->getType().getName().c_str());

    if (this->getOrderTarget() == NULL)
      strcpy_s(orderTargetIndex, 50, "OrderTarget:[NULL]");
    else
      sprintf_s(orderTargetIndex, 50, "OrderTarget:[%d](%s)", (int)(this->getOrderTarget()), this->getOrderTarget()->getType().getName().c_str());

    if (this->getPlayer() != NULL)
      sprintf_s(owner, 100, "Player = (%s)", this->getPlayer()->getName().c_str());
    else
      sprintf_s(owner, 100, "error owner id = (%d)", this->getOriginalRawData()->playerID);

    sprintf_s(unitName, 100, "(%s)", this->getType().getName().c_str());

    if (this->getChild() == NULL)
      sprintf_s(connectedUnit, 100, "(childUnit1 = NULL)");
    else
      sprintf_s(connectedUnit, 100, "(childUnit1 = %s)", this->getChild()->getType().getName().c_str());

    sprintf_s(orderName, 100, "(%s)", this->getOrder().getName().c_str());
    sprintf_s(message, 400, "%s %s %s %s %s %s %s %s", unitName,
              orderName,
              indexName,
              position,
              targetIndex,
              orderTargetIndex,
              owner,
              connectedUnit);

    return std::string(message);
  }
  //---------------------------------------------- UPDATE NEXT -----------------------------------------------
  UnitImpl* UnitImpl::getNext() const
  {
#ifdef PARANOID_DEBUG
#pragma warning(push)
#pragma warning(disable:4311)
    if (this->getOriginalRawData()->nextUnit != NULL)
    {
      if (((int)this->getOriginalRawData()->nextUnit - (int)BW::BWXFN_UnitNodeTable) / BW::UNIT_SIZE_IN_BYTES >= BW::UNIT_ARRAY_MAX_LENGTH)
        BWAPI::Broodwar.fatalError->log("Unit array too small, found unit with addr %X", (int)this->getOriginalRawData()->nextUnit);
      if ((int)this->getOriginalRawData()->nextUnit < (int)BW::BWXFN_UnitNodeTable)
        BWAPI::Broodwar.fatalError->log("Unit array begins at bad location, found unit with addr %X", (int)this->getOriginalRawData()->nextUnit);
    }
#pragma warning(pop)
#endif PARANOID_DEBUG
    return UnitImpl::BWUnitToBWAPIUnit(this->getRawDataLocal()->nextUnit);
  }
  //-------------------------------------------- GET ORDER TIMER ---------------------------------------------
  int UnitImpl::getOrderTimer() const
  {
    return this->getRawDataLocal()->mainOrderTimer;
  }
  //---------------------------------------- GET REMAINING BUILD TIME ----------------------------------------
  int UnitImpl::getRemainingBuildTime() const
  {
    return this->getRawDataLocal()->remainingBuildTime;
  }
  int UnitImpl::getRemainingTrainTime() const
  {
    if (this->getRawDataLocal()->currentBuildUnit)
      return this->getRawDataLocal()->currentBuildUnit->remainingBuildTime;
    return 0;
  }
  //----------------------------------------------------------------------------------------------------------
  u16 UnitImpl::getIndex() const
  {
    return this->index;
  }
  //----------------------------------------------------------------------------------------------------------
};
