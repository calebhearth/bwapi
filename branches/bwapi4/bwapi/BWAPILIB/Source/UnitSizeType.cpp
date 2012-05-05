#include <string>
#include <BWAPI/UnitSizeType.h>
#include <Util/Foreach.h>

#include "Common.h"

#include "../../Debug.h"

namespace BWAPI
{
  static const std::string unitSizeTypeName[UnitSizeTypes::Enum::MAX] =
  {
    "Independent",
    "Small",
    "Medium",
    "Large",
    "None",
    "Unknown"  
  };
  namespace UnitSizeTypeSet
  {
    using namespace UnitSizeTypes::Enum;
    BWAPI_TYPESET(unitSizeTypeSet, UnitSizeType, Independent, Small, Medium, Large, None, Unknown );
  }
  namespace UnitSizeTypes
  {
    BWAPI_TYPEDEF(UnitSizeType,Independent);
    BWAPI_TYPEDEF(UnitSizeType,Small);
    BWAPI_TYPEDEF(UnitSizeType,Medium);
    BWAPI_TYPEDEF(UnitSizeType,Large);
    BWAPI_TYPEDEF(UnitSizeType,None);
    BWAPI_TYPEDEF(UnitSizeType,Unknown);

  }
  UnitSizeType::UnitSizeType(int id) : Type( id )
  {
  }
  const std::string &UnitSizeType::getName() const
  {
    return unitSizeTypeName[this->getID()];
  }
  const char *UnitSizeType::c_str() const
  {
    return unitSizeTypeName[this->getID()].c_str();
  }
  UnitSizeType UnitSizeTypes::getUnitSizeType(std::string name)
  {
    for ( int i = 0; i < UnitSizeTypes::Enum::MAX; ++i )
    {
      if ( name == unitSizeTypeName[i] )
        return UnitSizeType(i);
    }
    return UnitSizeTypes::Unknown;
  }
  const UnitSizeType::set& UnitSizeTypes::allUnitSizeTypes()
  {
    return UnitSizeTypeSet::unitSizeTypeSet;
  }
}

