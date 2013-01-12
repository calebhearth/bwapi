#pragma once
#include <BWAPI.h>
#include "UnitData.h"
#include <string>

namespace BWAPI
{
  class Player;
  class UnitImpl : public Unit
  {
    private:
      int         id;
      UnitType    initialType;
      int         initialResources;
      int         initialHitPoints;
      Position    initialPosition;
      int         lastCommandFrame;
      UnitCommand lastCommand;
    public:
      UnitData* self;
      Unitset   connectedUnits;
      Unitset   loadedUnits;
      void      clear();
      void      saveInitialState();

      UnitImpl(int id);

      virtual int           getID() const;
      virtual bool          exists() const;
      virtual int           getReplayID() const;
      virtual Player*       getPlayer() const;
      virtual UnitType      getType() const;
      virtual Position      getPosition() const;
      virtual double        getAngle() const;
      virtual double        getVelocityX() const;
      virtual double        getVelocityY() const;
      virtual int           getHitPoints() const;
      virtual int           getShields() const;
      virtual int           getEnergy() const;
      virtual int           getResources() const;
      virtual int           getResourceGroup() const;

      virtual int           getLastCommandFrame() const;
      virtual UnitCommand   getLastCommand() const;
      virtual BWAPI::Player *getLastAttackingPlayer() const;

      virtual UnitType      getInitialType() const;
      virtual Position      getInitialPosition() const;
      virtual TilePosition  getInitialTilePosition() const;
      virtual int           getInitialHitPoints() const;
      virtual int           getInitialResources() const;

      virtual int getKillCount() const;
      virtual int getAcidSporeCount() const;
      virtual int getInterceptorCount() const;
      virtual int getScarabCount() const;
      virtual int getSpiderMineCount() const;
      virtual int getGroundWeaponCooldown() const;
      virtual int getAirWeaponCooldown() const;
      virtual int getSpellCooldown() const;
      virtual int getDefenseMatrixPoints() const;

      virtual int getDefenseMatrixTimer() const;
      virtual int getEnsnareTimer() const;
      virtual int getIrradiateTimer() const;
      virtual int getLockdownTimer() const;
      virtual int getMaelstromTimer() const;
      virtual int getOrderTimer() const;
      virtual int getPlagueTimer() const;
      virtual int getRemoveTimer() const;
      virtual int getStasisTimer() const;
      virtual int getStimTimer() const;

      virtual UnitType      getBuildType() const;
      virtual UnitType::set getTrainingQueue() const;
      virtual TechType      getTech() const;
      virtual UpgradeType   getUpgrade() const;
      virtual int           getRemainingBuildTime() const;
      virtual int           getRemainingTrainTime() const;
      virtual int           getRemainingResearchTime() const;
      virtual int           getRemainingUpgradeTime() const;
      virtual Unit*         getBuildUnit() const;

      virtual Unit*     getTarget() const;
      virtual Position  getTargetPosition() const;
      virtual Order     getOrder() const;
      virtual Unit*     getOrderTarget() const;
      virtual Position  getOrderTargetPosition() const;
      virtual Order     getSecondaryOrder() const;
      virtual Position  getRallyPosition() const;
      virtual Unit*     getRallyUnit() const;
      virtual Unit*     getAddon() const;
      virtual Unit*     getNydusExit() const;
      virtual Unit*     getPowerUp() const;

      virtual Unit*     getTransport() const;
      virtual Unitset   getLoadedUnits() const;
      virtual Unit*     getCarrier() const;
      virtual Unitset   getInterceptors() const;
      virtual Unit*     getHatchery() const;
      virtual Unitset   getLarva() const;

      virtual bool hasNuke() const;
      virtual bool isAccelerating() const;
      virtual bool isAttackFrame() const;
      virtual bool isAttacking() const;
      virtual bool isBeingGathered() const;
      virtual bool isBeingHealed() const;
      virtual bool isBlind() const;
      virtual bool isBraking() const;
      virtual bool isBurrowed() const;
      virtual bool isCarryingGas() const;
      virtual bool isCarryingMinerals() const;
      virtual bool isCloaked() const;
      virtual bool isCompleted() const;
      virtual bool isConstructing() const;
      virtual bool isDetected() const;
      virtual bool isGatheringGas() const;
      virtual bool isGatheringMinerals() const;
      virtual bool isHallucination() const;
      virtual bool isIdle() const;
      virtual bool isInterruptible() const;
      virtual bool isInvincible() const;
      virtual bool isLifted() const;
      virtual bool isMorphing() const;
      virtual bool isMoving() const;
      virtual bool isParasited() const;
      virtual bool isSelected() const;
      virtual bool isStartingAttack() const;
      virtual bool isStuck() const;
      virtual bool isTraining() const;
      virtual bool isUnderAttack() const;
      virtual bool isUnderDarkSwarm() const;
      virtual bool isUnderDisruptionWeb() const;
      virtual bool isUnderStorm() const;
      virtual bool isUnpowered() const;
      virtual bool isVisible(Player* player = nullptr) const;

      virtual bool canCommand() const;
      virtual bool canTargetUnit(const Unit* targetUnit, bool checkCommandibility = true) const;

