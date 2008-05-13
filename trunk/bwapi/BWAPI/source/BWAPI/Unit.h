#pragma once

#include "../Types.h"
#include "../BW/OrderID.h"
#include "../BW/UnitTypes.h"

namespace BW { class Position; };
namespace BW { struct Unit; };
namespace BWAPI { class UnitPrototype;  };
namespace BWAPI { class AbilityPrototype;  };
namespace BWAPI { class Player;  };

namespace BWAPI
{
  /** 
  * Interface for broodwar unit, can be used to obtain any information and issue commands.
  */
  class Unit
  {
    public:
      /** 

       * @param bwUnit         #bwUnit                          
       * @param bwOriginalUnit #bwOriginalUnit
       * @param bwUnitLocal    #bwUnitLocal
       *
       */
      Unit(BW::Unit* bwUnit,
           BW::Unit* bwOriginalUnit,
           BW::Unit* bwUnitLocal);
      /** Nothing is deleted as no data are owned.*/
      ~Unit();
      /** Gets #bwUnit->BW#Unit#unitID */
      BW::UnitType::Enum getType() const;
      /** Gets #bwUnit->BW#Unit#healthPoints. */
      u16 getHealthPoints() const; 
      /** Gets #bwUnit->BW#Unit#healthPointsFraction. */
      u8 getHealthPointsFraction() const;
      /** Gets #bwUnit->BW#Unit#shieldPoints. */
      u16 getShieldPoints() const; 
      /** Gets #bwUnit->BW#Unit#shieldPointsFraction. */
      u8 getShieldPointsFraction() const;
      /** Gets #bwUnit->BW#Unit#position. */
      const BW::Position& getPosition() const;
      /** Gets #bwUnit->BW#Unit#targetUnit. */
      Unit* getTarget();
      /** Gets #bwUnit->BW#Unit#targetUnit. */
      const Unit* getTarget() const;
      /** (const version) Gets #bwUnitLocal->BW#Unit#targetUnit. */
      Unit* getTargetLocal();
      /** Gets owner of the unit defined by #bwUnit->BW#Unit#playerID. */
      const Unit* getTargetLocal() const;
      /** (const version) Gets owner of the unit defined by #bwUnit->BW#Unit#playerID. */
      Player* getOwner() const;
      /** Gets prototype of the unit defined by #bwUnit->BW#Unit#unitID. */
      const UnitPrototype* getPrototype() const;
      /** Gets #bwUnit->BW#Unit#queueSlot. */
      u8 getBuildQueueSlot();
      /** Gets #bwUnitLocal->BW#Unit#buildQueueSlot - @ref localData */
      u8 getBuildQueueSlotLocal();
      /** Gets distance of unit edges. */
      u16 getDistance(Unit *unit) const; 
      /** Gets distance of unit centers. */
      u16 getCenterDistance(Unit *unit) const; 
      /** Gets distance between two positions. */
      u16 getDistance(int x1, int y1, int x2, int y2) const; 
      /**< Gets bwUnit->BW#Unit#orderID. */
      BW::OrderID::Enum getOrderID() const;
      /**< Gets bwUnitLocal->BW#Unit#orderID - @ref localData */
      BW::OrderID::Enum getOrderIDLocal() const;
      /** Gets #bwUnit->BW#Unit#buildQueue */
      BW::UnitType::Enum *getBuildQueue();
      /** Gets #bwUnitLocal->BW#Unit#buildQueue - @ref localData*/
      BW::UnitType::Enum *getBuildQueueLocal();

      bool canOrder(const AbilityPrototype* const ability, Unit* target) const;
      bool canOrder(const AbilityPrototype* const ability, const BW::Position& target) const;
      void order(const AbilityPrototype* const ability, Unit* target);
      void order(const AbilityPrototype* const ability, const BW::Position& target);
      void order(int commandCode, const BW::Position& target);
      /** Gets if the unit is alive (it exists), it uses hp > 0 heuristic for now. */
      bool isValid() const;
      /** Gets if the unit is alive - exists and it's construction is done. */
      bool isReady() const;
      /** Gets #bwUnit */
      BW::Unit *getRawData();
      /** Gets #bwUnit (const version that returns const pointer) */
      const BW::Unit *getRawData() const;
      /** Gets #bwOriginalUnit */
      BW::Unit *getOriginalRawData();
      /** Gets #bwOriginalUnit (const version that returns const pointer) */
      const BW::Unit *getOriginalRawData() const;
      /** Gets #bwUnitLocal */
      BW::Unit *getRawDataLocal();
      /** Gets #bwUnitLocal (const version that returns const pointer)*/
      const BW::Unit *getRawDataLocal() const;
      /** Returns if the unit has empty building queue */
      bool hasEmptyBuildQueue();
      /** Returns if the unit has empty building queue in the local version  - @ref localData*/
      bool hasEmptyBuildQueueLocal();
      /**
       * Order this unit to right click on the specified location. 
       * Note that right click on location will always result in move.
       */
      void orderRightClick(u16 x,u16 y);
      /**
       * Orders this unit to right click on the specified unit.
       * Note that right click on unit can result in lot of commands (attack, gather, follow, set relly point)
       */
      void orderRightClick(Unit *target);
      /** Orders this unit to train (construct) the specified unit. */
      void trainUnit(UnitPrototype *type);
      /** Orders to build the specified building. */
      void build(u16 tileX, u16 tileY, UnitPrototype *type);
      /** Orders to select this unit (previous selection will be lost. */
      void orderSelect();
      static Unit* BWUnitToBWAPIUnit(BW::Unit* unit);
      bool isMineral() const;
      std::string getName() const;
      Unit* getNext();
      /** Updates pointer to next unit (and recursively updates that unit. */
      void updateNext();
    private:
      BW::Unit* bwUnit; /**< Pointer to our copy of of unit data table. */
      BW::Unit* bwUnitLocal; /**< Pointer to our local (precomputed) version of unit data table  @ref localData. */ 
      BW::Unit* bwOriginalUnit; /**< Pointer to broodwar unit data table. */
      Unit *next; /**< Corresponds to bwOriginalUnit->nextUnit, precomputed pointer - to optimize unit listing. */
      
  };
};

