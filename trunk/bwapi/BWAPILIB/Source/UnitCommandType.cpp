#include <string>
#include <map>
#include <set>
#include <BWAPI/UnitCommandType.h>
#include <Util/Foreach.h>

#include "Common.h"

#include "../../Debug.h"

namespace BWAPI
{
  bool initializingUnitCommandType = true;
  std::string unitCommandTypeName[BWAPI_UNIT_COMMAND_TYPE_COUNT];
  std::map<std::string, UnitCommandType> unitCommandTypeMap;
  std::set< UnitCommandType > unitCommandTypeSet;
  namespace UnitCommandTypes
  {
    const UnitCommandType Attack_Move(0);
    const UnitCommandType Attack_Unit(1);
    const UnitCommandType Build(2);
    const UnitCommandType Build_Addon(3);
    const UnitCommandType Train(4);
    const UnitCommandType Morph(5);
    const UnitCommandType Research(6);
    const UnitCommandType Upgrade(7);
    const UnitCommandType Set_Rally_Position(8);
    const UnitCommandType Set_Rally_Unit(9);
    const UnitCommandType Move(10);
    const UnitCommandType Patrol(11);
    const UnitCommandType Hold_Position(12);
    const UnitCommandType Stop(13);
    const UnitCommandType Follow(14);
    const UnitCommandType Gather(15);
    const UnitCommandType Return_Cargo(16);
    const UnitCommandType Repair(17);
    const UnitCommandType Burrow(18);
    const UnitCommandType Unburrow(19);
    const UnitCommandType Cloak(20);
    const UnitCommandType Decloak(21);
    const UnitCommandType Siege(22);
    const UnitCommandType Unsiege(23);
    const UnitCommandType Lift(24);
    const UnitCommandType Land(25);
    const UnitCommandType Load(26);
    const UnitCommandType Unload(27);
    const UnitCommandType Unload_All(28);
    const UnitCommandType Unload_All_Position(29);
    const UnitCommandType Right_Click_Position(30);
    const UnitCommandType Right_Click_Unit(31);
    const UnitCommandType Halt_Construction(32);
    const UnitCommandType Cancel_Construction(33);
    const UnitCommandType Cancel_Addon(34);
    const UnitCommandType Cancel_Train(35);
    const UnitCommandType Cancel_Train_Slot(36);
    const UnitCommandType Cancel_Morph(37);
    const UnitCommandType Cancel_Research(38);
    const UnitCommandType Cancel_Upgrade(39);
    const UnitCommandType Use_Tech(40);
    const UnitCommandType Use_Tech_Position(41);
    const UnitCommandType Use_Tech_Unit(42);
    const UnitCommandType Place_COP(43);
    const UnitCommandType None(44);
    const UnitCommandType Unknown(45);

