
#include "Unit.h"
#include "BridgeClient.h"

#include <string>

#include "Util\Version.h"

namespace BWAgent
{
  void pushCommand(BWAPI::CommandID::Enum commandID,int unitID, int x=0, int y=0, int targetID=0, int specialID=0);
  Unit::Unit()
  {
  }
  int Unit::getID()
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return -1;
    return data->getID;
  }
  int Unit::getType()
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return -1;
    return data->getType;
  }


  int Unit::getHitPoints()
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return -1;
    return data->getHitPoints;
  }

  bool Unit::exists() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->exists;
  }

  bool Unit::isAccelerating() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isAccelerating;
  }
  bool Unit::isBeingConstructed() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isBeingConstructed;
  }
  bool Unit::isBeingGathered() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isBeingGathered;
  }
  bool Unit::isBeingHealed() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isBeingHealed;
  }
  bool Unit::isBlind() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isBlind;
  }
  bool Unit::isBraking() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isBraking;
  }
  bool Unit::isBurrowed() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isBurrowed;
  }
  bool Unit::isCarryingGas() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isCarryingGas;
  }
  bool Unit::isCarryingMinerals() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isCarryingMinerals;
  }
  bool Unit::isCloaked() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isCloaked;
  }
  bool Unit::isCompleted() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isCompleted;
  }
  bool Unit::isConstructing() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isConstructing;
  }
  bool Unit::isDefenseMatrixed() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isDefenseMatrixed;
  }
  bool Unit::isEnsnared() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isEnsnared;
  }
  bool Unit::isFollowing() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isFollowing;
  }
  bool Unit::isGatheringGas() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isGatheringGas;
  }
  bool Unit::isGatheringMinerals() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isGatheringMinerals;
  }
  bool Unit::isHallucination() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isHallucination;
  }
  bool Unit::isIdle() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isIdle;
  }
  bool Unit::isIrradiated() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isIrradiated;
  }
  bool Unit::isLifted() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isLifted;
  }
  bool Unit::isLoaded() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isLoaded;
  }
  bool Unit::isLockedDown() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isLockedDown;
  }
  bool Unit::isMaelstrommed() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isMaelstrommed;
  }
  bool Unit::isMorphing() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isMorphing;
  }
  bool Unit::isMoving() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isMoving;
  }
  bool Unit::isParasited() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isParasited;
  }
  bool Unit::isPatrolling() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isPatrolling;
  }
  bool Unit::isPlagued() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isPlagued;
  }
  bool Unit::isRepairing() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isRepairing;
  }
  bool Unit::isResearching() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isResearching;
  }
  bool Unit::isSelected() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isSelected;
  }
  bool Unit::isSieged() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isSieged;
  }
  bool Unit::isStartingAttack() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isStartingAttack;
  }
  bool Unit::isStasised() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isStasised;
  }
  bool Unit::isStimmed() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isStimmed;
  }
  bool Unit::isTraining() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isTraining;
  }
  bool Unit::isUnderStorm() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isUnderStorm;
  }
  bool Unit::isUnpowered() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isUnpowered;
  }
  bool Unit::isUpgrading() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isUpgrading;
  }
  bool Unit::isVisible() const
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return false;
    return data->isVisible;
  }
  bool Unit::attackUnit(Unit* target)
  {
    if(!BridgeClient::sharedCommandData || !this->data)
      return false;
    pushCommand(BWAPI::CommandID::AttackUnit,data->getID,0,0,target->getID());
    return true;
  }
  bool Unit::stop()
  {
    if(!BridgeClient::sharedCommandData || !this->data)
      return false;
    pushCommand(BWAPI::CommandID::Stop,data->getID);
    return true;
  }
  bool Unit::holdPosition()
  {
    if(!BridgeClient::sharedCommandData || !this->data)
      return false;
    pushCommand(BWAPI::CommandID::HoldPosition,data->getID);
    return true;
  }
  bool Unit::burrow()
  {
    if(!BridgeClient::sharedCommandData || !this->data)
      return false;
    pushCommand(BWAPI::CommandID::Burrow,data->getID);
    return true;
  }
  bool Unit::unburrow()
  {
    if(!BridgeClient::sharedCommandData || !this->data)
      return false;
    pushCommand(BWAPI::CommandID::Unburrow,data->getID);
    return true;
  }
  bool Unit::siege()
  {
    if(!BridgeClient::sharedCommandData || !this->data)
      return false;
    pushCommand(BWAPI::CommandID::Siege,data->getID);
    return true;
  }
  bool Unit::unsiege()
  {
    if(!BridgeClient::sharedCommandData || !this->data)
      return false;
    pushCommand(BWAPI::CommandID::Unsiege,data->getID);
    return true;
  }
  bool Unit::cloak()
  {
    if(!BridgeClient::sharedCommandData || !this->data)
      return false;
    pushCommand(BWAPI::CommandID::Cloak,data->getID);
    return true;
  }
  bool Unit::decloak()
  {
    if(!BridgeClient::sharedCommandData || !this->data)
      return false;
    pushCommand(BWAPI::CommandID::Decloak,data->getID);
    return true;
  }
  bool Unit::lift()
  {
    if(!BridgeClient::sharedCommandData || !this->data)
      return false;
    pushCommand(BWAPI::CommandID::Lift,data->getID);
    return true;
  }
  bool Unit::cancelConstruction()
  {
    if(!BridgeClient::sharedCommandData || !this->data)
      return false;
    pushCommand(BWAPI::CommandID::CancelConstruction,data->getID);
    return true;
  }
  bool Unit::haltConstruction()
  {
    if(!BridgeClient::sharedCommandData || !this->data)
      return false;
    pushCommand(BWAPI::CommandID::HaltConstruction,data->getID);
    return true;
  }
  bool Unit::cancelMorph()
  {
    if(!BridgeClient::sharedCommandData || !this->data)
      return false;
    pushCommand(BWAPI::CommandID::CancelMorph,data->getID);
    return true;
  }
  bool Unit::cancelTrain()
  {
    if(!BridgeClient::sharedCommandData || !this->data)
      return false;
    pushCommand(BWAPI::CommandID::CancelTrain,data->getID);
    return true;
  }
  bool Unit::cancelTrain(int slot)
  {
    if(!BridgeClient::sharedCommandData || !this->data)
      return false;
    pushCommand(BWAPI::CommandID::CancelTrain,data->getID,0,0,0,slot);
    return true;
  }
  bool Unit::cancelAddon()
  {
    if(!BridgeClient::sharedCommandData || !this->data)
      return false;
    pushCommand(BWAPI::CommandID::CancelAddon,data->getID);
    return true;
  }
  bool Unit::cancelResearch()
  {
    if(!BridgeClient::sharedCommandData || !this->data)
      return false;
    pushCommand(BWAPI::CommandID::CancelResearch,data->getID);
    return true;
  }
  bool Unit::cancelUpgrade()
  {
    if(!BridgeClient::sharedCommandData || !this->data)
      return false;
    pushCommand(BWAPI::CommandID::CancelUpgrade,data->getID);
    return true;
  }

  void Unit::_update(BWAPI::ClearanceLevels level,BWAPI::State* data)
  {
    this->level=level;
    this->data=data;
  }
  void pushCommand(BWAPI::CommandID::Enum commandID, int unitID, int x, int y, int targetID, int specialID)
  {
    BWAPI::UnitCommand* c = &(BridgeClient::sharedCommandData->commandQueue[BridgeClient::sharedCommandData->lastFreeCommandSlot]);
    c->commandID=commandID;
    c->unitID=unitID;
    c->x=x;
    c->y=y;
    c->targetID=targetID;
    c->specialID=specialID;
    BridgeClient::sharedCommandData->lastFreeCommandSlot++;
  }
}
