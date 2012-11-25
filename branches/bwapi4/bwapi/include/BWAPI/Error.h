#pragma once
#include <BWAPI/Type.h>
#include <string>

namespace BWAPI
{
  class UnitType;
 // class Error;

  namespace Errors
  {
    /// Enumeration of Error types
    namespace Enum
    {
      enum Enum
      {
#define BWAPI_PARAM(x) x,
#define COL_B
#include <BWAPI/def/Errors.csv>
        MAX
      };
    };
  };
  
  /// The Error object is generally used to determine
  /// why certain functions in BWAPI have failed.
  ///
  /// For example, you may not have enough resources
  /// to construct a unit.
  /// @see Game::getLastError, Game::setLastError
 // typedef Type<Error, Errors::Enum::Unknown> ErrorBase;
  class Error : public Type<Error, Errors::Enum::Unknown>
  {
  public:
    Error(int id = Errors::Enum::None);
  };
  namespace Errors
  {
    /** The set of all the error codes. */
    const Error::const_set& allErrors();

#define BWAPI_PARAM(x) extern const Error x;
#define COL_B
#include <BWAPI/def/Errors.csv>
  }
}