    void init()
    {
      unitCommandTypeName[Attack_Move]          = "Attack Move";
      unitCommandTypeName[Attack_Unit]          = "Attack Unit";
      unitCommandTypeName[Build]                = "Build";
      unitCommandTypeName[Build_Addon]          = "Build Addon";
      unitCommandTypeName[Train]                = "Train";
      unitCommandTypeName[Morph]                = "Morph";
      unitCommandTypeName[Research]             = "Research";
      unitCommandTypeName[Upgrade]              = "Upgrade";
      unitCommandTypeName[Set_Rally_Position]   = "Set Rally Position";
      unitCommandTypeName[Set_Rally_Unit]       = "Set Rally Unit";
      unitCommandTypeName[Move]                 = "Move";
      unitCommandTypeName[Patrol]               = "Patrol";
      unitCommandTypeName[Hold_Position]        = "Hold Position";
      unitCommandTypeName[Stop]                 = "Stop";
      unitCommandTypeName[Follow]               = "Follow";
      unitCommandTypeName[Gather]               = "Gather";
      unitCommandTypeName[Return_Cargo]         = "Return Cargo";
      unitCommandTypeName[Repair]               = "Repair";
      unitCommandTypeName[Burrow]               = "Burrow";
      unitCommandTypeName[Unburrow]             = "Unburrow";
      unitCommandTypeName[Cloak]                = "Cloak";
      unitCommandTypeName[Decloak]              = "Decloak";
      unitCommandTypeName[Siege]                = "Siege";
      unitCommandTypeName[Unsiege]              = "Unsiege";
      unitCommandTypeName[Lift]                 = "Lift";
      unitCommandTypeName[Land]                 = "Land";
      unitCommandTypeName[Load]                 = "Load";
      unitCommandTypeName[Unload]               = "Unload";
      unitCommandTypeName[Unload_All]           = "Unload All";
      unitCommandTypeName[Unload_All_Position]  = "Unload All Position";
      unitCommandTypeName[Right_Click_Position] = "Right Click Position";
      unitCommandTypeName[Right_Click_Unit]     = "Right Click Unit";
      unitCommandTypeName[Halt_Construction]    = "Halt Construction";
      unitCommandTypeName[Cancel_Construction]  = "Cancel Construction";
      unitCommandTypeName[Cancel_Addon]         = "Cancel Addon";
      unitCommandTypeName[Cancel_Train]         = "Cancel Train";
      unitCommandTypeName[Cancel_Train_Slot]    = "Cancel Train Slot";
      unitCommandTypeName[Cancel_Morph]         = "Cancel Morph";
      unitCommandTypeName[Cancel_Research]      = "Cancel Research";
      unitCommandTypeName[Cancel_Upgrade]       = "Cancel Upgrade";
      unitCommandTypeName[Use_Tech]             = "Use Tech";
      unitCommandTypeName[Use_Tech_Position]    = "Use Tech Position";
      unitCommandTypeName[Use_Tech_Unit]        = "Use Tech Unit";
      unitCommandTypeName[Place_COP]            = "Place COP";
      unitCommandTypeName[None]                 = "None";
      unitCommandTypeName[Unknown]              = "Unknown";

      unitCommandTypeSet.insert(Attack_Move);
      unitCommandTypeSet.insert(Attack_Unit);
      unitCommandTypeSet.insert(Build);
      unitCommandTypeSet.insert(Build_Addon);
      unitCommandTypeSet.insert(Train);
      unitCommandTypeSet.insert(Morph);
      unitCommandTypeSet.insert(Research);
      unitCommandTypeSet.insert(Upgrade);
      unitCommandTypeSet.insert(Set_Rally_Position);
      unitCommandTypeSet.insert(Set_Rally_Unit);
      unitCommandTypeSet.insert(Move);
      unitCommandTypeSet.insert(Patrol);
      unitCommandTypeSet.insert(Hold_Position);
      unitCommandTypeSet.insert(Stop);
      unitCommandTypeSet.insert(Follow);
      unitCommandTypeSet.insert(Gather);
      unitCommandTypeSet.insert(Return_Cargo);
      unitCommandTypeSet.insert(Repair);
      unitCommandTypeSet.insert(Burrow);
      unitCommandTypeSet.insert(Unburrow);
      unitCommandTypeSet.insert(Cloak);
      unitCommandTypeSet.insert(Decloak);
      unitCommandTypeSet.insert(Siege);
      unitCommandTypeSet.insert(Unsiege);
      unitCommandTypeSet.insert(Lift);
      unitCommandTypeSet.insert(Land);
      unitCommandTypeSet.insert(Load);
      unitCommandTypeSet.insert(Unload);
      unitCommandTypeSet.insert(Unload_All);
      unitCommandTypeSet.insert(Unload_All_Position);
      unitCommandTypeSet.insert(Right_Click_Position);
      unitCommandTypeSet.insert(Right_Click_Unit);
      unitCommandTypeSet.insert(Halt_Construction);
      unitCommandTypeSet.insert(Cancel_Construction);
      unitCommandTypeSet.insert(Cancel_Addon);
      unitCommandTypeSet.insert(Cancel_Train);
      unitCommandTypeSet.insert(Cancel_Train_Slot);
      unitCommandTypeSet.insert(Cancel_Morph);
      unitCommandTypeSet.insert(Cancel_Research);
      unitCommandTypeSet.insert(Cancel_Upgrade);
      unitCommandTypeSet.insert(Use_Tech);
      unitCommandTypeSet.insert(Use_Tech_Position);
      unitCommandTypeSet.insert(Use_Tech_Unit);
      unitCommandTypeSet.insert(Place_COP);
      unitCommandTypeSet.insert(None);
      unitCommandTypeSet.insert(Unknown);

      foreach(UnitCommandType i, unitCommandTypeSet)
      {
        std::string name = i.getName();
        fixName(&name);
        unitCommandTypeMap.insert(std::make_pair(name, i));
      }
      initializingUnitCommandType = false;
    }
  }
  UnitCommandType::UnitCommandType()
  {
    this->id = UnitCommandTypes::None;
  }
  UnitCommandType::UnitCommandType(int id)
  {
    this->id = id;
    if (!initializingUnitCommandType && (id < 0 || id >= BWAPI_UNIT_COMMAND_TYPE_COUNT))
      this->id = UnitCommandTypes::Unknown;
  }
  UnitCommandType::UnitCommandType(const UnitCommandType& other)
  {
    this->id = other;
  }
  UnitCommandType& UnitCommandType::operator=(const UnitCommandType& other)
  {
    this->id = other;
    return *this;
  }
  UnitCommandType::operator int() const
  {
    return id;
  }
  int UnitCommandType::getID() const
  {
    return this->id;
  }
  std::string UnitCommandType::getName() const
  {
    return unitCommandTypeName[this->id];
  }
  UnitCommandType UnitCommandTypes::getUnitCommandType(std::string name)
  {
    fixName(&name);
    std::map<std::string, UnitCommandType>::iterator i = unitCommandTypeMap.find(name);
    if (i == unitCommandTypeMap.end())
      return UnitCommandTypes::Unknown;
    return (*i).second;
  }
  std::set<UnitCommandType>& UnitCommandTypes::allUnitCommandTypes()
  {
    return unitCommandTypeSet;
  }
}

