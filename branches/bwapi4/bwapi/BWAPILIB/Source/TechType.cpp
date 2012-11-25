#include <string>
#include <BWAPI/TechType.h>
#include <BWAPI/Race.h>
#include <BWAPI/Order.h>
#include <BWAPI/UnitType.h>
#include <BWAPI/WeaponType.h>
#include <Util/Foreach.h>

#include "Common.h"

#include "../../Debug.h"

namespace BWAPI
{
  // NAMES
  const std::string TechType::typeNames[TechTypes::Enum::MAX] =
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
    using namespace UnitTypes::Enum;
    static const int whatResearches[TechTypes::Enum::MAX] =
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
    using namespace Races::Enum;
    static const int techRaces[TechTypes::Enum::MAX] =
    { Terran, Terran, Terran, Terran, Terran, Terran, Terran, Terran, Terran, Terran, Terran,
    Zerg, Zerg, Zerg, Zerg, Zerg, Zerg, Zerg, Zerg,
    Protoss, Protoss, Protoss, Protoss, Protoss, 
    Terran, Protoss, None, Protoss, Protoss, Protoss, Terran, Protoss, Zerg, None, Terran,
    None, None, None, None, None, None, None, None, None, None, Terran, Unknown
    };
  }
  namespace techInternalWeapons
  {
    using namespace WeaponTypes::Enum;
    static const int techWeapons[TechTypes::Enum::MAX] =
    {
      None, Lockdown, EMP_Shockwave, Spider_Mines, None, None, None, Irradiate, Yamato_Gun,
      None, None, None, None, Spawn_Broodlings, Dark_Swarm, Plague, Consume, Ensnare, Parasite,
      Psionic_Storm, None, None, Stasis_Field, None, Restoration, Disruption_Web, None, Mind_Control,
      None, Feedback, Optical_Flare, Maelstrom, None, None, None, None, None, None, None, None, None, 
      None, None, None, None, Nuclear_Strike, Unknown
    };
  }

