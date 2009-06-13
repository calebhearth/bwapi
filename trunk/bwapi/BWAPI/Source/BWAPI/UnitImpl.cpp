#include "UnitImpl.h"

#include <math.h>

#include <Util/Logger.h>

#include "BWAPI/Player.h"
#include "BWAPI/GameImpl.h"
#include "Globals.h"
#include "CommandTrain.h"
#include "CommandBuild.h"
#include "CommandAttackLocation.h"
#include "CommandRightClick.h"
#include "CommandInvent.h"
#include "CommandUpgrade.h"

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
  :bwUnit(unitData)
  ,bwOriginalUnit(originalUnit)
  ,bwUnitLocal(unitDataLocal)
  ,index(index)
  ,userSelected(false)
  {
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  UnitImpl::~UnitImpl()
  {
  }
  //------------------------------------------- GET HEALTH POINTS --------------------------------------------
  u16 UnitImpl::health() const
  {
    return this->getRawDataLocal()->healthPoints;
  }
  //------------------------------------------- GET HEALTH POINTS --------------------------------------------
  u32 UnitImpl::shield() const
  {
    return this->getRawDataLocal()->shieldPoints;
  }
  //------------------------------------------- GET ENERGY POINTS --------------------------------------------
  u16 UnitImpl::energy() const
  {
    return this->getRawDataLocal()->energy; /* Unverified */
  }
  //----------------------------------------------- GET OWNER ------------------------------------------------
  Player* UnitImpl::getOwner() const
  {
    if (this->getRawDataLocal()->playerID < 12)
      return (Player*)BroodwarImpl.players[this->bwUnit->playerID];
    else 
      return NULL;
  }
  //------------------------------------------------ IS VALID ------------------------------------------------
  bool UnitImpl::isValid() const
  {
    if (this->getRawDataLocal()->playerID > 11)
       return false;
    if (this->isMineral())
      return  !this->getRawDataLocal()->orderFlags.getBit(BW::OrderFlags::willWanderAgain);
    else         
      return this->health() > 0 && this->getType()!=BWAPI::UnitTypes::None && this->getType()!=BWAPI::UnitTypes::Unknown;
  }
  //------------------------------------------------ IS VALID ------------------------------------------------
  bool UnitImpl::isReady() const
  {
    return this->isValid() &&
           this->isCompleted();
  }
  //------------------------------------------------ IS LOADED -----------------------------------------------
  bool UnitImpl::isLoaded() const
  {
    return this->loaded;
  }
  //----------------------------------------------- IS VISIBLE -----------------------------------------------
  bool UnitImpl::isVisible() const
  {
    if (this->getOwner()==BWAPI::BroodwarImpl.self())
    {
      return true;
    }
    if (!BWAPI::BroodwarImpl.visible(this->getTilePosition().x,this->getTilePosition().y))
    {
      return false;
    }
    /* TODO: Also need to check for detection (overlords, observers, Terran scans, etc) */
    if (this->isBurrowed() || this->isCloaked())
    {
      return false;
    }
    return true;
  }
  //---------------------------------------------- IS COMPLETED ----------------------------------------------
  bool UnitImpl::isCompleted() const
  {
    return this->getRawDataLocal()->status.getBit(BW::StatusFlags::Completed);
  }
  //------------------------------------------------ IS LIFTED -----------------------------------------------
  bool UnitImpl::isLifted() const
  {
    return this->getRawDataLocal()->status.getBit(BW::StatusFlags::InAir) &&
           this->getRawDataLocal()->unitID.isBuilding();
  }
  //----------------------------------------------- IS BURROWED ----------------------------------------------
  bool UnitImpl::isBurrowed() const
  {
    return this->getRawDataLocal()->status.getBit(BW::StatusFlags::Burrowed);
  }
  //------------------------------------------------ IS CLOAKED ----------------------------------------------
  bool UnitImpl::isCloaked() const
  {
    return this->getRawDataLocal()->status.getBit(BW::StatusFlags::Cloaked);
  }
  //------------------------------------------------ IS DISABLED ---------------------------------------------
  bool UnitImpl::isDisabled() const
  {
    return this->getRawDataLocal()->status.getBit(BW::StatusFlags::Disabled);
  }
  //---------------------------------------------- IS SELECTED -----------------------------------------------
  bool UnitImpl::isSelected() const
  {
    if (BWAPI::BroodwarImpl.isFlagEnabled(BWAPI::Flag::UserInput)==false)
      return false;
    return this->userSelected;
  }
  //--------------------------------------------- SET SELECTED -----------------------------------------------
  void UnitImpl::setSelected(bool selectedState)
  {
    this->userSelected=selectedState;
  }
  //---------------------------------------------- SET LOADED ------------------------------------------------
  void UnitImpl::setLoaded(bool loadedState)
  {
    this->loaded=loadedState;
  }
  //---------------------------------------------- GET POSITION ----------------------------------------------
  const BW::Position& UnitImpl::getPosition() const
  {
    return this->getRawDataLocal()->position;
  }
  //------------------------------------------- GET TILE POSITION --------------------------------------------
  BW::TilePosition UnitImpl::getTilePosition() const
  {
    return BW::Position(this->getPosition().x - this->getType().tileWidth()*BW::TILE_SIZE/2,
                        this->getPosition().y - this->getType().tileHeight()*BW::TILE_SIZE/2);
  }
  //----------------------------------------------- GET TATGET -----------------------------------------------
  Unit* UnitImpl::getTarget() const
  {
    return UnitImpl::BWUnitToBWAPIUnit(this->getRawData()->targetUnit);
  }
  //----------------------------------------------- GET TATGET -----------------------------------------------
  Unit* UnitImpl::getOrderTarget() const
  {
    return UnitImpl::BWUnitToBWAPIUnit(this->getRawData()->orderTargetUnit);
  }
  //--------------------------------------------- GET BUILD UNIT ---------------------------------------------
  Unit* UnitImpl::getBuildUnit() const
  {
    return UnitImpl::BWUnitToBWAPIUnit(this->getRawDataLocal()->currentBuildUnit);
  }
  //--------------------------------------------- GET BUILD UNIT ---------------------------------------------
  Unit* UnitImpl::getChild() const
  {
    return UnitImpl::BWUnitToBWAPIUnit(this->getRawDataLocal()->childInfoUnion.childUnit1);
  }
  //------------------------------------------ GET TATGET POSITION -------------------------------------------
  BW::Position UnitImpl::getTargetPosition() const
  {
   return this->getRawDataLocal()->moveToPos;
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
  //---------------------------------------------- GET ORDER ID ----------------------------------------------
  Order UnitImpl::getOrderID() const
  {
    return BWAPI::Order(this->getRawDataLocal()->orderID);
  }
  //----------------------------------------- GET SECONDARY ORDER ID -----------------------------------------
  Order UnitImpl::getSecondaryOrderID() const
  {
    return BWAPI::Order(this->getRawDataLocal()->secondaryOrderID);
  }
  //---------------------------------------------- IS IDLE ---------------------------------------------------
  bool UnitImpl::isIdle() const
  {
    return (this->getOrderID() == BW::OrderID::Guard ||
            this->getOrderID() == BW::OrderID::Stop ||
            this->getOrderID() == BW::OrderID::Pickup1 ||
            this->getOrderID() == BW::OrderID::Nothing2 ||
            this->getOrderID() == BW::OrderID::Medic ||
            this->getOrderID() == BW::OrderID::Carrier ||
            this->getOrderID() == BW::OrderID::Critter ||
            this->getOrderID() == BW::OrderID::NukeTrain);
  }
  //---------------------------------------------- GET DISTANCE ----------------------------------------------

  #pragma warning(push)
  #pragma warning(disable:4244)
  u16 UnitImpl::getDistance(Unit *unit) const
  {
   u32 result;
   if (unit == this)
     return 0;
   if (this->getPosition().y - this->getType().dimensionUp() <= unit->getPosition().y + unit->getType().dimensionDown())
     if (this->getPosition().y + this->getType().dimensionDown() >= unit->getPosition().y - unit->getType().dimensionUp())
       if (this->getPosition().x > unit->getPosition().x)
         result = this->getPosition().x - this->getType().dimensionLeft() - unit->getPosition().x - unit->getType().dimensionRight();
       else
         result = unit->getPosition().x - unit->getType().dimensionRight() - this->getPosition().x - this->getType().dimensionLeft();

   if (this->getPosition().x - this->getType().dimensionLeft() <= unit->getPosition().x + unit->getType().dimensionRight())
     if (this->getPosition().x + this->getType().dimensionRight() >= unit->getPosition().x - unit->getType().dimensionLeft())
       if (this->getPosition().y > unit->getPosition().y)
         result = this->getPosition().y - this->getType().dimensionUp() - unit->getPosition().y - unit->getType().dimensionDown();
       else
         result = unit->getPosition().y - unit->getType().dimensionDown() - this->getPosition().y - this->getType().dimensionUp();

   if (this->getPosition().x > unit->getPosition().x)
     if (this->getPosition().y > unit->getPosition().y)
       result = this->getDistance(this->getPosition().x - this->getType().dimensionLeft(),
                                  this->getPosition().y - this->getType().dimensionUp(),
                                  unit->getPosition().x + unit->getType().dimensionRight(),
                                  unit->getPosition().y + unit->getType().dimensionDown());
     else
       result = this->getDistance(this->getPosition().x - this->getType().dimensionLeft(),
                                 this->getPosition().y + this->getType().dimensionDown(),
                                 unit->getPosition().x + unit->getType().dimensionRight(),
                                 unit->getPosition().y - unit->getType().dimensionUp());
   else
     if (this->getPosition().y > unit->getPosition().y)
       result = this->getDistance(this->getPosition().x + this->getType().dimensionRight(),
                                this->getPosition().y - this->getType().dimensionUp(),
                                unit->getPosition().x - unit->getType().dimensionLeft(),
                                unit->getPosition().y + unit->getType().dimensionDown());
     else
       result = this->getDistance(this->getPosition().x + this->getType().dimensionRight(),
                                this->getPosition().y + this->getType().dimensionDown(),
                                unit->getPosition().x - unit->getType().dimensionLeft(),
                                unit->getPosition().y - unit->getType().dimensionUp());
   if (result > 0)
     return result;
   else
     return 0;
  }
  //---------------------------------------------- GET DISTANCE ----------------------------------------------
  u16 UnitImpl::getDistance(BW::Position position) const
  {
    return this->getDistance(this->getPosition().x, 
                             this->getPosition().y,
                             position.x, 
                             position.y);
  }
  //------------------------------------------ GET CENTER DISTANCE -------------------------------------------
  u16 UnitImpl::getCenterDistance(Unit *unit) const
  {
    return this->getDistance(this->getPosition().x, 
                             this->getPosition().y,
                             unit->getPosition().x, 
                             unit->getPosition().y);
  }
  //---------------------------------------------- GET DISTANCE ----------------------------------------------
  u16 UnitImpl::getDistance(int x1, int y1, int x2, int y2) const
  {
    return sqrt((long double)((long double)x1 - x2)*((long double)x1 - x2) +
                (long double)((long double)y1 - y2)*((long double)y1 - y2));
  }
  #pragma warning(pop)
  //---------------------------------------------- IS TRAINING -----------------------------------------------
  bool UnitImpl::isTraining() const
  {
    return !this->hasEmptyBuildQueue();
  }
  //------------------------------------------ GET TRAINING QUEUE --------------------------------------------
  std::list<UnitType > UnitImpl::getTrainingQueue() const
  {
    std::list<UnitType > trainList;
    if (this->hasEmptyBuildQueue()) return trainList;
    int i = this->getBuildQueueSlot()%5;
    int starti=i;
    trainList.push_front(BWAPI::UnitType(this->getBuildQueue()[i].id));
    i=(i + 1)%5;
    while(this->getBuildQueue()[i] != BW::UnitID::None && i!=starti)
    {
      trainList.push_back(BWAPI::UnitType(this->getBuildQueue()[i].id));
      i=(i + 1)%5;
    }
    return trainList;
  }
  //-------------------------------------------- HAS EMPTY QUEUE ---------------------------------------------
  bool UnitImpl::hasEmptyBuildQueueSync() const
  {
    return this->getBuildQueueSync()[this->getBuildQueueSlotSync()] == BW::UnitID::None;
  }
  //----------------------------------------- HAS EMPTY QUEUE LOCAL ------------------------------------------
  bool UnitImpl::hasEmptyBuildQueue() const
  {
    if (this->getBuildQueueSlot() < 5)
      return this->getBuildQueue()[this->getBuildQueueSlot()] == BW::UnitID::None;
    else
      return false;
  }
  //----------------------------------------- HAS FULL QUEUE LOCAL -------------------------------------------
  bool UnitImpl::hasFullBuildQueue() const
  {
    return this->getBuildQueue()[(this->getBuildQueueSlot() + 1) % 5] != BW::UnitID::None;
  }
  //------------------------------------------- ORDER Attack Location ----------------------------------------
  void UnitImpl::attackLocation(BW::Position position, Order order)
  {
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(position, order.getID()), sizeof(BW::Orders::Attack)); 
    BroodwarImpl.addToCommandBuffer(new CommandAttackLocation(this, position));
  }
  //------------------------------------------- ORDER RIGHT CLICK --------------------------------------------
  void UnitImpl::rightClick(BW::Position position)
  {
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::RightClick(position), sizeof(BW::Orders::RightClick)); 
    BroodwarImpl.addToCommandBuffer(new CommandRightClick(this, position));
  }
  //------------------------------------------- ORDER RIGHT CLICK --------------------------------------------
  void UnitImpl::rightClick(Unit *target)
  {
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::RightClick((UnitImpl*)target), sizeof(BW::Orders::RightClick)); 
    BroodwarImpl.addToCommandBuffer(new CommandRightClick(this, (UnitImpl*)target));
  }
  //------------------------------------------------- BUILD --------------------------------------------------
  void UnitImpl::build(BW::TilePosition position, UnitType type1)
  {
    BW::UnitType type(BW::UnitID::Enum(type1.getID()));
    this->orderSelect();
    if (!type.isAddon())
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::MakeBuilding(position, type), sizeof(BW::Orders::MakeBuilding)); 
    else
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::MakeAddon(position, type), sizeof(BW::Orders::MakeAddon)); 
    BroodwarImpl.addToCommandBuffer(new CommandBuild(this, type, position));
  }
  //------------------------------------------------- INVENT -------------------------------------------------
  void UnitImpl::invent(TechType tech)
  {
    this->orderSelect();
    BW::TechID::Enum techenum=static_cast<BW::TechID::Enum>(tech.getID());
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Invent(BW::TechType(techenum)), sizeof(BW::Orders::Invent));
    BroodwarImpl.addToCommandBuffer(new CommandInvent(this, BW::TechType(techenum)));
  }
  //------------------------------------------------- INVENT -------------------------------------------------
  void UnitImpl::upgrade(BWAPI::UpgradeType upgrade)
  {
    this->orderSelect();
    BW::UpgradeID::Enum upgradeenum=static_cast<BW::UpgradeID::Enum>(upgrade.getID());
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Upgrade(BW::UpgradeType(upgradeenum)), sizeof(BW::Orders::Upgrade)); 
    BroodwarImpl.addToCommandBuffer(new CommandUpgrade(this, BW::UpgradeType(upgradeenum)));
  }
  //-------------------------------------------------- STOP --------------------------------------------------
  void UnitImpl::stop()
  {
    //TODO: Handle stop order
  }
  //---------------------------------------------- HOLD POSITION ---------------------------------------------
  void UnitImpl::holdPosition()
  {
    //TODO: Handle hold position order
  }
  //-------------------------------------------------- PATROL ------------------------------------------------
  void UnitImpl::patrol(BW::Position position)
  {
    //TODO: Handle patrol order
  }
  //------------------------------------------------- USE TECH -----------------------------------------------
  void UnitImpl::useTech(TechType tech)
  {
    //TODO: Handle use tech order
  }
  //------------------------------------------------- USE TECH -----------------------------------------------
  void UnitImpl::useTech(TechType tech, BW::Position position)
  {
    //TODO: Handle use tech order
  }
  //------------------------------------------------- USE TECH -----------------------------------------------
  void UnitImpl::useTech(TechType tech, Unit* target)
  {
    //TODO: Handle use tech order
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
    if ( this->getRawDataLocal()->unitID.id==BW::UnitID::Resource_MineralPatch1
      || this->getRawDataLocal()->unitID.id==BW::UnitID::Resource_MineralPatch2
      || this->getRawDataLocal()->unitID.id==BW::UnitID::Resource_MineralPatch3)
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
  //----------------------------------------------- TRAIN UNIT -----------------------------------------------
  void UnitImpl::train(UnitType type1)
  {
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
    return BroodwarImpl.getUnit(((int)unit - (int)BW::BWDATA_UnitNodeTable)/336);
  }
  #pragma warning (pop)
  //----------------------------------------------------------------------------------------------------------
  bool UnitImpl::isMineral() const
  {
     return this->getType() == BW::UnitID::Resource_MineralPatch1 ||
            this->getType() == BW::UnitID::Resource_MineralPatch2 ||
            this->getType() == BW::UnitID::Resource_MineralPatch3;
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
    sprintf_s(position, 100, "Position = (%u,%u)", this->getPosition().x, 
                                              this->getPosition().y);

    sprintf_s(indexName, 50, "[%d]", this->getIndex());

    if (this->getTarget() == NULL)
      strcpy_s(targetIndex, 50, "Target:[NULL]");
    else
      sprintf_s(targetIndex, 50, "Target:[%d](%s)", (int)this->getTarget(), this->getTarget()->getType().getName().c_str());

    if (this->getOrderTarget() == NULL)
      strcpy_s(orderTargetIndex, 50, "OrderTarget:[NULL]");
    else
      sprintf_s(orderTargetIndex, 50, "OrderTarget:[%d](%s)", (int)(this->getOrderTarget()), this->getOrderTarget()->getType().getName().c_str());
  
    if (this->getOwner() != NULL)
      sprintf_s(owner, 100, "Player = (%s)", this->getOwner()->getName().c_str());
    else
      sprintf_s(owner, 100, "error owner id = (%d)", this->getOriginalRawData()->playerID);

    sprintf_s(unitName, 100, "(%s)", this->getType().getName().c_str());

    if (this->getChild() == NULL)
      sprintf_s(connectedUnit, 100, "(childUnit1 = NULL)");
    else
      sprintf_s(connectedUnit, 100, "(childUnit1 = %s)", this->getChild()->getType().getName().c_str());

    sprintf_s(orderName, 100, "(%s)", this->getOrderID().getName().c_str());
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
      if (((int)this->getOriginalRawData()->nextUnit - (int)BW::BWXFN_UnitNodeTable)/BW::UNIT_SIZE_IN_BYTES >= BW::UNIT_ARRAY_MAX_LENGTH)
       BWAPI::Broodwar.fatalError->log("Unit array too small, found unit with addr %X", (int)this->getOriginalRawData()->nextUnit);
      if ((int)this->getOriginalRawData()->nextUnit < (int)BW::BWXFN_UnitNodeTable)
       BWAPI::Broodwar.fatalError->log("Unit array begins at bad location, found unit with addr %X", (int)this->getOriginalRawData()->nextUnit);
    }
    #pragma warning(pop)
    #endif PARANOID_DEBUG
    return UnitImpl::BWUnitToBWAPIUnit(this->getRawDataLocal()->nextUnit);
  }
  //-------------------------------------------- GET ORDER TIMER ---------------------------------------------
  u8 UnitImpl::getOrderTimer() const
  {
     return this->getRawDataLocal()->mainOrderTimer;
  }
  //---------------------------------------- GET REMAINING BUILD TIME ----------------------------------------
  u16 UnitImpl::getRemainingBuildTime() const
  {
     return this->getRawDataLocal()->remainingBuildTime;
  }
  //----------------------------------------------------------------------------------------------------------
  u16 UnitImpl::getIndex() const
  {
    return this->index;
  }
  //----------------------------------------------------------------------------------------------------------
};
