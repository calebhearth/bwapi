#include <string>
#include <map>
#include <BWAPI/UnitCommandType.h>
#include <Util/Foreach.h>

#include "Common.h"

#include "../../Debug.h"

namespace BWAPI
{
  static const std::string unitCommandTypeName[UnitCommandTypes::Enum::MAX] =
  {
    "Attack_Move",
    "Attack_Unit",
    "Build",
    "Build_Addon",
    "Train",
    "Morph",
    "Research",
    "Upgrade",
    "Set_Rally_Position",
    "Set_Rally_Unit",
    "Move",
    "Patrol",
    "Hold_Position",
    "Stop",
    "Follow",
    "Gather",
    "Return_Cargo",
    "Repair",
    "Burrow",
    "Unburrow",
    "Cloak",
    "Decloak",
    "Siege",
    "Unsiege",
    "Lift",
    "Land",
    "Load",
    "Unload",
    "Unload_All",
    "Unload_All_Position",
    "Right_Click_Position",
    "Right_Click_Unit",
    "Halt_Construction",
    "Cancel_Construction",
    "Cancel_Addon",
    "Cancel_Train",
    "Cancel_Train_Slot",
    "Cancel_Morph",
    "Cancel_Research",
    "Cancel_Upgrade",
    "Use_Tech",
    "Use_Tech_Position",
    "Use_Tech_Unit",
    "Place_COP",
    "None",
    "Unknown"
  };
  std::map<std::string, UnitCommandType> unitCommandTypeMap;
  namespace UnitCommandTypes
  {
    UnitCommandType ucmdTypeArr[] = { Attack_Move, Attack_Unit, Build, Build_Addon, Train, Morph, Research, Upgrade, Set_Rally_Position,
                    Set_Rally_Unit, Move, Patrol, Hold_Position, Stop, Follow, Gather, Return_Cargo, Repair, Burrow,
                    Unburrow, Cloak, Decloak, Siege, Unsiege, Lift, Land, Load, Unload, Unload_All, Unload_All_Position,
                    Right_Click_Position, Right_Click_Unit, Halt_Construction, Cancel_Construction, Cancel_Addon,
                    Cancel_Train, Cancel_Train_Slot, Cancel_Morph, Cancel_Research, Cancel_Upgrade, Use_Tech,
                    Use_Tech_Position, Use_Tech_Unit, Place_COP, None, Unknown };  
    UnitCommandType::set unitCommandTypeSet(ucmdTypeArr, countof(ucmdTypeArr));
    void init()
    {
      foreach(UnitCommandType i, unitCommandTypeSet)
      {
        std::string name(i.getName());
        fixName(&name);
        unitCommandTypeMap.insert(std::make_pair(name, i));
      }
    }
  }
  UnitCommandType::UnitCommandType(int id) : Type( id )
  {
  }
  const std::string &UnitCommandType::getName() const
  {
    return unitCommandTypeName[this->getID()];
  }
  const char *UnitCommandType::c_str() const
  {
    return unitCommandTypeName[this->getID()].c_str();
  }
  UnitCommandType UnitCommandTypes::getUnitCommandType(std::string name)
  {
    fixName(&name);
    std::map<std::string, UnitCommandType>::iterator i = unitCommandTypeMap.find(name);
    if (i == unitCommandTypeMap.end())
      return UnitCommandTypes::Unknown;
    return (*i).second;
  }
  const UnitCommandType::set& UnitCommandTypes::allUnitCommandTypes()
  {
    return UnitCommandTypes::unitCommandTypeSet;
  }
}

