#include <string>
#include <map>
#include <BWAPI/GameType.h>
#include <Util/Foreach.h>

#include "Common.h"

#include "../../Debug.h"

namespace BWAPI
{
  static const std::string gameTypeName[GameTypes::Enum::MAX] =
  {
    "None",
    "Custom",
    "Melee",
    "Free_For_All",
    "One_on_One",
    "Capture_The_Flag",
    "Greed",
    "Slaughter",
    "Sudden_Death",
    "Ladder",
    "Use_Map_Settings",
    "Team_Melee",
    "Team_Free_For_All",
    "Team_Capture_The_Flag",
    "",
    "Top_vs_Bottom",
    "Iron_Man_Ladder",  // 16
    "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
    "Unknown"
  };
  std::map<std::string, GameType> gameTypeMap;

  namespace GameTypeSet
  {
    using namespace GameTypes::Enum;
    BWAPI_TYPESET(gameTypeSet, GameType, Melee, Free_For_All, One_on_One, Capture_The_Flag, Greed, Slaughter, Sudden_Death, 
                        Ladder, Use_Map_Settings, Team_Melee, Team_Free_For_All, Team_Capture_The_Flag,
                        Top_vs_Bottom, None, Unknown );
  }
  namespace GameTypes
  {
    BWAPI_TYPEDEF(GameType,Melee);
    BWAPI_TYPEDEF(GameType,Free_For_All);
    BWAPI_TYPEDEF(GameType,One_on_One);
    BWAPI_TYPEDEF(GameType,Capture_The_Flag);
    BWAPI_TYPEDEF(GameType,Greed);
    BWAPI_TYPEDEF(GameType,Slaughter);
    BWAPI_TYPEDEF(GameType,Sudden_Death);
    BWAPI_TYPEDEF(GameType,Ladder);
    BWAPI_TYPEDEF(GameType,Use_Map_Settings);
    BWAPI_TYPEDEF(GameType,Team_Melee);
    BWAPI_TYPEDEF(GameType,Team_Free_For_All);
    BWAPI_TYPEDEF(GameType,Team_Capture_The_Flag);
    BWAPI_TYPEDEF(GameType,Top_vs_Bottom);
    BWAPI_TYPEDEF(GameType,None);
    BWAPI_TYPEDEF(GameType,Unknown);
    
    void init()
    {
      foreach(GameType i, GameTypeSet::gameTypeSet)
      {
        std::string name(i.getName());
        fixName(&name);
        gameTypeMap.insert(std::make_pair(name, i));
      }
    }
  }
  GameType::GameType(int id) : Type( id )
  {
  }
  const std::string &GameType::getName() const
  {
    return gameTypeName[this->getID()];
  }
  const char *GameType::c_str() const
  {
    return gameTypeName[this->getID()].c_str();
  }
  GameType GameTypes::getGameType(std::string name)
  {
    fixName(&name);
    std::map<std::string, GameType>::iterator i = gameTypeMap.find(name);
    if (i == gameTypeMap.end()) return GameTypes::Unknown;
    return (*i).second;
  }
  const GameType::set& GameTypes::allGameTypes()
  {
    return GameTypeSet::gameTypeSet;
  }
}