#define TARG_UNIT  1
#define TARG_POS  2
#define TARG_BOTH  3
  static const int techTypeFlags[TechTypes::Enum::MAX] =
  { 0, TARG_UNIT, TARG_BOTH, TARG_POS, TARG_BOTH, 0, TARG_UNIT, TARG_UNIT, TARG_UNIT, 0, 0, 0, 
  TARG_UNIT, TARG_UNIT, TARG_BOTH, TARG_BOTH, TARG_UNIT, TARG_BOTH, TARG_UNIT, TARG_BOTH, TARG_UNIT,
  TARG_BOTH, TARG_BOTH, TARG_UNIT, TARG_UNIT, TARG_BOTH, 0, TARG_UNIT, TARG_UNIT, TARG_UNIT, TARG_UNIT,
  TARG_BOTH, 0, 0, TARG_BOTH, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, TARG_BOTH };

  namespace techInternalOrders
  {
    using namespace Orders::Enum;
    static const int techOrders[TechTypes::Enum::MAX] =
    {  None, CastLockdown, CastEMPShockwave, PlaceMine, CastScannerSweep, None, CastDefensiveMatrix,
      CastIrradiate, FireYamatoGun, None, None, None, CastInfestation, CastSpawnBroodlings,
      CastDarkSwarm, CastPlague, CastConsume, CastEnsnare, CastParasite, CastPsionicStorm,
      CastHallucination, CastRecall, CastStasisField, None, CastRestoration, CastDisruptionWeb,
      None, CastMindControl, None, CastFeedback, CastOpticalFlare, CastMaelstrom, None, None, MedicHeal,
      None, None, None, None, None, None, None, None, None, None, NukePaint, Unknown
    };
  }
  namespace techInternalUsage
  {
    using namespace UnitTypes::Enum;

    static const int Stim[] = { Terran_Marine, Terran_Firebat, Hero_Jim_Raynor_Marine, Hero_Gui_Montag };
    static const int Lockdown[] = { Terran_Ghost, Hero_Alexei_Stukov, Hero_Infested_Duran, Hero_Samir_Duran, Hero_Sarah_Kerrigan };
    static const int EMP[] = { Terran_Science_Vessel, Hero_Magellan };
    static const int Spider_Mine[] = { Terran_Vulture, Hero_Jim_Raynor_Vulture };
    static const int Scanner[] = { Terran_Comsat_Station };
    static const int Siege_Mode[] = { Terran_Siege_Tank_Tank_Mode, Terran_Siege_Tank_Siege_Mode, Hero_Edmund_Duke_Tank_Mode, Hero_Edmund_Duke_Siege_Mode };
    static const int Matrix[] = { Terran_Science_Vessel, Hero_Magellan };
    static const int Irradiate[] = { Terran_Science_Vessel, Hero_Magellan };
    static const int Yamato[] = { Terran_Battlecruiser, Hero_Gerard_DuGalle, Hero_Hyperion, Hero_Norad_II };
    static const int Cloaking_Field[] = { Terran_Wraith, Hero_Tom_Kazansky };
    static const int Personnel_Cloaking[] = { Terran_Ghost, Hero_Alexei_Stukov, Hero_Infested_Duran, Hero_Samir_Duran, Hero_Sarah_Kerrigan, Hero_Infested_Kerrigan };
    static const int Burrow[] = { Zerg_Zergling, Zerg_Hydralisk, Zerg_Drone, Zerg_Defiler, Zerg_Infested_Terran, Hero_Unclean_One, Hero_Hunter_Killer, Hero_Devouring_One, Zerg_Lurker };
    static const int Infest[] = { Zerg_Queen, Hero_Matriarch };
    static const int Broodlings[] = { Zerg_Queen, Hero_Matriarch };
    static const int Dark_Swarm[] = { Zerg_Defiler, Hero_Unclean_One };
    static const int Plague[] = { Zerg_Defiler, Hero_Unclean_One };
    static const int Consume[] = { Zerg_Defiler, Hero_Unclean_One, Hero_Infested_Kerrigan, Hero_Infested_Duran };
    static const int Ensnare[] = { Zerg_Queen, Hero_Matriarch, Hero_Infested_Kerrigan };
    static const int Parasite[] = { Zerg_Queen, Hero_Matriarch };
    static const int Psi_Storm[] = { Protoss_High_Templar, Hero_Tassadar, Hero_Infested_Kerrigan };
    static const int Hallucinate[] = { Protoss_High_Templar, Hero_Tassadar };
    static const int Recall[] = { Protoss_Arbiter, Hero_Danimoth };
    static const int Stasis[] = { Protoss_Arbiter, Hero_Danimoth };
    static const int Archon_Warp[] = { Protoss_High_Templar };
    static const int Restore[] = { Terran_Medic };
    static const int Disruption_Web[] = { Protoss_Corsair, Hero_Raszagal };
    static const int Mind_Control[] = { Protoss_Dark_Archon };
    static const int Dark_Archon_Meld[] = { Protoss_Dark_Templar };
    static const int Feedback[] = { Protoss_Dark_Archon };
    static const int Optical_Flare[] = { Terran_Medic };
    static const int Maelstrom[] = { Protoss_Dark_Archon };
    static const int Lurker_Aspect[] = { Zerg_Hydralisk };
    static const int Healing[] = { Terran_Medic };
    static const int Nuke[] = { Terran_Ghost };

#define TSET(x) UnitType::const_set(x, countof(x))
#define TSETEMPTY UnitType::const_set(&UnitTypes::None)

    static const UnitType::const_set techWhatUses[TechTypes::Enum::MAX] =
    {
      TSET(Stim), TSET(Lockdown), TSET(EMP), TSET(Spider_Mine), TSET(Scanner), TSET(Siege_Mode), TSET(Matrix), TSET(Irradiate), TSET(Yamato),
      TSET(Cloaking_Field), TSET(Personnel_Cloaking), TSET(Burrow), TSET(Infest), TSET(Broodlings), TSET(Dark_Swarm),
      TSET(Plague), TSET(Consume), TSET(Ensnare), TSET(Parasite), TSET(Psi_Storm), TSET(Hallucinate), TSET(Recall), TSET(Stasis),
      TSET(Archon_Warp), TSET(Restore), TSET(Disruption_Web), TSETEMPTY, TSET(Mind_Control), TSET(Dark_Archon_Meld),
      TSET(Feedback), TSET(Optical_Flare), TSET(Maelstrom), TSET(Lurker_Aspect), TSETEMPTY, TSET(Healing), TSETEMPTY,
      TSETEMPTY, TSETEMPTY, TSETEMPTY, TSETEMPTY, TSETEMPTY, TSETEMPTY, TSETEMPTY, TSETEMPTY, TSETEMPTY, TSET(Nuke), TSETEMPTY
    };
  }

  namespace TechTypeSet
  {
    using namespace TechTypes::Enum;
    BWAPI_TYPESET(techTypeSet, TechType, Stim_Packs, Lockdown, EMP_Shockwave, Spider_Mines,
                      Scanner_Sweep, Tank_Siege_Mode, Defensive_Matrix, Irradiate,
                      Yamato_Gun, Cloaking_Field, Personnel_Cloaking, Burrowing,
                      Infestation, Spawn_Broodlings, Dark_Swarm, Plague, Consume,
                      Ensnare, Parasite, Psionic_Storm, Hallucination, Recall,
                      Stasis_Field, Archon_Warp, Restoration, Disruption_Web, 
                      Mind_Control, Dark_Archon_Meld, Feedback, Optical_Flare,
                      Maelstrom, Lurker_Aspect, Healing, None, Nuclear_Strike, Unknown );
  }
  namespace TechTypes
  {
    BWAPI_TYPEDEF(TechType,Stim_Packs);
    BWAPI_TYPEDEF(TechType,Lockdown);
    BWAPI_TYPEDEF(TechType,EMP_Shockwave);
    BWAPI_TYPEDEF(TechType,Spider_Mines);
    BWAPI_TYPEDEF(TechType,Scanner_Sweep);
    BWAPI_TYPEDEF(TechType,Tank_Siege_Mode);
    BWAPI_TYPEDEF(TechType,Defensive_Matrix);
    BWAPI_TYPEDEF(TechType,Irradiate);
    BWAPI_TYPEDEF(TechType,Yamato_Gun);
    BWAPI_TYPEDEF(TechType,Cloaking_Field);
    BWAPI_TYPEDEF(TechType,Personnel_Cloaking);
    BWAPI_TYPEDEF(TechType,Burrowing);
    BWAPI_TYPEDEF(TechType,Infestation);
    BWAPI_TYPEDEF(TechType,Spawn_Broodlings);
    BWAPI_TYPEDEF(TechType,Dark_Swarm);
    BWAPI_TYPEDEF(TechType,Plague);
    BWAPI_TYPEDEF(TechType,Consume);
    BWAPI_TYPEDEF(TechType,Ensnare);
    BWAPI_TYPEDEF(TechType,Parasite);
    BWAPI_TYPEDEF(TechType,Psionic_Storm);
    BWAPI_TYPEDEF(TechType,Hallucination);
    BWAPI_TYPEDEF(TechType,Recall);
    BWAPI_TYPEDEF(TechType,Stasis_Field);
    BWAPI_TYPEDEF(TechType,Archon_Warp);
    BWAPI_TYPEDEF(TechType,Restoration);
    BWAPI_TYPEDEF(TechType,Disruption_Web);
    BWAPI_TYPEDEF(TechType,Mind_Control);
    BWAPI_TYPEDEF(TechType,Dark_Archon_Meld);
    BWAPI_TYPEDEF(TechType,Feedback);
    BWAPI_TYPEDEF(TechType,Optical_Flare);
    BWAPI_TYPEDEF(TechType,Maelstrom);
    BWAPI_TYPEDEF(TechType,Lurker_Aspect);
    BWAPI_TYPEDEF(TechType,Healing);
    BWAPI_TYPEDEF(TechType,None);
    BWAPI_TYPEDEF(TechType,Nuclear_Strike);
    BWAPI_TYPEDEF(TechType,Unknown);
  }
  TechType::TechType(int id) : Type( id )
  {
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
  int TechType::energyCost() const
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
  const UnitType::const_set& TechType::whatUses() const
  {
    return techInternalUsage::techWhatUses[this->getID()];
  }
  Order TechType::getOrder() const
  {
    return techInternalOrders::techOrders[this->getID()];
  }
  const TechType::const_set& TechTypes::allTechTypes()
  {
    return TechTypeSet::techTypeSet;
  }
}

