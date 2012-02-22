#include <string>
#include <map>
#include <set>
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
	std::map<std::string, PlayerType> playerTypeMap;
	namespace PlayerTypes
	{
		static const PlayerType::set playerTypeSet( None | Computer | Player | RescuePassive | EitherPreferComputer | EitherPreferHuman |
													Neutral | Closed | PlayerLeft | ComputerLeft | Unknown );
		void init()
		{
			foreach(PlayerType i, playerTypeSet)
			{
				std::string name(i.getName());
				fixName(&name);
				playerTypeMap.insert(std::make_pair(name, i));
			}
		}
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
	PlayerType PlayerTypes::getPlayerType(std::string name)
	{
		fixName(&name);
		std::map<std::string, PlayerType>::iterator i = playerTypeMap.find(name);
		if (i == playerTypeMap.end())
			return PlayerTypes::Unknown;
		return (*i).second;
	}
	const PlayerType::set& PlayerTypes::allPlayerTypes()
	{
		return PlayerTypes::playerTypeSet;
	}
}
