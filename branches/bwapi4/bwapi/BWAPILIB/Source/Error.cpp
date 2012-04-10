#include <string>
#include <map>
#include <BWAPI/Error.h>
#include <Util/Foreach.h>

#include "Common.h"

#include "../../Debug.h"

namespace BWAPI
{

  static const std::string errorName[Errors::Enum::MAX] =
  {
    "Unit_Does_Not_Exist",
    "Unit_Not_Visible",
    "Unit_Not_Owned",
    "Unit_Busy",
    "Incompatible_UnitType",
    "Incompatible_TechType",
    "Incompatible_State",
    "Already_Researched",
    "Fully_Upgraded",
    "Currently_Researching",
    "Currently_Upgrading",
    "Insufficient_Minerals",
    "Insufficient_Gas",
    "Insufficient_Supply",
    "Insufficient_Energy",
    "Insufficient_Tech",
    "Insufficient_Ammo",
    "Insufficient_Space",
    "Invalid_Tile_Position",
    "Unbuildable_Location",
    "Unreachable_Location",
    "Out_Of_Range",
    "Unable_To_Hit",
    "Access_Denied",
    "File_Not_Found",
    "Invalid_Parameter",
    "None",
    "Unknown"
  };

  std::map<std::string, Error> errorMap;

  namespace Errors
  {
    BWAPI_TYPEDEF(Error,Unit_Does_Not_Exist);
    BWAPI_TYPEDEF(Error,Unit_Not_Visible);
    BWAPI_TYPEDEF(Error,Unit_Not_Owned);
    BWAPI_TYPEDEF(Error,Unit_Busy);
    BWAPI_TYPEDEF(Error,Incompatible_UnitType);
    BWAPI_TYPEDEF(Error,Incompatible_TechType);
    BWAPI_TYPEDEF(Error,Incompatible_State);
    BWAPI_TYPEDEF(Error,Already_Researched);
    BWAPI_TYPEDEF(Error,Fully_Upgraded);
    BWAPI_TYPEDEF(Error,Currently_Researching);
    BWAPI_TYPEDEF(Error,Currently_Upgrading);
    BWAPI_TYPEDEF(Error,Insufficient_Minerals);
    BWAPI_TYPEDEF(Error,Insufficient_Gas);
    BWAPI_TYPEDEF(Error,Insufficient_Supply);
    BWAPI_TYPEDEF(Error,Insufficient_Energy);
    BWAPI_TYPEDEF(Error,Insufficient_Tech);
    BWAPI_TYPEDEF(Error,Insufficient_Ammo);
    BWAPI_TYPEDEF(Error,Insufficient_Space);
    BWAPI_TYPEDEF(Error,Invalid_Tile_Position);
    BWAPI_TYPEDEF(Error,Unbuildable_Location);
    BWAPI_TYPEDEF(Error,Unreachable_Location);
    BWAPI_TYPEDEF(Error,Out_Of_Range);
    BWAPI_TYPEDEF(Error,Unable_To_Hit);
    BWAPI_TYPEDEF(Error,Access_Denied);
    BWAPI_TYPEDEF(Error,File_Not_Found);
    BWAPI_TYPEDEF(Error,Invalid_Parameter);
    BWAPI_TYPEDEF(Error,None);
    BWAPI_TYPEDEF(Error,Unknown);

    static const Error::set errorSet(Unit_Does_Not_Exist | Unit_Not_Visible | Unit_Not_Owned | Unit_Busy | Incompatible_UnitType |
              Incompatible_TechType | Incompatible_State | Already_Researched | Fully_Upgraded | Currently_Researching |
              Currently_Upgrading | Insufficient_Minerals | Insufficient_Gas | Insufficient_Supply | Insufficient_Energy |
              Insufficient_Tech | Insufficient_Ammo | Insufficient_Space | Invalid_Tile_Position | Unbuildable_Location |
              Unreachable_Location | Out_Of_Range | Unable_To_Hit | Access_Denied | File_Not_Found | Invalid_Parameter |
              None | Unknown);
    void init()
    {
      foreach(Error i, errorSet)
      {
        std::string name(i.toString());
        fixName(&name);
        errorMap.insert(std::make_pair(name, i));
      }
    }
  }

  Error::Error(int id) : Type( id )
  {
  }
  std::string Error::toString() const
  {
    return errorName[this->getID()];
  }
  const char *Error::c_str() const
  {
    return errorName[this->getID()].c_str();
  }
  Error Errors::getError(std::string name)
  {
    fixName(&name);
    std::map<std::string, Error>::iterator i = errorMap.find(name);
    if (i == errorMap.end())
      return Errors::Unknown;
    return (*i).second;
  }
  const Error::set& Errors::allErrors()
  {
    return Errors::errorSet;
  }
}
