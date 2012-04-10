#include <string>
#include <map>
#include <BWAPI/DamageType.h>

#include <Util/Foreach.h>

#include "Common.h"

#include "../../Debug.h"

namespace BWAPI
{
  static const std::string damageTypeName[DamageTypes::Enum::MAX] = 
  {
    "Independent",
    "Explosive",
    "Concussive",
    "Normal",
    "Ignore_Armor",
    "None",
    "Unknown"
  };

  std::map<std::string, DamageType> damageTypeMap;
  namespace DamageTypeSet
  {
    using namespace DamageTypes::Enum;
    BWAPI_TYPESET(damageTypeSet, DamageType, Independent, Explosive, Concussive, Normal, Ignore_Armor, None, Unknown);
  }
  namespace DamageTypes
  {
    BWAPI_TYPEDEF(DamageType,Independent);
    BWAPI_TYPEDEF(DamageType,Explosive);
    BWAPI_TYPEDEF(DamageType,Concussive);
    BWAPI_TYPEDEF(DamageType,Normal);
    BWAPI_TYPEDEF(DamageType,Ignore_Armor);
    BWAPI_TYPEDEF(DamageType,None);
    BWAPI_TYPEDEF(DamageType,Unknown);

    void init()
    {
      foreach(DamageType i, DamageTypeSet::damageTypeSet)
      {
        std::string name(i.getName());
        fixName(&name);
        damageTypeMap.insert(std::make_pair(name, i));
      }
    }
  }
  DamageType::DamageType(int id) : Type( id )
  {
  }
  const std::string &DamageType::getName() const
  {
    return damageTypeName[this->getID()];
  }
  const char *DamageType::c_str() const
  {
    return damageTypeName[this->getID()].c_str();
  }
  DamageType DamageTypes::getDamageType(std::string name)
  {
    fixName(&name);
    std::map<std::string, DamageType>::iterator i = damageTypeMap.find(name);
    if (i == damageTypeMap.end())
      return DamageTypes::Unknown;
    return (*i).second;
  }
  const DamageType::set& DamageTypes::allDamageTypes()
  {
    return DamageTypeSet::damageTypeSet;
  }
}
