#include <string>
#include <map>
#include <BWAPI/ExplosionType.h>
#include <Util/Foreach.h>

#include "Common.h"

#include "../../Debug.h"

namespace BWAPI
{
	static const std::string explosionTypeName[ExplosionTypes::Enum::MAX] =
	{
		"None",
		"Normal",
		"Radial_Splash",
		"Enemy_Splash",
		"Lockdown",
		"Nuclear_Missile",
		"Parasite",
		"Broodlings",
		"EMP_Shockwave",
		"Irradiate",
		"Ensnare",
		"Plague",
		"Stasis_Field",
		"Dark_Swarm",
		"Consume",
		"Yamato_Gun",
		"Restoration",
		"Disruption_Web",
		"Corrosive_Acid",
		"Mind_Control",
		"Feedback",
		"Optical_Flare",
		"Maelstrom",
		"Unused",
		"Air_Splash",
		"Unknown"
	};

	std::map<std::string, ExplosionType> explosionTypeMap;
	namespace ExplosionTypes
	{
		static const ExplosionType::set explosionTypeSet(None | Normal | Radial_Splash | Enemy_Splash | Lockdown | Nuclear_Missile |
														Parasite | Broodlings | EMP_Shockwave | Irradiate | Ensnare | Plague | 
														Stasis_Field | Dark_Swarm | Consume | Yamato_Gun | Restoration | Disruption_Web |
														Corrosive_Acid | Mind_Control | Feedback | Optical_Flare | Maelstrom |
														Air_Splash | Unknown);
		void init()
		{
			foreach(ExplosionType i, explosionTypeSet)
			{
				std::string name(i.getName());
				fixName(&name);
				explosionTypeMap.insert(std::make_pair(name, i));
			}
		}
	}
	ExplosionType::ExplosionType(int id) : Type( id )
	{
	}
	const std::string &ExplosionType::getName() const
	{
		return explosionTypeName[this->getID()];
	}
	const char *ExplosionType::c_str() const
	{
		return explosionTypeName[this->getID()].c_str();
	}
	ExplosionType ExplosionTypes::getExplosionType(std::string name)
	{
		fixName(&name);
		std::map<std::string, ExplosionType>::iterator i = explosionTypeMap.find(name);
		if (i == explosionTypeMap.end())
			return ExplosionTypes::Unknown;
		return (*i).second;
	}
	const ExplosionType::set& ExplosionTypes::allExplosionTypes()
	{
		return ExplosionTypes::explosionTypeSet;
	}
}
