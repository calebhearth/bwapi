#pragma once

#include <list>

#include <BWAPI/Order.h>
#include <BWAPI/TechType.h>
#include <BWAPI/UpgradeType.h>
#include <BWAPI/UnitType.h>
#include <BWAPI/Position.h>
#include <BWAPI/TilePosition.h>

namespace BWAPI
{
  class Player;
  class Unit
  {
  public:
    virtual Player* getOwner() const=0;
    virtual UnitType getType() const=0;
    virtual int health() const=0;
    virtual int shield() const=0;
    virtual int energy() const=0;
    virtual int getKillCount() const=0;
    virtual int groundWeaponCooldown() const=0;
    virtual int airWeaponCooldown() const=0;
    virtual int spellCooldown() const=0;
    virtual int defenseMatrixPoints() const=0;
    virtual int defenseMatrixTimer() const=0;
    virtual int stimTimer() const=0;
    virtual int ensnareTimer() const=0;
    virtual int lockdownTimer() const=0;
    virtual int irradiateTimer() const=0;
    virtual int stasisTimer() const=0;
    virtual int plagueTimer() const=0;
    virtual int maelstromTimer() const=0;
    virtual Position getPosition() const=0;
    virtual TilePosition getTilePosition() const=0;
    virtual int currentDirection() const=0;

    virtual Unit* getTarget() const=0;
    virtual Position getTargetPosition() const=0;
    virtual Order getOrder() const=0;
    virtual Unit* getOrderTarget() const=0;
    virtual int getOrderTimer() const=0;
    virtual Order getSecondaryOrder() const=0;
    virtual Unit* getBuildUnit() const=0;
    virtual int getRemainingBuildTime() const=0;
    virtual int getRemainingTrainTime() const=0;
    virtual Unit* getChild() const=0;
    virtual std::list<UnitType > getTrainingQueue() const=0;
    virtual std::list<Unit*> getLoadedUnits() const=0;
    virtual TechType getTech() const=0;
    virtual UpgradeType getUpgrade() const=0;
    virtual int getRemainingResearchTime() const=0;
    virtual int getRemainingUpgradeTime() const=0;
    virtual Position getRallyPosition() const=0;
    virtual Unit* getRallyUnit() const=0;

    virtual bool isAccelerating() const=0;
    virtual bool isBeingConstructed() const=0;
    virtual bool isBeingHealed() const=0;
    virtual bool isBlind() const=0;
    virtual bool isBraking() const=0;
    virtual bool isBurrowed() const=0;
    virtual bool isCloaked() const=0;
    virtual bool isCompleted() const=0;
    virtual bool isConstructing() const=0;
    virtual bool isDisabled() const=0;
    virtual bool isIdle() const=0;
    virtual bool isLifted() const=0;
    virtual bool isLoaded() const=0;
    virtual bool isLockedDown() const=0;
    virtual bool isMorphing() const=0;
    virtual bool isMoving() const=0;
    virtual bool isRepairing() const=0;
    virtual bool isResearching() const=0;
    virtual bool isSelected() const=0;
    virtual bool isSieged() const=0;
    virtual bool isStartingAttack() const=0;
    virtual bool isStasised() const=0;
    virtual bool isTraining() const=0;
    virtual bool isUnderStorm() const=0;
    virtual bool isUpgrading() const=0;
    virtual bool isVisible() const=0;

    virtual bool attackMove(Position position)=0;
    virtual bool attackUnit(Unit* target)=0;
    virtual bool rightClick(Position position)=0;
    virtual bool rightClick(Unit* target)=0;
    virtual bool train(UnitType type)=0;
    virtual bool build(TilePosition position, UnitType type)=0;
    virtual bool research(TechType tech)=0;
    virtual bool upgrade(UpgradeType upgrade)=0;
    virtual bool stop()=0;
    virtual bool holdPosition()=0;
    virtual bool patrol(Position position)=0;
    virtual bool repair(Unit* target)=0;
    virtual bool morph(UnitType type)=0;
    virtual bool burrow()=0;
    virtual bool unburrow()=0;
    virtual bool siege()=0;
    virtual bool unsiege()=0;
    virtual bool cloak()=0;
    virtual bool decloak()=0;
    virtual bool lift()=0;
    virtual bool land(TilePosition position)=0;
    virtual bool load(Unit* target)=0;
    virtual bool unload(Unit* target)=0;
    virtual bool unloadAll()=0;
    virtual bool unloadAll(Position position)=0;
    virtual bool cancelConstruction()=0;
    virtual bool haltConstruction()=0;
    virtual bool cancelMorph()=0;
    virtual bool cancelTrain()=0;
    virtual bool cancelTrain(int slot)=0;
    virtual bool cancelAddon()=0;
    virtual bool cancelResearch()=0;
    virtual bool cancelUpgrade()=0;
    virtual bool useTech(TechType tech)=0;
    virtual bool useTech(TechType tech, Position position)=0;
    virtual bool useTech(TechType tech, Unit* target)=0;
  };
}
