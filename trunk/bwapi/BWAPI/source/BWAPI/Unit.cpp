#include "Unit.h"

#include "../BWAPI/TargetType.h"
#include "../BWAPI/AbilityPrototype.h"
#include "../BWAPI/UnitPrototype.h"
#include "../BWAPI/UnitPrototypeDefinitions.h"
#include "../BWAPI/Player.h"
#include "../BWAPI/Globals.h"
#include "../BWAPI/CommandTrain.h"
#include "../BWAPI/CommandRightClick.h"

#include "../BW/Unit.h"
#include "../BW/Offsets.h"
#include "../BW/UnitTypes.h"

#include <math.h>

namespace BWAPI
{
  //----------------------------- CONSTRUCTOR -----------------------------------
  Unit::Unit(BW::Unit* unitData, 
             BW::Unit* originalUnit,
             BW::Unit* unitDataLocal)
  :bwUnit(unitData)
  ,bwOriginalUnit(originalUnit)
  ,bwUnitLocal(unitDataLocal)
  {
  }
  //----------------------------- DESTRUCTOR -----------------------------------
  Unit::~Unit()
  {
  }
  //-------------------------- GET HEALTH POINTS --------------------------------
  u16 Unit::getHealthPoints() const
  {
    return bwUnit->healthPoints;
  }
  //----------------------- GET HEALT POINTS FACTION ----------------------------
  u8 Unit::getHealthPointsFraction() const
  {
    return bwUnit->healthPointsFraction;
  }
  //-------------------------- GET HEALTH POINTS --------------------------------
  u16 Unit::getShieldPoints() const
  {
    return bwUnit->shieldPoints;
  }
  //----------------------- GET HEALT POINTS FACTION ----------------------------
  u8 Unit::getShieldPointsFraction() const
  {
    return bwUnit->shieldPointsFraction;
  }
  //--------------------------  GET UNIT PROTOTYPE -----------------------------
  const UnitPrototype* Unit::getPrototype() const
  {
    if (this->getType() < 228)
      return BWAPI::Prototypes::unitIDToPrototypeTable[this->getType()];
    else 
      return NULL;
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
    return Broodwar.players[this->bwUnit->playerID];
  }
  //-------------------------------- IS VALID ----------------------------------
  bool Unit::isValid() const
  {
    if (this->isMineral())
      return  this->getOriginalRawData()->mainOrderState == 0;
    else         
      return (
               this->getHealthPoints() > 0 || 
               this->getHealthPointsFraction() > 0
             ) &&
             this->getPrototype() != NULL;
  }
  //-------------------------------- IS VALID ----------------------------------
  bool Unit::isReady() const
  {
    return this->isValid() &&
           this->getRawData()->remainingBuildTime == 0 &&
           this->getOrderID() != BW::OrderID::Finishing;
  }
  //-------------------------------- GET POSITION ------------------------------
  const BW::Position& Unit::getPosition() const
  {
    return this->bwUnit->position;
  }
  //-------------------------------- GET TATGET --------------------------------
  Unit* Unit::getTarget()
  {
    return Unit::BWUnitToBWAPIUnit(this->getRawData()->orderTargetUnit);
  }
  //----------------------------- GET TATGET LCCAL -----------------------------
  Unit* Unit::getTargetLocal()
  {
    return Unit::BWUnitToBWAPIUnit(this->getRawDataLocal()->orderTargetUnit);
  }
  //-------------------------------- GET RAW DATA ------------------------------
  BW::Unit *Unit::getRawData()
  {
    return this->bwUnit;
  }
  //-------------------------------- GET RAW DATA ------------------------------
  const BW::Unit *Unit::getRawData() const
  {
    return this->bwUnit;
  }
  //------------------------------ GET RAW DATA LOCAL --------------------------
  BW::Unit *Unit::getRawDataLocal()
  {
    return this->bwUnitLocal;
  }
  //------------------------------ GET RAW DATA LOCAL --------------------------
  const BW::Unit *Unit::getRawDataLocal() const
  {
    return this->bwUnitLocal;
  }
  //------------------------------ GET ORIGINAL RAW DATA -----------------------
  BW::Unit *Unit::getOriginalRawData()
  {
    return this->bwOriginalUnit;
  }
  //------------------------------ GET ORIGINAL RAW DATA -----------------------
  const BW::Unit *Unit::getOriginalRawData() const
  {
    return this->bwOriginalUnit;
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
   u32 result;
   if (unit == this)
     return 0;
   if (this->getPosition().y - this->getPrototype()->dimensionUp() <= unit->getPosition().y + unit->getPrototype()->dimensionDown())
     if (this->getPosition().y + this->getPrototype()->dimensionDown() >= unit->getPosition().y - unit->getPrototype()->dimensionUp())
       if (this->getPosition().x > unit->getPosition().x)
         result = this->getPosition().x - this->getPrototype()->dimensionLeft() - unit->getPosition().x - unit->getPrototype()->dimensionRight();
       else
         result = unit->getPosition().x - unit->getPrototype()->dimensionRight() - this->getPosition().x - this->getPrototype()->dimensionLeft();

   if (this->getPosition().x - this->getPrototype()->dimensionLeft() <= unit->getPosition().x + unit->getPrototype()->dimensionRight())
     if (this->getPosition().x + this->getPrototype()->dimensionRight() >= unit->getPosition().x - unit->getPrototype()->dimensionLeft())
       if (this->getPosition().y > unit->getPosition().y)
         result = this->getPosition().y - this->getPrototype()->dimensionUp() - unit->getPosition().y - unit->getPrototype()->dimensionDown();
       else
         result = unit->getPosition().y - unit->getPrototype()->dimensionDown() - this->getPosition().y - this->getPrototype()->dimensionUp();

   if (this->getPosition().x > unit->getPosition().x)
     if (this->getPosition().y > unit->getPosition().y)
       result = this->getDistance(this->getPosition().x - this->getPrototype()->dimensionLeft(),
                                  this->getPosition().y - this->getPrototype()->dimensionUp(),
                                  unit->getPosition().x + unit->getPrototype()->dimensionRight(),
                                  unit->getPosition().y + unit->getPrototype()->dimensionDown());
     else
       result = this->getDistance(this->getPosition().x - this->getPrototype()->dimensionLeft(),
                                 this->getPosition().y + this->getPrototype()->dimensionDown(),
                                 unit->getPosition().x + unit->getPrototype()->dimensionRight(),
                                 unit->getPosition().y - unit->getPrototype()->dimensionUp());
   else
     if (this->getPosition().y > unit->getPosition().y)
       result = this->getDistance(this->getPosition().x + this->getPrototype()->dimensionRight(),
                                this->getPosition().y - this->getPrototype()->dimensionUp(),
                                unit->getPosition().x - unit->getPrototype()->dimensionLeft(),
                                unit->getPosition().y + unit->getPrototype()->dimensionDown());
     else
       result = this->getDistance(this->getPosition().x + this->getPrototype()->dimensionRight(),
                                this->getPosition().y + this->getPrototype()->dimensionDown(),
                                unit->getPosition().x - unit->getPrototype()->dimensionLeft(),
                                unit->getPosition().y - unit->getPrototype()->dimensionUp());
   if (result > 0)
     return result;
   else
     return 0;
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
  //------------------------------- ORDER RIGHT CLICK -------------------------
  void Unit::orderRightClick(u16 x,u16 y)
  {
    this->orderSelect();
    Broodwar.IssueCommand((PBYTE)&BW::Orders::RightClick(BW::Position(x, y)), sizeof(BW::Orders::RightClick)); 
    Broodwar.addToCommandBuffer(new CommandRightClick(this, BW::Position(x, y)));
  }
  //------------------------------- ORDER RIGHT CLICK -------------------------
  void Unit::orderRightClick(Unit *target)
  {
    this->orderSelect();
    Broodwar.IssueCommand((PBYTE)&BW::Orders::RightClick(target), sizeof(BW::Orders::RightClick)); 
    Broodwar.addToCommandBuffer(new CommandRightClick(this, target));
  }
  //------------------------------------ BUILD --------------------------------
  void Unit::build(u16 tileX, u16 tileY, UnitPrototype *type)
  {
    this->orderSelect();
    Broodwar.IssueCommand((PBYTE)&BW::Orders::MakeBuilding(tileX, tileY, type->getUnitID()), sizeof(BW::Orders::MakeBuilding)); 
    Broodwar.addToCommandBuffer(new CommandRightClick(this, BW::Position(tileX*BW::TileSize, tileY*BW::TileSize)));
  }
  //------------------------------- ORDER SELECT ------------------------------
  void Unit::orderSelect()
  {
    /*BW::Unit * * list = new BW::Unit * [2];
    list[0] = this->getOriginalRawData();
	   list[1] = NULL;
    int one = 1;
    void (_stdcall* selectUnitsHelperSTD)(int, BW::Unit * *, bool, bool) = (void (_stdcall*) (int, BW::Unit * *, bool, bool))0x0049AB90;
    selectUnitsHelperSTD(one, list, true, true);*/
    Broodwar.IssueCommand((PBYTE)&BW::Orders::SelectSingle(this),sizeof(BW::Orders::SelectSingle)); 
  }
  //---------------------------------- GET TYPE --------------------------------
  BW::UnitType::Enum Unit::getType() const
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
    this->orderSelect();
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
  #pragma warning(push)
  #pragma warning(disable:4311)
  Unit* Unit::BWUnitToBWAPIUnit(BW::Unit* unit)
  {
    if (unit == NULL)
      return NULL;
    return Broodwar.getUnit(((int)unit - (int)BW::BWXFN_UnitNodeTable)/336);
  }
  #pragma warning (pop)
  //----------------------------------------------------------------------------
  bool Unit::isMineral() const
  {
     return this->getType() == BW::UnitType::Resource_MineralPatch1 ||
            this->getType() == BW::UnitType::Resource_MineralPatch2 ||
            this->getType() == BW::UnitType::Resource_MineralPatch3;
  }
  //----------------------------------------------------------------------------
  std::string Unit::getName() const
  {
    if (this->getPrototype() != NULL)
     return (std::string)"(" + this->getPrototype()->getName() + ")";
    char message[30];
    sprintf(message,"(Unknown name - unit id = %d)", this->getType());
    std::string ret = message;
    return ret;
  }
  //----------------------------------------------------------------------------
};
