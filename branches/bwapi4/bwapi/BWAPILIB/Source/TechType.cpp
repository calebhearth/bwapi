#include <string>
#include <map>
#include <BWAPI/TechType.h>
#include <BWAPI/Race.h>
#include <BWAPI/UnitType.h>
#include <BWAPI/WeaponType.h>
#include <Util/Foreach.h>

#include "Common.h"

#include "../../Debug.h"

namespace BWAPI
{
	// NAMES
	static const std::string techTypeNames[TechTypes::Enum::MAX] =
	{
		"Stim_Packs",
		"Lockdown",
		"EMP_Shockwave",
		"Spider_Mines",
		"Scanner_Sweep",
		"Tank_Siege_Mode",
		"Defensive_Matrix",
		"Irradiate",
		"Yamato_Gun",
		"Cloaking_Field",
		"Personnel_Cloaking",
		"Burrowing",
		"Infestation",
		"Spawn_Broodlings",
		"Dark_Swarm",
		"Plague",
		"Consume",
		"Ensnare",
		"Parasite",
		"Psionic_Storm",
		"Hallucination",
		"Recall",
		"Stasis_Field",
		"Archon_Warp",
		"Restoration",
		"Disruption_Web",
		"",
		"Mind_Control",
		"Dark_Archon_Meld",
		"Feedback",
		"Optical_Flare",
		"Maelstrom",
		"Lurker_Aspect",
		"",
		"Healing",
		"", "", "", "", "", "", "", "", ""
		"None",
		"Nuclear_Strike",
		"Unknown",
	};

	// LOCALIZATION
	std::string techLocalNames[TechTypes::Enum::MAX];

	// DEFAULTS
	static const int defaultOreCost[TechTypes::Enum::MAX] = 
	{ 100, 200, 200, 100, 0, 150, 0, 200, 100, 150, 100, 100, 0, 100, 0, 200, 100, 100, 0, 200, 150, 150, 150, 0, 100, 200, 0, 200, 0, 100, 100, 100, 200 };
	static const int defaultTimeCost[TechTypes::Enum::MAX] =
	{ 1200, 1500, 1800, 1200, 0, 1200, 0, 1200, 1800, 1500, 1200, 1200, 0, 1200, 0, 1500, 1500, 1200, 0, 1800, 1200, 1800, 1500, 0, 1200, 1200, 0, 1800, 0, 1800, 1800, 1500, 1800 };
	static const int defaultEnergyCost[TechTypes::Enum::MAX] =
	{ 0, 100, 100, 0, 50, 0, 100, 75, 150, 25, 25, 0, 0, 150, 100, 150, 0, 75, 75, 75, 100, 150, 100, 0, 50, 125, 0, 150, 0, 50, 75, 100, 0, 0, 1 };
	
	// ACTUAL
	int oreCost[TechTypes::Enum::MAX], gasCost[TechTypes::Enum::MAX], timeCost[TechTypes::Enum::MAX], energyCost[TechTypes::Enum::MAX];

	namespace techInternalResearches
	{
		using namespace UnitTypes;
		static const UnitType whatResearches[TechTypes::Enum::MAX] =
		{ Terran_Academy, Terran_Covert_Ops, Terran_Science_Facility, Terran_Machine_Shop,
		None, Terran_Machine_Shop, None, Terran_Science_Facility, Terran_Physics_Lab, 
		Terran_Control_Tower, Terran_Covert_Ops, Zerg_Hatchery, None, Zerg_Queens_Nest,
		None, Zerg_Defiler_Mound, Zerg_Defiler_Mound, Zerg_Queens_Nest, None,
		Protoss_Templar_Archives, Protoss_Templar_Archives, Protoss_Arbiter_Tribunal,
		Protoss_Arbiter_Tribunal, None, Terran_Academy, Protoss_Fleet_Beacon, None,
		Protoss_Templar_Archives, None, None, Terran_Academy, Protoss_Templar_Archives,
		Zerg_Hydralisk_Den, None, None, None, None, None, None, None, None, None, None, None,
		None, None, Unknown
		};
	}
	namespace techInternalRaces
	{
		using namespace Races;
		static const Race techRaces[TechTypes::Enum::MAX] =
		{ Terran, Terran, Terran, Terran, Terran, Terran, Terran, Terran, Terran, Terran, Terran,
		Zerg, Zerg, Zerg, Zerg, Zerg, Zerg, Zerg, Zerg,
		Protoss, Protoss, Protoss, Protoss, Protoss, 
		Terran, Protoss, None, Protoss, Protoss, Protoss, Terran, Protoss, Zerg, None, Terran,
		None, None, None, None, None, None, None, None, None, None, Terran, Unknown
		};
	}
	namespace techInternalWeapons
	{
		using namespace WeaponTypes;
		static const WeaponType techWeapons[TechTypes::Enum::MAX] =
		{
			None, Lockdown, EMP_Shockwave, Spider_Mines, None, None, None, Irradiate, Yamato_Gun,
			None, None, None, None, Spawn_Broodlings, Dark_Swarm, Plague, Consume, Ensnare, Parasite,
			Psionic_Storm, None, None, Stasis_Field, None, Restoration, Disruption_Web, None, Mind_Control,
			None, Feedback, Optical_Flare, Maelstrom, None, None, None, None, None, None, None, None, None, 
			None, None, None, None, Nuclear_Strike, Unknown
		};
	}

#define TARG_UNIT	1
#define TARG_POS	2
#define TARG_BOTH	3
	static const int techTypeFlags[TechTypes::Enum::MAX] =
	{ 0, TARG_UNIT, TARG_BOTH, TARG_POS, TARG_BOTH, 0, TARG_UNIT, TARG_UNIT, TARG_UNIT, 0, 0, 0, 
	TARG_UNIT, TARG_UNIT, TARG_BOTH, TARG_BOTH, TARG_UNIT, TARG_BOTH, TARG_UNIT, TARG_BOTH, TARG_UNIT,
	TARG_BOTH, TARG_BOTH, TARG_UNIT, TARG_UNIT, TARG_BOTH, 0, TARG_UNIT, TARG_UNIT, TARG_UNIT, TARG_UNIT,
	TARG_BOTH, 0, 0, TARG_BOTH, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, TARG_BOTH };

