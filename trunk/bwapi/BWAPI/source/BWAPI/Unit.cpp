#include "Unit.h"

#include "../BWAPI/UnitPrototypeDefinitions.h"
#include "../BWAPI/Player.h"
#include "../BWAPI/Globals.h"
#include "../BWAPI/CommandTrain.h"
#include "../BWAPI/CommandRightClick.h"

#include "../BW/UnitData.h"
#include "../BW/Offsets.h"
#include "../BW/UnitTypes.h"

#include <math.h>

namespace BWAPI
{
  //----------------------------- CONSTRUCTOR -----------------------------------
  Unit::Unit(BW::UnitData* unitData, 
             BW::UnitData* originalUnitData,
             BW::UnitData* unitDataLocal)
    :bwUnitData(unitData)
    ,bwOriginalUnitData(originalUnitData)
    ,bwUnitDataLocal(unitDataLocal)
  {
  }
  //----------------------------- DESTRUCTOR -----------------------------------
  Unit::~Unit()
  {
  }
  //-------------------------- GET HEALTH POINTS --------------------------------
  u16 Unit::getHealthPoints() const
  {
    return bwUnitData->healthPoints;
  }
  //----------------------- GET HEALT POINTS FACTION ----------------------------
  u8 Unit::getHealthPointsFraction() const
  {
    return bwUnitData->healthPointsFraction;
  }
  //-------------------------- GET HEALTH POINTS --------------------------------
  u16 Unit::getShieldPoints() const
  {
    return bwUnitData->shieldPoints;
  }
  //----------------------- GET HEALT POINTS FACTION ----------------------------
  u8 Unit::getShieldPointsFraction() const
  {
    return bwUnitData->shieldPointsFraction;
  }
  //--------------------------  GET UNIT PROTOTYPE -----------------------------
  const UnitPrototype* const Unit::getPrototype() const
  {
     switch (this->bwUnitData->unitID)
     {
       case BW::UnitType::Terran_SCV                : return BWAPI::Prototypes::SCV;
       case BW::UnitType::Terran_CommandCenter      : return BWAPI::Prototypes::CommandCenter;
       case BW::UnitType::Terran_SupplyDepot        : return BWAPI::Prototypes::SupplyDepot;
       case BW::UnitType::Terran_Barracks           : return BWAPI::Prototypes::Barracks;
       case BW::UnitType::Terran_Marine             : return BWAPI::Prototypes::Marine;
       case BW::UnitType::Terran_Firebat            : return BWAPI::Prototypes::Firebat;
       case BW::UnitType::Terran_Ghost              : return BWAPI::Prototypes::Ghost;
       case BW::UnitType::Terran_SiegeTankSiegeMode : return BWAPI::Prototypes::SiegeTank_SiegeMode;
       case BW::UnitType::Terran_SiegeTankTankMode  : return BWAPI::Prototypes::SiegeTank_TankMode;

       case BW::UnitType::Terran_Medic              : return BWAPI::Prototypes::Medic;
       case BW::UnitType::Terran_Goliath            : return BWAPI::Prototypes::Goliath;
       case BW::UnitType::Terran_GoliathTurret      : return BWAPI::Prototypes::GoliathTurret;
       case BW::UnitType::Terran_Wraith             : return BWAPI::Prototypes::Wraith;
       case BW::UnitType::Terran_BattleCruiser      : return BWAPI::Prototypes::BattleCruiser;
       case BW::UnitType::Terran_NuclearMissile     : return BWAPI::Prototypes::NuclearMissile;
       /** Protoss*/
       case BW::UnitType::Protoss_Probe             : return BWAPI::Prototypes::Probe;
       case BW::UnitType::Protoss_Zealot            : return BWAPI::Prototypes::Zealot;
       case BW::UnitType::Protoss_HighTemplar       : return BWAPI::Prototypes::HighTemplar;
       case BW::UnitType::Protoss_Dragoon           : return BWAPI::Prototypes::Dragoon;
       case BW::UnitType::Protoss_Nexus             : return BWAPI::Prototypes::Nexus;
       case BW::UnitType::Protoss_Pylon             : return BWAPI::Prototypes::Pylon;
       case BW::UnitType::Protoss_Gateway           : return BWAPI::Prototypes::Gateway;
       case BW::UnitType::Protoss_CyberneticsCore   : return BWAPI::Prototypes::CyberneticsCore;
       /** Zerg */
       case BW::UnitType::Zerg_Queen                : return BWAPI::Prototypes::Queen;
       case BW::UnitType::Zerg_Mutalisk             : return BWAPI::Prototypes::Mutalisk;
       case BW::UnitType::Zerg_Larva                : return BWAPI::Prototypes::Larva;
       case BW::UnitType::Zerg_Drone                : return BWAPI::Prototypes::Drone;
       case BW::UnitType::Zerg_Zergling             : return BWAPI::Prototypes::Zergling;
       case BW::UnitType::Zerg_Overlord             : return BWAPI::Prototypes::Overlord;
       case BW::UnitType::Zerg_Hatchery             : return BWAPI::Prototypes::Hatchery;
       case BW::UnitType::Zerg_SpawningPool         : return BWAPI::Prototypes::SpawningPool;
       /** Neutral */
       case BW::UnitType::Resource_MineralPatch1    : return BWAPI::Prototypes::Minerals1;
       case BW::UnitType::Resource_MineralPatch2    : return BWAPI::Prototypes::Minerals2;
       case BW::UnitType::Resource_MineralPatch3    : return BWAPI::Prototypes::Minerals3;
       case BW::UnitType::Resource_VespeneGeyser    : return BWAPI::Prototypes::VaspineGayser;
       default : return NULL;
     }
  }
  //------------------------------ CAN ORDER -----------------------------------
  bool Unit::canOrder(const AbilityPrototype* const ability, Unit* target) const
  {
    if (!this->getPrototype()->canOrder(ability, target))
      return false;

    if (this == target &&
      !(ability->getTargetFlags() & TargetType::TARGET_SELF))
      return false;

    if (this->getOwner() == target->getOwner() &&
      !(ability->getTargetFlags() & TargetType::TARGET_MY_UNITS))
      return false;

    if (this->getOwner() == target->getOwner() &&
      !(ability->getTargetFlags() & TargetType::TARGET_OPPONENTS_UNITS))
      return false;

    return true;
  }
  //------------------------------ CAN ORDER -----------------------------------
  bool Unit::canOrder(const AbilityPrototype* const ability, const BW::Position& target) const
  {
    if (!this->getPrototype()->canOrder(ability, target))
      return false;
    return true;
  }
  //-------------------------------- ORDER -------------------------------------
  void Unit::order(const AbilityPrototype* const ability, Unit* target)
  {
  }
 //--------------------------------- ORDER ------------------------------------
  void Unit::order(const AbilityPrototype* const ability, const BW::Position& target)
  {
  }
  //--------------------------------- ORDER ------------------------------------
  void Unit::order(int commandCode, const BW::Position& target)
  {
  }
  //------------------------------- GET OWNER ----------------------------------
  Player* Unit::getOwner() const
  {
    return Broodwar.players[this->bwUnitData->playerID];
  }
  //-------------------------------- IS VALID ----------------------------------
  bool Unit::isValid() const
  {
    return this->getHealthPoints() > 0 || 
           this->getHealthPointsFraction() > 0;
  }
  //-------------------------------- GET POSITION ------------------------------
  const BW::Position& Unit::getPosition() const
  {
    return this->bwUnitData->position;
  }
  //-------------------------------- GET TATGET --------------------------------
  Unit* Unit::getTarget()
  {
    if (this->getRawData()->orderTargetUnit == NULL)
      return NULL;
    return Unit::BWUnitToBWAPIUnit(this->getRawData()->orderTargetUnit);
  }
  //-------------------------------- GET RAW DATA ------------------------------
  BW::UnitData *Unit::getRawData()
  {
    return this->bwUnitData;
  }
  //-------------------------------- GET RAW DATA ------------------------------
  const BW::UnitData *Unit::getRawData() const
  {
    return this->bwUnitData;
  }
  //------------------------------ GET RAW DATA LOCAL --------------------------
  BW::UnitData *Unit::getRawDataLocal()
  {
    return this->bwUnitDataLocal;
  }
  //------------------------------ GET RAW DATA LOCAL --------------------------
  const BW::UnitData *Unit::getRawDataLocal() const
  {
    return this->bwUnitDataLocal;
  }
  //------------------------------ GET ORIGINAL RAW DATA -----------------------
  BW::UnitData *Unit::getOriginalRawData()
  {
    return this->bwOriginalUnitData;
  }
  //------------------------------ GET ORIGINAL RAW DATA -----------------------
  const BW::UnitData *Unit::getOriginalRawData() const
  {
    return this->bwOriginalUnitData;
  }
  //-------------------------------- GET ORDER ID ------------------------------
  BW::OrderID::Enum Unit::getOrderID() const
  {
    return this->getRawData()->orderID;
  }
  //-------------------------------- GET ORDER ID ------------------------------
  BW::OrderID::Enum Unit::getOrderIDLocal() const
  {
    return this->getRawDataLocal()->orderID;
  }
  //------------------------------- GET DISTANCE -------------------------------

