#pragma once
#include "Type.h"

#define BWAPI_DAMAGETYPEDEF(x) static const DamageType x(Enum::x) /** ref x */

namespace BWAPI
{
  namespace DamageTypes
  {
    namespace Enum
    {
      enum Enum
      {
        Independent = 0,
        Explosive,
        Concussive,
        Normal,
        Ignore_Armor,
        None,
        Unknown,
        MAX
      };
    }
  }
  class DamageType : public Type<DamageType, DamageTypes::Enum::Unknown>
  {
    public:
      DamageType(int id = DamageTypes::Enum::None);

      /** Returns the name of this damage type. For example DamageTypes::Explosive.getName() will return
       * std::string("Explosive"). */
      const std::string &getName() const;
      const char *c_str() const;
  };
  namespace DamageTypes
  {
    /** Given the name of a damage type, this will return a corresponding DamageType object. For example,
     * DamageTypes::getDamageType("Concussive") will return DamageTypes::Concussive. */
    DamageType getDamageType(std::string name);

    /** Returns the set of all the DamageTypes. */
    const DamageType::set& allDamageTypes();

    void init();

    BWAPI_DAMAGETYPEDEF(Independent);
    BWAPI_DAMAGETYPEDEF(Explosive);
    BWAPI_DAMAGETYPEDEF(Concussive);
    BWAPI_DAMAGETYPEDEF(Normal);
    BWAPI_DAMAGETYPEDEF(Ignore_Armor);
    BWAPI_DAMAGETYPEDEF(None);
    BWAPI_DAMAGETYPEDEF(Unknown);
  }
}
