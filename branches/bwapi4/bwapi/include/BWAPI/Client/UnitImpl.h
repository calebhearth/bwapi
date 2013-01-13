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

      virtual int           getID() const override;
      virtual bool          exists() const override;
      virtual int           getReplayID() const override;
      virtual Player*       getPlayer() const override;
      virtual UnitType      getType() const override;
      virtual Position      getPosition() const override;
      virtual double        getAngle() const override;
      virtual double        getVelocityX() const override;
      virtual double        getVelocityY() const override;
      virtual int           getHitPoints() const override;
      virtual int           getShields() const override;
      virtual int           getEnergy() const override;
      virtual int           getResources() const override;
      virtual int           getResourceGroup() const override;

      virtual int           getLastCommandFrame() const override;
      virtual UnitCommand   getLastCommand() const override;
      virtual BWAPI::Player *getLastAttackingPlayer() const override;

      virtual UnitType      getInitialType() const override;
      virtual Position      getInitialPosition() const override;
      virtual TilePosition  getInitialTilePosition() const override;
      virtual int           getInitialHitPoints() const override;
      virtual int           getInitialResources() const override;

      virtual int getKillCount() const override;
      virtual int getAcidSporeCount() const override;
      virtual int getInterceptorCount() const override;
      virtual int getScarabCount() const override;
      virtual int getSpiderMineCount() const override;
      virtual int getGroundWeaponCooldown() const override;
      virtual int getAirWeaponCooldown() const override;
      virtual int getSpellCooldown() const override;
      virtual int getDefenseMatrixPoints() const override;

      virtual int getDefenseMatrixTimer() const override;
      virtual int getEnsnareTimer() const override;
      virtual int getIrradiateTimer() const override;
      virtual int getLockdownTimer() const override;
      virtual int getMaelstromTimer() const override;
      virtual int getOrderTimer() const override;
      virtual int getPlagueTimer() const override;
      virtual int getRemoveTimer() const override;
      virtual int getStasisTimer() const override;
      virtual int getStimTimer() const override;

      virtual UnitType      getBuildType() const override;
      virtual UnitType::set getTrainingQueue() const override;
      virtual TechType      getTech() const override;
      virtual UpgradeType   getUpgrade() const override;
      virtual int           getRemainingBuildTime() const override;
      virtual int           getRemainingTrainTime() const override;
      virtual int           getRemainingResearchTime() const override;
      virtual int           getRemainingUpgradeTime() const override;
      virtual Unit*         getBuildUnit() const override;

      virtual Unit*     getTarget() const override;
      virtual Position  getTargetPosition() const override;
      virtual Order     getOrder() const override;
      virtual Unit*     getOrderTarget() const override;
      virtual Position  getOrderTargetPosition() const override;
      virtual Order     getSecondaryOrder() const override;
      virtual Position  getRallyPosition() const override;
      virtual Unit*     getRallyUnit() const override;
      virtual Unit*     getAddon() const override;
      virtual Unit*     getNydusExit() const override;
      virtual Unit*     getPowerUp() const override;

      virtual Unit*     getTransport() const override;
      virtual Unitset   getLoadedUnits() const override;
      virtual Unit*     getCarrier() const override;
      virtual Unitset   getInterceptors() const override;
      virtual Unit*     getHatchery() const override;
      virtual Unitset   getLarva() const override;

      virtual bool hasNuke() const override;
      virtual bool isAccelerating() const override;
      virtual bool isAttackFrame() const override;
      virtual bool isAttacking() const override;
      virtual bool isBeingGathered() const override;
      virtual bool isBeingHealed() const override;
      virtual bool isBlind() const override;
      virtual bool isBraking() const override;
      virtual bool isBurrowed() const override;
      virtual bool isCarryingGas() const override;
      virtual bool isCarryingMinerals() const override;
      virtual bool isCloaked() const override;
      virtual bool isCompleted() const override;
      virtual bool isConstructing() const override;
      virtual bool isDetected() const override;
      virtual bool isGatheringGas() const override;
      virtual bool isGatheringMinerals() const override;
      virtual bool isHallucination() const override;
      virtual bool isIdle() const override;
      virtual bool isInterruptible() const override;
      virtual bool isInvincible() const override;
      virtual bool isLifted() const override;
      virtual bool isMorphing() const override;
      virtual bool isMoving() const override;
      virtual bool isParasited() const override;
      virtual bool isSelected() const override;
      virtual bool isStartingAttack() const override;
      virtual bool isStuck() const override;
      virtual bool isTraining() const override;
      virtual bool isUnderAttack() const override;
      virtual bool isUnderDarkSwarm() const override;
      virtual bool isUnderDisruptionWeb() const override;
      virtual bool isUnderStorm() const override;
      virtual bool isUnpowered() const override;
      virtual bool isVisible(Player* player = nullptr) const override;

      virtual bool canCommand() const override;
      virtual bool canTargetUnit(const Unit* targetUnit, bool checkCommandibility = true) const override;

      virtual bool canAttackMove(bool checkCommandibility = true) const override;
      virtual bool canAttackUnit(bool checkCommandibility = true) const override;
      virtual bool canAttackUnit(Unit* targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const override;
      virtual bool canBuild(bool checkCommandibility = true) const override;
      virtual bool canBuild(UnitType uType, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const override;
      virtual bool canBuild(UnitType uType, BWAPI::TilePosition tilePos, bool checkTargetUnitType = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const override;
      virtual bool canBuildAddon(bool checkCommandibility = true) const override;
      virtual bool canBuildAddon(UnitType uType, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const override;
      virtual bool canTrain(bool checkCommandibility = true) const override;
      virtual bool canTrain(UnitType uType, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const override;
      virtual bool canMorph(bool checkCommandibility = true) const override;
      virtual bool canMorph(UnitType uType, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const override;
      virtual bool canResearch(bool checkCommandibility = true) const override;
      virtual bool canResearch(TechType type, bool checkCanIssueCommandType = true) const override;
      virtual bool canUpgrade(bool checkCommandibility = true) const override;
      virtual bool canUpgrade(UpgradeType type, bool checkCanIssueCommandType = true) const override;
      virtual bool canSetRallyPosition(bool checkCommandibility = true) const override;
      virtual bool canSetRallyUnit(bool checkCommandibility = true) const override;
      virtual bool canSetRallyUnit(const Unit* targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const override;
      virtual bool canMove(bool checkCommandibility = true) const override;
      virtual bool canPatrol(bool checkCommandibility = true) const override;
      virtual bool canFollow(bool checkCommandibility = true) const override;
      virtual bool canFollow(const Unit* targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const override;
      virtual bool canGather(bool checkCommandibility = true) const override;
      virtual bool canGather(const Unit* targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const override;
      virtual bool canReturnCargo(bool checkCommandibility = true) const override;
      virtual bool canHoldPosition(bool checkCommandibility = true) const override;
      virtual bool canStop(bool checkCommandibility = true) const override;
      virtual bool canRepair(bool checkCommandibility = true) const override;
      virtual bool canRepair(const Unit* targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const override;
      virtual bool canBurrow(bool checkCommandibility = true) const override;
      virtual bool canUnburrow(bool checkCommandibility = true) const override;
      virtual bool canCloak(bool checkCommandibility = true) const override;
      virtual bool canDecloak(bool checkCommandibility = true) const override;
      virtual bool canSiege(bool checkCommandibility = true) const override;
      virtual bool canUnsiege(bool checkCommandibility = true) const override;
      virtual bool canLift(bool checkCommandibility = true) const override;
      virtual bool canLand(bool checkCommandibility = true) const override;
      virtual bool canLand(TilePosition target, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const override;
      virtual bool canLoad(bool checkCommandibility = true) const override;
      virtual bool canLoad(const Unit* targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const override;
      virtual bool canUnloadWithOrWithoutTarget(bool checkCommandibility = true) const override;
      virtual bool canUnloadAtPosition(Position targDropPos, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const override;
      virtual bool canUnload(bool checkCommandibility = true) const override;
      virtual bool canUnload(const Unit* targetUnit, bool checkCanTargetUnit = true, bool checkPosition = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const override;
      virtual bool canUnloadAll(bool checkCommandibility = true) const override;
      virtual bool canUnloadAllPosition(bool checkCommandibility = true) const override;
      virtual bool canUnloadAllPosition(Position targDropPos, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const override;
      virtual bool canRightClickPosition(bool checkCommandibility = true) const override;
      virtual bool canRightClickUnit(bool checkCommandibility = true) const override;
      virtual bool canRightClickUnit(Unit* targetUnit, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const override;
      virtual bool canHaltConstruction(bool checkCommandibility = true) const override;
      virtual bool canCancelConstruction(bool checkCommandibility = true) const override;
      virtual bool canCancelAddon(bool checkCommandibility = true) const override;
      virtual bool canCancelTrain(bool checkCommandibility = true) const override;
      virtual bool canCancelTrainSlot(bool checkCommandibility = true) const override;
      virtual bool canCancelTrainSlot(int slot, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const override;
      virtual bool canCancelMorph(bool checkCommandibility = true) const override;
      virtual bool canCancelResearch(bool checkCommandibility = true) const override;
      virtual bool canCancelUpgrade(bool checkCommandibility = true) const override;
      virtual bool canUseTechWithOrWithoutTarget(bool checkCommandibility = true) const override;
      virtual bool canUseTechWithOrWithoutTarget(BWAPI::TechType tech, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const override;
      virtual bool canUseTechWithoutTarget(BWAPI::TechType tech, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const override;
      virtual bool canUseTechUnit(BWAPI::TechType tech, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const override;
      virtual bool canUseTechUnit(BWAPI::TechType tech, const Unit* targetUnit, bool checkCanTargetUnit = true, bool checkTargetsUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const override;
      virtual bool canUseTechPosition(BWAPI::TechType tech, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const override;
      virtual bool canPlaceCOP(bool checkCommandibility = true) const override;
      virtual bool canPlaceCOP(TilePosition target, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const override;

      virtual bool canIssueCommandType(BWAPI::UnitCommandType ct, bool checkCommandibility = true) const override;
      virtual bool canIssueCommand(UnitCommand command, bool checkCanUseTechUnitOnUnits = true, bool checkCanBuildUnitType = true, bool checkCanTargetUnit = true, bool checkCanIssueCommandType = true, bool checkCommandibility = true) const override;

      virtual bool issueCommand(UnitCommand command) override;
  };
}
