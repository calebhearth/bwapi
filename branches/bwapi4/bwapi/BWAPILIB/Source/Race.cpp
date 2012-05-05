#include <string>
#include <map>
#include <BWAPI/Race.h>
#include <BWAPI/UnitType.h>
#include <Util/Foreach.h>

#include "Common.h"

#include "../../Debug.h"

namespace BWAPI
{
  // (local scope)
  namespace RaceInternal
  {
    using namespace UnitTypes;
  
    // NAMES
    static const std::string raceNames[Races::Enum::MAX] =
    {
      "Zerg", "Terran", "Protoss",
      "", "", "",
      "Random", "None", "Unknown"
    };
  
    // LOCALIZATION
    std::string raceLocalNames[Races::Enum::MAX];

    // WORKER TYPES
    static const UnitType workerTypes[Races::Enum::MAX] =
    {
      Zerg_Drone, Terran_SCV, Protoss_Probe, 
      None, None, None, // unused
      Unknown, None, Unknown // random, none, unk
    };

    // BASE TYPES
    static const UnitType baseTypes[Races::Enum::MAX] =
    {
      Zerg_Hatchery, Terran_Command_Center, Protoss_Nexus,
      None, None, None, // unused
      Unknown, None, Unknown // random, none, unk
    };

    // REFINERY TYPES
    static const UnitType refineryTypes[Races::Enum::MAX] =
    {
      Zerg_Extractor, Terran_Refinery, Protoss_Assimilator,
      None, None, None, // unused
      Unknown, None, Unknown // random, none, unk
    };

    // TRANSPORT TYPES
    static const UnitType transportTypes[Races::Enum::MAX] =
    {
      Zerg_Overlord, Terran_Dropship, Protoss_Shuttle,
      None, None, None, // unused
      Unknown, None, Unknown // random, none, unk
    };

    // SUPPLY TYPES
    static const UnitType supplyTypes[Races::Enum::MAX] =
    {
      Zerg_Overlord, Terran_Supply_Depot, Protoss_Pylon,
      None, None, None, // unused
      Unknown, None, Unknown // random, none, unk
    };
  };// end local scope

  bool initializingRace = true;

  std::map<std::string, Race> raceMap;
  namespace RaceSet
  {
    using namespace Races::Enum;
    BWAPI_TYPESET(raceSet, Race, Zerg, Terran, Protoss, None, Unknown );
  }
  namespace Races
  {
    BWAPI_TYPEDEF(Race,Zerg);
    BWAPI_TYPEDEF(Race,Terran);
    BWAPI_TYPEDEF(Race,Protoss);
    BWAPI_TYPEDEF(Race,Random);
    BWAPI_TYPEDEF(Race,None);
    BWAPI_TYPEDEF(Race,Unknown);

    void init()
    {
      foreach(Race i, RaceSet::raceSet)
      {
        std::string name(i.getName());
        fixName(&name);
        raceMap.insert(std::make_pair(name, i));
      }
    }
  }
  Race::Race(int id) : Type(id)
  {
  }
  const std::string &Race::getName() const
  {
    return RaceInternal::raceNames[this->getID()];
  }
  const char *Race::c_str() const
  {
    return RaceInternal::raceNames[this->getID()].c_str();
  }
  UnitType Race::getWorker() const
  {
    return RaceInternal::workerTypes[this->getID()];
  }
  UnitType Race::getCenter() const
  {
    return RaceInternal::baseTypes[this->getID()];
  }
  UnitType Race::getRefinery() const
  {
    return RaceInternal::refineryTypes[this->getID()];
  }
  UnitType Race::getTransport() const
  {
    return RaceInternal::transportTypes[this->getID()];
  }
  UnitType Race::getSupplyProvider() const
  {
    return RaceInternal::supplyTypes[this->getID()];
  }
  Race Races::getRace(std::string &name)
  {
    fixName(&name);
    std::map<std::string, Race>::iterator i = raceMap.find(name);
    if (i == raceMap.end())
      return Races::Unknown;
    return (*i).second;
  }
  const Race::set& Races::allRaces()
  {
    return RaceSet::raceSet;
  }
}