      virtual bool canAttackMove(bool checkCommandibility = true) const;
      virtual bool canAttackUnit(bool checkCommandibility = true) const;
      virtual bool canAttackUnit(Unit* targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      virtual bool canBuild(bool checkCommandibility = true) const;
      virtual bool canBuild(UnitType uType, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      virtual bool canBuild(UnitType uType, BWAPI::TilePosition tilePos, bool checkTargetUnitType = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      virtual bool canBuildAddon(bool checkCommandibility = true) const;
      virtual bool canBuildAddon(UnitType uType, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      virtual bool canTrain(bool checkCommandibility = true) const;
      virtual bool canTrain(UnitType uType, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      virtual bool canMorph(bool checkCommandibility = true) const;
      virtual bool canMorph(UnitType uType, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      virtual bool canResearch(bool checkCommandibility = true) const;
      virtual bool canResearch(TechType type, bool checkCanIssueCommandType = true) const;
      virtual bool canUpgrade(bool checkCommandibility = true) const;
      virtual bool canUpgrade(UpgradeType type, bool checkCanIssueCommandType = true) const;
      virtual bool canSetRallyPosition(bool checkCommandibility = true) const;
      virtual bool canSetRallyUnit(bool checkCommandibility = true) const;
      virtual bool canSetRallyUnit(const Unit* targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      virtual bool canMove(bool checkCommandibility = true) const;
      virtual bool canPatrol(bool checkCommandibility = true) const;
      virtual bool canFollow(bool checkCommandibility = true) const;
      virtual bool canFollow(const Unit* targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      virtual bool canGather(bool checkCommandibility = true) const;
      virtual bool canGather(const Unit* targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      virtual bool canReturnCargo(bool checkCommandibility = true) const;
      virtual bool canHoldPosition(bool checkCommandibility = true) const;
      virtual bool canStop(bool checkCommandibility = true) const;
      virtual bool canRepair(bool checkCommandibility = true) const;
      virtual bool canRepair(const Unit* targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      virtual bool canBurrow(bool checkCommandibility = true) const;
      virtual bool canUnburrow(bool checkCommandibility = true) const;
      virtual bool canCloak(bool checkCommandibility = true) const;
      virtual bool canDecloak(bool checkCommandibility = true) const;
      virtual bool canSiege(bool checkCommandibility = true) const;
      virtual bool canUnsiege(bool checkCommandibility = true) const;
      virtual bool canLift(bool checkCommandibility = true) const;
      virtual bool canLand(bool checkCommandibility = true) const;
      virtual bool canLand(TilePosition target, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      virtual bool canLoad(bool checkCommandibility = true) const;
      virtual bool canLoad(const Unit* targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      virtual bool canUnloadWithOrWithoutTarget(bool checkCommandibility = true) const;
      virtual bool canUnloadAtPosition(Position targDropPos, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      virtual bool canUnload(bool checkCommandibility = true) const;
      virtual bool canUnload(const Unit* targetUnit, bool checkCanTargetUnit = true, bool checkPosition = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      virtual bool canUnloadAll(bool checkCommandibility = true) const;
      virtual bool canUnloadAllPosition(bool checkCommandibility = true) const;
      virtual bool canUnloadAllPosition(Position targDropPos, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      virtual bool canRightClickPosition(bool checkCommandibility = true) const;
      virtual bool canRightClickUnit(bool checkCommandibility = true) const;
      virtual bool canRightClickUnit(Unit* targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      virtual bool canHaltConstruction(bool checkCommandibility = true) const;
      virtual bool canCancelConstruction(bool checkCommandibility = true) const;
      virtual bool canCancelAddon(bool checkCommandibility = true) const;
      virtual bool canCancelTrain(bool checkCommandibility = true) const;
      virtual bool canCancelTrainSlot(bool checkCommandibility = true) const;
      virtual bool canCancelTrainSlot(int slot, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      virtual bool canCancelMorph(bool checkCommandibility = true) const;
      virtual bool canCancelResearch(bool checkCommandibility = true) const;
      virtual bool canCancelUpgrade(bool checkCommandibility = true) const;
      virtual bool canUseTechWithOrWithoutTarget(bool checkCommandibility = true) const;
      virtual bool canUseTechWithOrWithoutTarget(BWAPI::TechType tech, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      virtual bool canUseTechWithoutTarget(BWAPI::TechType tech, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      virtual bool canUseTechUnit(BWAPI::TechType tech, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      virtual bool canUseTechUnit(BWAPI::TechType tech, const Unit* targetUnit, bool checkCanTargetUnit = true, bool checkTargetsUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      virtual bool canUseTechPosition(BWAPI::TechType tech, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;
      virtual bool canPlaceCOP(bool checkCommandibility = true) const;
      virtual bool canPlaceCOP(TilePosition target, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;

      virtual bool canIssueCommandType(BWAPI::UnitCommandType ct, bool checkCommandibility = true) const;
      virtual bool canIssueCommand(UnitCommand command, bool checkCanUseTechUnitOnUnits = true, bool checkCanBuildUnitType = true, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const;

      virtual bool issueCommand(UnitCommand command);
  };
}
