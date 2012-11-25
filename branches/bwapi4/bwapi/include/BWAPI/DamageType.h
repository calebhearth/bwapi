#pragma once
#include <BWAPI/Type.h>

namespace BWAPI
{
  namespace DamageTypes
  {
    namespace Enum
    {
      enum Enum
      {
#define BWAPI_PARAM(x) x,
#define COL_B
#include <BWAPI/def/DamageTypes.csv>
        MAX
      };
    }
  }
  class DamageType : public Type<DamageType, DamageTypes::Enum::Unknown>
  {
    public:
      DamageType(int id = DamageTypes::Enum::None);
  };
  namespace DamageTypes
  {
    /** Returns the set of all the DamageTypes. */
    const DamageType::const_set& allDamageTypes();

#define BWAPI_PARAM(x) extern const DamageType x;
#define COL_B
#include <BWAPI/def/DamageTypes.csv>
  }
}
