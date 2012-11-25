#include <string>
#include <BWAPI/Error.h>
#include <Util/Foreach.h>

#include "Common.h"

#include "../../Debug.h"

namespace BWAPI
{
  const std::string Error::typeNames[Errors::Enum::MAX] =
  {
#define BWAPI_PARAM(x) #x,
#define COL_B
#include <BWAPI/def/Errors.csv>
  };

  namespace Errors
  {
    static const int setArr[Errors::Enum::MAX]={ 
#define BWAPI_PARAM(x) Enum::x,
#define COL_B
#include <BWAPI/def/Errors.csv>    
    };
    static const Error::const_set typeSet(setArr, countof(setArr));

#define BWAPI_PARAM(x) const Error x(Enum::x);
#define COL_B
#include <BWAPI/def/Errors.csv>
  }
  Error::Error(int id) : Type( id ) {}
  const Error::const_set& Errors::allErrors()
  {
    return Errors::typeSet;
  }
}