	namespace techInternalOrders
	{
		using namespace Orders;
		static const Order techOrders[TechTypes::Enum::MAX] =
		{	None, CastLockdown, CastEMPShockwave, PlaceMine, CastScannerSweep, None, CastDefensiveMatrix,
			CastIrradiate, FireYamatoGun, None, None, None, CastInfestation, CastSpawnBroodlings,
			CastDarkSwarm, CastPlague, CastConsume, CastEnsnare, CastParasite, CastPsionicStorm,
			CastHallucination, CastRecall, CastStasisField, None, CastRestoration, CastDisruptionWeb,
			None, CastMindControl, None, CastFeedback, CastOpticalFlare, CastMaelstrom, None, None, MedicHeal1,
			None, None, None, None, None, None, None, None, None, None, NukePaint, Unknown
		};
	}
	namespace techInternalUsage
	{
		using namespace UnitTypes;
		/*static const UnitType techWhatUsesArr[TechTypes::Enum::MAX][] = 
		{	{ Terran_Marine, Terran_Firebat, Hero_Jim_Raynor_Marine, Hero_Gui_Montag},
			{ Terran_Ghost, Hero_Alexei_Stukov, Hero_Infested_Duran, Hero_Samir_Duran, Hero_Sarah_Kerrigan }
		};*/
		static const UnitType usesStim[] = { Terran_Marine, Terran_Firebat, Hero_Jim_Raynor_Marine, Hero_Gui_Montag };
		static const UnitType::set techWhatUses[TechTypes::Enum::MAX] = { UnitType::set(usesStim, countof(usesStim)) };
	}

	std::map<std::string, TechType> techTypeMap;
	namespace TechTypes
	{
		static const TechType techTypeArr[] = { Stim_Packs, Lockdown, EMP_Shockwave, Spider_Mines,
											Scanner_Sweep, Tank_Siege_Mode, Defensive_Matrix, Irradiate,
											Yamato_Gun, Cloaking_Field, Personnel_Cloaking, Burrowing,
											Infestation, Spawn_Broodlings, Dark_Swarm, Plague, Consume,
											Ensnare, Parasite, Psionic_Storm, Hallucination, Recall,
											Stasis_Field, Archon_Warp, Restoration, Disruption_Web, 
											Mind_Control, Dark_Archon_Meld, Feedback, Optical_Flare,
											Maelstrom, Lurker_Aspect, Healing, None, Unknown, Nuclear_Strike };
		static const TechType::set techTypeSet(techTypeArr, countof(techTypeArr));

		void init()
		{
			foreach(TechType i, techTypeSet)
			{
				std::string name(i.getName());
				fixName(&name);
				techTypeMap.insert(std::make_pair(name, i));
			}
		}
	}
	TechType::TechType(int id) : Type( id )
	{
	}
	const std::string &TechType::getName() const
	{
		return techTypeNames[this->getID()];
	}
	const char *TechType::c_str() const
	{
		return techTypeNames[this->getID()].c_str();
	}
	Race TechType::getRace() const
	{
		return techInternalRaces::techRaces[this->getID()];
	}
	int TechType::mineralPrice() const
	{
		return defaultOreCost[this->getID()];
	}
	int TechType::gasPrice() const
	{
		return defaultOreCost[this->getID()];
	}
	int TechType::researchTime() const
	{
		return defaultTimeCost[this->getID()];
	}
	int TechType::energyUsed() const
	{
		return defaultEnergyCost[this->getID()];
	}
	UnitType TechType::whatResearches() const
	{
		return techInternalResearches::whatResearches[this->getID()];
	}
	WeaponType TechType::getWeapon() const
	{
		return techInternalWeapons::techWeapons[this->getID()];
	}
	bool TechType::targetsUnit() const
	{
		return !!(techTypeFlags[this->getID()] & TARG_UNIT);
	}
	bool TechType::targetsPosition() const
	{
		return !!(techTypeFlags[this->getID()] & TARG_POS);
	}
	const UnitType::set& TechType::whatUses() const
	{
		return techInternalUsage::techWhatUses[this->getID()];
	}
	Order TechType::getOrder() const
	{
		return techInternalOrders::techOrders[this->getID()];
	}
	TechType TechTypes::getTechType(std::string name)
	{
		fixName(&name);
		std::map<std::string, TechType>::iterator i = techTypeMap.find(name);
		if (i == techTypeMap.end()) 
			return TechTypes::Unknown;
		return (*i).second;
	}
	const TechType::set& TechTypes::allTechTypes()
	{
		return TechTypes::techTypeSet;
	}
}

