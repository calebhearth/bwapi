#pragma once
#include <string>
#include "Type.h"

#define BWAPI_UNITCMDTYPEDEF(x) static const UnitCommandType x(Enum::x) /** ref x */

namespace BWAPI
{
  namespace UnitCommandTypes
  {
    namespace Enum
    {
      enum Enum
      {
        Attack_Move = 0,
        Attack_Unit,
        Build,
        Build_Addon,
        Train,
        Morph,
        Research,
        Upgrade,
        Set_Rally_Position,
        Set_Rally_Unit,
        Move,
        Patrol,
        Hold_Position,
        Stop,
        Follow,
        Gather,
        Return_Cargo,
        Repair,
        Burrow,
        Unburrow,
        Cloak,
        Decloak,
        Siege,
        Unsiege,
        Lift,
        Land,
        Load,
        Unload,
        Unload_All,
        Unload_All_Position,
        Right_Click_Position,
        Right_Click_Unit,
        Halt_Construction,
        Cancel_Construction,
        Cancel_Addon,
        Cancel_Train,
        Cancel_Train_Slot,
        Cancel_Morph,
        Cancel_Research,
        Cancel_Upgrade,
        Use_Tech,
        Use_Tech_Position,
        Use_Tech_Unit,
        Place_COP,
        None,
        Unknown,
        MAX
      };
    };
  };
  class UnitCommandType : public Type<UnitCommandType, UnitCommandTypes::Enum::Unknown>
  {
  public:
    UnitCommandType(int id = UnitCommandTypes::Enum::None);
    /** Returns the string corresponding to the UnitCommandType object. For example,
     * UnitCommandTypes::Set_Rally_Position.getName() returns std::string("Set Rally Position")*/
    const std::string &getName() const;
    const char *c_str() const;
  };
  namespace UnitCommandTypes
  {
    /** Given a string, this function returns the command type it refers to. For example,
     * UnitCommandTypes::getUnitCommandType("Attack Position") returns UnitCommandTypes::Attack_Position. */
    UnitCommandType getUnitCommandType(std::string name);

    /** Returns the set of all the sizes, which are listed below: */
    const UnitCommandType::set& allUnitCommandTypes();
    void init();
    
    BWAPI_UNITCMDTYPEDEF(Attack_Move);
    BWAPI_UNITCMDTYPEDEF(Attack_Unit);
    BWAPI_UNITCMDTYPEDEF(Build);
    BWAPI_UNITCMDTYPEDEF(Build_Addon);
    BWAPI_UNITCMDTYPEDEF(Train);
    BWAPI_UNITCMDTYPEDEF(Morph);
    BWAPI_UNITCMDTYPEDEF(Research);
    BWAPI_UNITCMDTYPEDEF(Upgrade);
    BWAPI_UNITCMDTYPEDEF(Set_Rally_Position);
    BWAPI_UNITCMDTYPEDEF(Set_Rally_Unit);
    BWAPI_UNITCMDTYPEDEF(Move);
    BWAPI_UNITCMDTYPEDEF(Patrol);
    BWAPI_UNITCMDTYPEDEF(Hold_Position);
    BWAPI_UNITCMDTYPEDEF(Stop);
    BWAPI_UNITCMDTYPEDEF(Follow);
    BWAPI_UNITCMDTYPEDEF(Gather);
    BWAPI_UNITCMDTYPEDEF(Return_Cargo);
    BWAPI_UNITCMDTYPEDEF(Repair);
    BWAPI_UNITCMDTYPEDEF(Burrow);
    BWAPI_UNITCMDTYPEDEF(Unburrow);
    BWAPI_UNITCMDTYPEDEF(Cloak);
    BWAPI_UNITCMDTYPEDEF(Decloak);
    BWAPI_UNITCMDTYPEDEF(Siege);
    BWAPI_UNITCMDTYPEDEF(Unsiege);
    BWAPI_UNITCMDTYPEDEF(Lift);
    BWAPI_UNITCMDTYPEDEF(Land);
    BWAPI_UNITCMDTYPEDEF(Load);
    BWAPI_UNITCMDTYPEDEF(Unload);
    BWAPI_UNITCMDTYPEDEF(Unload_All);
    BWAPI_UNITCMDTYPEDEF(Unload_All_Position);
    BWAPI_UNITCMDTYPEDEF(Right_Click_Position);
    BWAPI_UNITCMDTYPEDEF(Right_Click_Unit);
    BWAPI_UNITCMDTYPEDEF(Halt_Construction);
    BWAPI_UNITCMDTYPEDEF(Cancel_Construction);
    BWAPI_UNITCMDTYPEDEF(Cancel_Addon);
    BWAPI_UNITCMDTYPEDEF(Cancel_Train);
    BWAPI_UNITCMDTYPEDEF(Cancel_Train_Slot);
    BWAPI_UNITCMDTYPEDEF(Cancel_Morph);
    BWAPI_UNITCMDTYPEDEF(Cancel_Research);
    BWAPI_UNITCMDTYPEDEF(Cancel_Upgrade);
    BWAPI_UNITCMDTYPEDEF(Use_Tech);
    BWAPI_UNITCMDTYPEDEF(Use_Tech_Position);
    BWAPI_UNITCMDTYPEDEF(Use_Tech_Unit);
    BWAPI_UNITCMDTYPEDEF(Place_COP);
    BWAPI_UNITCMDTYPEDEF(None);
    BWAPI_UNITCMDTYPEDEF(Unknown);
  }
}
