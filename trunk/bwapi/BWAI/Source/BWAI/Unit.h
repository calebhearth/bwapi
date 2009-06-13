#pragma once

#include <BWAPI.h>
#include <BW/TilePosition.h>
#include <list>

namespace BWAPI  { class Unit; }
namespace BWAI  { class Task; }
namespace BWAI  { class Expansion; }
namespace BWAI  { class Player; }

namespace BWAI
{
  /** 
   * Proxy of BWAPI unit type with included information that uses BWAI
   */
  class Unit
  {
    public :
      Unit(BWAPI::Unit *unit);
      Expansion* expansion;
      static Unit* BWAPIUnitToBWAIUnit(BWAPI::Unit* unit);
      /** 
       * Gets #bwUnit->BW#Unit#targetUnit. 
       * This is similar to BWAPI#Unit#getTarget, it just get's the unit from ai scope
       */
      BWAPI::UnitType getType() const;
      int getHealthPoints() const; 
      int getShieldPoints() const; 
      const BW::Position& getPosition() const;
      BW::TilePosition getTilePosition() const;
      Unit* getTarget() const;
      Unit* getOrderTarget() const;
      Unit* getBuildUnit() const;
      Unit* getChild() const;
      BW::Position getTargetPosition() const;
      Player* getOwner() const;
      std::string getName() const;
      bool isMineral() const;
      bool isCompleted() const;
      bool isLifted() const;
      bool isSelected() const;
      bool isIdle() const;

      bool isTraining() const;
      std::list<BWAPI::UnitType > getTrainingQueue() const;
      int getRemainingBuildTime() const;
      int getOrderTimer() const;

      /** Gets distance of unit edges. */
      int getDistance(Unit *unit) const;
      /** Gets distance of unit center and position. */
      int getDistance(BW::Position position) const;
      /** Gets distance of unit centers. */
      int getCenterDistance(Unit *unit) const;
      /**< Gets bwUnit->BW#Unit#orderID. */
      BWAPI::Order getOrderID() const;
      /**< Gets bwUnit->BW#Unit#secondaryOrderID. */
      BWAPI::Order getSecondaryOrderID() const;

      void orderAttackLocation(BW::Position position, BWAPI::Order order);
      void orderRightClick(BW::Position position);
      void orderRightClick(Unit *target);
      void trainUnit(BWAPI::UnitType type);
      void build(BW::TilePosition position, BWAPI::UnitType type);
      void invent(BWAPI::TechType tech);
      void upgrade(BWAPI::UpgradeType upgrade);

      int lastFrameSpam;
      
      Task* getTask() const;
      void freeFromTask();
      void clearTask();
      void setTask(Task* task);
      std::list<Unit*>::iterator taskListIterator;
      BWAPI::Unit* getUnit();
    private :
      Task* task;
      BWAPI::Unit* unit;
  };
}