  #pragma warning(push)
  #pragma warning(disable:4244)
  u16 Unit::getDistance(Unit *unit) const
  {
   if (this->getPosition().y - this->getPrototype()->dimensionUp() <= unit->getPosition().y + unit->getPrototype()->dimensionDown())
     if (this->getPosition().y + this->getPrototype()->dimensionDown() >= unit->getPosition().y - unit->getPrototype()->dimensionUp())
       if (this->getPosition().x > unit->getPosition().x)
         return this->getPosition().x - this->getPrototype()->dimensionLeft() - unit->getPosition().x - unit->getPrototype()->dimensionRight();
       else
         return unit->getPosition().x - unit->getPrototype()->dimensionRight() - this->getPosition().x - this->getPrototype()->dimensionLeft();

   if (this->getPosition().x - this->getPrototype()->dimensionLeft() <= unit->getPosition().x + unit->getPrototype()->dimensionRight())
     if (this->getPosition().x + this->getPrototype()->dimensionRight() >= unit->getPosition().x - unit->getPrototype()->dimensionLeft())
       if (this->getPosition().y > unit->getPosition().y)
         return this->getPosition().y - this->getPrototype()->dimensionUp() - unit->getPosition().y - unit->getPrototype()->dimensionDown();
       else
         return unit->getPosition().y - unit->getPrototype()->dimensionDown() - this->getPosition().y - this->getPrototype()->dimensionUp();

   if (this->getPosition().x > unit->getPosition().x)
     if (this->getPosition().y > unit->getPosition().y)
       return this->getDistance(this->getPosition().x - this->getPrototype()->dimensionLeft(),
                                this->getPosition().y - this->getPrototype()->dimensionUp(),
                                unit->getPosition().x + unit->getPrototype()->dimensionRight(),
                                unit->getPosition().y + unit->getPrototype()->dimensionDown());
     else
       return this->getDistance(this->getPosition().x - this->getPrototype()->dimensionLeft(),
                                this->getPosition().y + this->getPrototype()->dimensionDown(),
                                unit->getPosition().x + unit->getPrototype()->dimensionRight(),
                                unit->getPosition().y - unit->getPrototype()->dimensionUp());
   else
     if (this->getPosition().y > unit->getPosition().y)
       return this->getDistance(this->getPosition().x + this->getPrototype()->dimensionRight(),
                                this->getPosition().y - this->getPrototype()->dimensionUp(),
                                unit->getPosition().x - unit->getPrototype()->dimensionLeft(),
                                unit->getPosition().y + unit->getPrototype()->dimensionDown());
     else
       return this->getDistance(this->getPosition().x + this->getPrototype()->dimensionRight(),
                                this->getPosition().y + this->getPrototype()->dimensionDown(),
                                unit->getPosition().x - unit->getPrototype()->dimensionLeft(),
                                unit->getPosition().y - unit->getPrototype()->dimensionUp());
  }
  //-------------------------------- GET CENTER DISTANCE -----------------------
  u16 Unit::getCenterDistance(Unit *unit) const
  {
    return this->getDistance(this->getPosition().x, 
                             this->getPosition().y,
                             unit->getPosition().x, 
                             unit->getPosition().y);
  }
  //-------------------------------- GET DISTANCE ------------------------------
  u16 Unit::getDistance(int x1, int y1, int x2, int y2) const
  {
    return sqrt((long double)(x1 - x2)*(x1 - x2) +
                (long double)(y1 - y2)*(y1 - y2));
  }
  #pragma warning(pop)
  //------------------------------ HAS EMPTY QUEUE -----------------------------
  bool Unit::hasEmptyBuildQueue(void)
  {
     return this->getBuildQueue()[this->getBuildQueueSlot()] == 0xe4;
  }
  //------------------------------ HAS EMPTY QUEUE -----------------------------
  bool Unit::hasEmptyBuildQueueLocal(void)
  {
     return this->getBuildQueueLocal()[this->getBuildQueueSlotLocal()] == 0xe4;
  }
  //-------------------------------- ORDER MOVE --------------------------------
  void Unit::orderRightClick(u16 x,u16 y)
  {
    this->orderSelect();
    Broodwar.IssueCommand((PBYTE)&BW::Orders::RightClick(BW::Position(x, y)), sizeof(BW::Orders::RightClick)); 
  }
  //-------------------------------- ORDER MOVE --------------------------------
  void Unit::orderRightClick(Unit *target)
  {
    this->orderSelect();
    Broodwar.IssueCommand((PBYTE)&BW::Orders::RightClick(target), sizeof(BW::Orders::RightClick)); 
    Broodwar.addToCommandBuffer(new CommandRightClick(this, target));
  }
  //-------------------------------- ORDER SELECT --------------------------------
  void Unit::orderSelect()
  {
    BW::UnitData * * list = new BW::UnitData * [2];
    list[0] = this->getOriginalRawData();
	   list[1] = NULL;
    int one = 1;
    void (_stdcall* selectUnitsHelperSTD)(int, BW::UnitData * *, bool, bool) = (void (_stdcall*) (int, BW::UnitData * *, bool, bool))0x0049AB90;
    selectUnitsHelperSTD(one, list, true, true);
    //Broodwar.IssueCommand((PBYTE)&BW::Orders::SelectSingle(this),sizeof(BW::Orders::SelectSingle)); 
  }
  //---------------------------------- GET TYPE --------------------------------
  BW::UnitType::Enum Unit::getType()
  {
    return this->getRawData()->unitID;
  }
  //---------------------------------- GET QUEUE -------------------------------
  BW::UnitType::Enum* Unit::getBuildQueue()
  {
    return this->getRawData()->buildQueue;
  }
  //-------------------------------- GET QUEUE LOCAL  --------------------------
  BW::UnitType::Enum* Unit::getBuildQueueLocal()
  {
    return this->getRawDataLocal()->buildQueue;
  }
  //----------------------------------- TRAIN UNIT -----------------------------
  void Unit::trainUnit(UnitPrototype *type)
  {
    Broodwar.IssueCommand((PBYTE)&BW::Orders::TrainUnit(type->getUnitID()), 0x3);
    Broodwar.addToCommandBuffer(new CommandTrain(this, type));
  }
  //-------------------------------- GET QUEUE SLOT ----------------------------
  u8 Unit::getBuildQueueSlot()
  {
    return this->getRawData()->buildQueueSlot;
  }
  //------------------------------- GET QUEUE SLOT LOCAL -----------------------
  u8 Unit::getBuildQueueSlotLocal()
  {
    return this->getOriginalRawData()->buildQueueSlot;
  }
  //----------------------------------------------------------------------------
  Unit* Unit::BWUnitToBWAPIUnit(BW::UnitData* unit)
  {
    if (unit == NULL)
      return NULL;
    return Broodwar.getUnit(((int)unit - (int)BW::BWXFN_UnitNodeTable)/336);
  }
  //----------------------------------------------------------------------------
};
