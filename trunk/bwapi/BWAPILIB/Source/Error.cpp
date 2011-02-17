#include <string>
#include <map>
#include <set>
#include <BWAPI/Error.h>
#include <Util/Foreach.h>

#include "Common.h"

#include "../../Debug.h"

#define BWAPI_ERROR_MAX_COUNT 28

namespace BWAPI
{
  bool initializingError = true;
  std::string errorName[BWAPI_ERROR_MAX_COUNT];
  std::map<std::string, Error> errorMap;
  std::set< Error > errorSet;
  namespace Errors
  {
    const Error Unit_Does_Not_Exist(0);
    const Error Unit_Not_Visible(1);
    const Error Unit_Not_Owned(2);
    const Error Unit_Busy(3);
    const Error Incompatible_UnitType(4);
    const Error Incompatible_TechType(5);
    const Error Incompatible_State(6);
    const Error Already_Researched(7);
    const Error Fully_Upgraded(8);
    const Error Currently_Researching(9);
    const Error Currently_Upgrading(10);
    const Error Insufficient_Minerals(11);
    const Error Insufficient_Gas(12);
    const Error Insufficient_Supply(13);
    const Error Insufficient_Energy(14);
    const Error Insufficient_Tech(15);
    const Error Insufficient_Ammo(16);
    const Error Insufficient_Space(17);
    const Error Invalid_Tile_Position(18);
    const Error Unbuildable_Location(19);
    const Error Unreachable_Location(20);
    const Error Out_Of_Range(21);
    const Error Unable_To_Hit(22);
    const Error Access_Denied(23);
    const Error File_Not_Found(24);
    const Error Invalid_Parameter(25);
    const Error None(26);
    const Error Unknown(27);

    void init()
    {
      errorName[Unit_Does_Not_Exist]   = "Unit Does Not Exist";
      errorName[Unit_Not_Visible]      = "Unit Not Visible";
      errorName[Unit_Not_Owned]        = "Unit Not Owned";
      errorName[Unit_Busy]             = "Unit Busy";
      errorName[Incompatible_UnitType] = "Incompatible UnitType";
      errorName[Incompatible_TechType] = "Incompatible TechType";
      errorName[Incompatible_State]    = "Incompatible State";
      errorName[Already_Researched]    = "Already Researched";
      errorName[Fully_Upgraded]        = "Fully Upgraded";
      errorName[Currently_Researching] = "Currently Researching";
      errorName[Currently_Upgrading]   = "Currently Upgrading";
      errorName[Insufficient_Minerals] = "Insufficient Minerals";
      errorName[Insufficient_Gas]      = "Insufficient Gas";
      errorName[Insufficient_Supply]   = "Insufficient Supply";
      errorName[Insufficient_Energy]   = "Insufficient Energy";
      errorName[Insufficient_Tech]     = "Insufficient Tech";
      errorName[Insufficient_Ammo]     = "Insufficient Ammo";
      errorName[Insufficient_Space]    = "Insufficient Space";
      errorName[Invalid_Tile_Position] = "Invalid Tile Position";
      errorName[Unbuildable_Location]  = "Unbuildable Location";
      errorName[Unreachable_Location]  = "Unreachable Location";
      errorName[Out_Of_Range]          = "Out Of Range";
      errorName[Unable_To_Hit]         = "Unable To Hit";
      errorName[Access_Denied]         = "Access Denied";
      errorName[File_Not_Found]        = "File Not Found";
      errorName[Invalid_Parameter]     = "Invalid Parameter";
      errorName[None]                  = "None";
      errorName[Unknown]               = "Unknown";

      errorSet.insert(Unit_Does_Not_Exist);
      errorSet.insert(Unit_Not_Visible);
      errorSet.insert(Unit_Not_Owned);
      errorSet.insert(Unit_Busy);
      errorSet.insert(Incompatible_UnitType);
      errorSet.insert(Incompatible_TechType);
      errorSet.insert(Incompatible_State);
      errorSet.insert(Already_Researched);
      errorSet.insert(Fully_Upgraded);
      errorSet.insert(Currently_Researching);
      errorSet.insert(Currently_Upgrading);
      errorSet.insert(Insufficient_Minerals);
      errorSet.insert(Insufficient_Gas);
      errorSet.insert(Insufficient_Supply);
      errorSet.insert(Insufficient_Energy);
      errorSet.insert(Insufficient_Tech);
      errorSet.insert(Insufficient_Ammo);
      errorSet.insert(Insufficient_Space);
      errorSet.insert(Invalid_Tile_Position);
      errorSet.insert(Unbuildable_Location);
      errorSet.insert(Unreachable_Location);
      errorSet.insert(Out_Of_Range);
      errorSet.insert(Unable_To_Hit);
      errorSet.insert(Access_Denied);
      errorSet.insert(File_Not_Found);
      errorSet.insert(Invalid_Parameter);
      errorSet.insert(None);
      errorSet.insert(Unknown);

      foreach(Error i, errorSet)
      {
        std::string name = i.toString();
        fixName(&name);
        errorMap.insert(std::make_pair(name, i));
      }
      initializingError = false;
    }
  }

  Error::Error()
  {
    this->id = Errors::None;
  }
  Error::Error(int id)
  {
    this->id = id;
    if (!initializingError && (id < 0 || id >= BWAPI_ERROR_MAX_COUNT))
      this->id = Errors::Unknown;
  }
  Error::Error(const Error& other)
  {
    this->id = other;
  }
  Error& Error::operator=(const Error& other)
  {
    this->id = other;
    return *this;
  }
  Error::operator int() const
  {
    return id;
  }
  int Error::getID() const
  {
    return this->id;
  }
  std::string Error::toString() const
  {
    return errorName[this->id];
  }
  Error Errors::getError(std::string name)
  {
    fixName(&name);
    std::map<std::string, Error>::iterator i = errorMap.find(name);
    if (i == errorMap.end())
      return Errors::Unknown;
    return (*i).second;
  }
  std::set<Error>& Errors::allErrors()
  {
    return errorSet;
  }
}
