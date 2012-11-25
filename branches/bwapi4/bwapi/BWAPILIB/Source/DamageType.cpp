#include <string>
#include <BWAPI/DamageType.h>

#include <Util/Foreach.h>

#include "Common.h"

#include "../../Debug.h"

namespace BWAPI
{
  const std::string DamageType::typeNames[DamageTypes::Enum::MAX] = 
  {
#define BWAPI_PARAM(x) #x,
#define COL_B
#include <BWAPI/def/DamageTypes.csv>
  };

  namespace DamageTypes
  {
    static const int setArr[DamageTypes::Enum::MAX]={ 
#define BWAPI_PARAM(x) Enum::x,
#define COL_B
#include <BWAPI/def/DamageTypes.csv>    
    };
    static const DamageType::const_set typeSet(setArr, countof(setArr));

#define BWAPI_PARAM(x) const DamageType x(Enum::x);
#define COL_B
#include <BWAPI/def/DamageTypes.csv>
  }
  DamageType::DamageType(int id) : Type( id )
  {
  }
  const DamageType::const_set& DamageTypes::allDamageTypes()
  {
    return DamageTypes::typeSet;
  }
}
