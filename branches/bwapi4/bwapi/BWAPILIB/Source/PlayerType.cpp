#include <string>
#include <BWAPI/PlayerType.h>
#include <Util/Foreach.h>

#include "Common.h"

#include "../../Debug.h"

namespace BWAPI
{
  static const std::string playerTypeName[PlayerTypes::Enum::MAX] =
  {
        "None",
        "Computer",
        "Player",
        "RescuePassive",
        "RescueActive",
        "EitherPreferComputer",
        "EitherPreferHuman",
        "Neutral",
        "Closed",
        "Observer",
        "PlayerLeft",
        "ComputerLeft",
        "Unknown"
  };
  namespace PlayerTypeSet
  {
    using namespace PlayerTypes::Enum;
    BWAPI_TYPESET(playerTypeSet, PlayerType, None, Computer, Player, RescuePassive, EitherPreferComputer, EitherPreferHuman,
                          Neutral, Closed, PlayerLeft, ComputerLeft, Unknown );
  }
  namespace PlayerTypes
  {
    BWAPI_TYPEDEF(PlayerType,None);
    BWAPI_TYPEDEF(PlayerType,Computer);
    BWAPI_TYPEDEF(PlayerType,Player);
    BWAPI_TYPEDEF(PlayerType,RescuePassive);
    // Rescue Active
    
    BWAPI_TYPEDEF(PlayerType,EitherPreferComputer);
    BWAPI_TYPEDEF(PlayerType,EitherPreferHuman);
    BWAPI_TYPEDEF(PlayerType,Neutral);
    BWAPI_TYPEDEF(PlayerType,Closed);
    // Observer
    BWAPI_TYPEDEF(PlayerType,PlayerLeft);
    BWAPI_TYPEDEF(PlayerType,ComputerLeft);
    BWAPI_TYPEDEF(PlayerType,Unknown);

  }
  PlayerType::PlayerType(int id) : Type( id )
  {
  }
  const std::string &PlayerType::getName() const
  {
    return playerTypeName[this->getID()];
  }
  const char *PlayerType::c_str() const
  {
    return playerTypeName[this->getID()].c_str();
  }
  bool PlayerType::isLobbyType() const
  {
    return this->getID() == PlayerTypes::Enum::EitherPreferComputer ||
           this->getID() == PlayerTypes::Enum::EitherPreferHuman    ||
           this->getID() == PlayerTypes::Enum::RescuePassive        ||
           this->getID() == PlayerTypes::Enum::RescueActive         ||
           this->getID() == PlayerTypes::Enum::Neutral;
  }
  bool PlayerType::isGameType() const
  {
    return this->getID() == PlayerTypes::Enum::Player         ||
           this->getID() == PlayerTypes::Enum::Computer       ||
           this->getID() == PlayerTypes::Enum::RescuePassive  ||
           this->getID() == PlayerTypes::Enum::RescueActive   ||
           this->getID() == PlayerTypes::Enum::Neutral;
  }
  PlayerType PlayerTypes::getPlayerType(std::string name)
  {
    for ( int i = 0; i < PlayerTypes::Enum::MAX; ++i )
    {
      if ( name == playerTypeName[i] )
        return PlayerType(i);
    }
    return PlayerTypes::Unknown;
  }
  const PlayerType::const_set& PlayerTypes::allPlayerTypes()
  {
    return PlayerTypeSet::playerTypeSet;
  }
  std::ostream &operator << (std::ostream &out, const PlayerType &t)
  {
    out << t.getName();
    return out;
  }
}
