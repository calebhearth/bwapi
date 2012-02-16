#pragma once
#include <string>
#include "Type.h"

#define BWAPI_PLYRTYPEDEF(x) static const PlayerType x(Enum::x) /** ref x */

namespace BWAPI
{
	namespace PlayerTypes
	{
		namespace Enum
		{
			enum Enum
			{
				None = 0,
				Computer,
				Player,
				RescuePassive,
				RescueActive,
				EitherPreferComputer,
				EitherPreferHuman,
				Neutral,
				Closed,
				Observer,
				PlayerLeft,
				ComputerLeft,
				Unknown,
				MAX
			};
		};
	};
	class PlayerType : public Type<PlayerType, PlayerTypes::Enum::Unknown>
	{
		public:
			PlayerType(int id = PlayerTypes::Enum::None);
			/** Returns the name of the player type. For example PlayerTypes::Computer.getName() will return an
			 * std::string object containing "Computer". */
			const std::string &getName() const;
			const char *c_str() const;
	};
	namespace PlayerTypes
	{
		/** Given the name of a player type, this function will return the playertype. For example:
		 *	PlayerTypes::getPlayerType("Human") will return PlayerTypes::Human. */
		PlayerType getPlayerType(std::string name);

		/** Returns the set of all the PlayerTypes. */
		const PlayerType::set& allPlayerTypes();
		void init();
		
		BWAPI_PLYRTYPEDEF(None);
		BWAPI_PLYRTYPEDEF(Computer);
		BWAPI_PLYRTYPEDEF(Player);
		BWAPI_PLYRTYPEDEF(RescuePassive);
		// Rescue Active
		
		BWAPI_PLYRTYPEDEF(EitherPreferComputer);
		BWAPI_PLYRTYPEDEF(EitherPreferHuman);
		BWAPI_PLYRTYPEDEF(Neutral);
		BWAPI_PLYRTYPEDEF(Closed);
		// Observer
		BWAPI_PLYRTYPEDEF(PlayerLeft);
		BWAPI_PLYRTYPEDEF(ComputerLeft);
		BWAPI_PLYRTYPEDEF(Unknown);
	}
}
