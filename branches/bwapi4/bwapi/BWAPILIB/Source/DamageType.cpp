#include <string>
#include <map>
#include <set>
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
  namespace DamageTypes
  {
    static const DamageType::set damageTypeSet(Independent | Explosive | Concussive | Normal | Ignore_Armor | None | Unknown);
    void init()
    {
      foreach(DamageType i, damageTypeSet)
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
    return DamageTypes::damageTypeSet;
  }
}
