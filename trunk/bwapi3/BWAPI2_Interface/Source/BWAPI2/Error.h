#pragma once

#include "BWAPI2Interface.h"

#include <string>
#include <set>

namespace BWAPI2
{
  class UnitType;
  class Error
  {
    public:
      Error();
      Error(int id);
      Error(const Error& other);
      Error& operator=(const Error& other);
      bool operator==(const Error& other) const;
      bool operator!=(const Error& other) const;
      bool operator<(const Error& other) const;
      int getID() const;
      std::string toString() const;
    private:
      int id;
  };
  namespace Errors
  {
    Error getError(std::string& name);
    std::set<Error>& allErrors();
    void init();
    BWAPI2_EXPORT extern const Error Connection_Failure;
    BWAPI2_EXPORT extern const Error Unit_Does_Not_Exist;
    BWAPI2_EXPORT extern const Error Unit_Not_Visible;
    BWAPI2_EXPORT extern const Error Unit_Not_Owned;
    BWAPI2_EXPORT extern const Error Unit_Busy;
    BWAPI2_EXPORT extern const Error Incompatible_UnitType;
    BWAPI2_EXPORT extern const Error Incompatible_TechType;
    BWAPI2_EXPORT extern const Error Already_Researched;
    BWAPI2_EXPORT extern const Error Fully_Upgraded;
    BWAPI2_EXPORT extern const Error Insufficient_Minerals;
    BWAPI2_EXPORT extern const Error Insufficient_Gas;
    BWAPI2_EXPORT extern const Error Insufficient_Supply;
    BWAPI2_EXPORT extern const Error Insufficient_Energy;
    BWAPI2_EXPORT extern const Error Insufficient_Tech;
    BWAPI2_EXPORT extern const Error Insufficient_Ammo;
    BWAPI2_EXPORT extern const Error Insufficient_Space;
    BWAPI2_EXPORT extern const Error Unbuildable_Location;
    BWAPI2_EXPORT extern const Error Out_Of_Range;
    BWAPI2_EXPORT extern const Error Unable_To_Hit;
    BWAPI2_EXPORT extern const Error Access_Denied;
    BWAPI2_EXPORT extern const Error None;
    BWAPI2_EXPORT extern const Error Unknown;
  }
}
