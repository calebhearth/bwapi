#include <string>
#include <BWAPI/UnitType.h>
#include <BWAPI/WeaponType.h>
#include <BWAPI/TechType.h>
#include <BWAPI/UpgradeType.h>
#include <BWAPI/DamageType.h>
#include <BWAPI/ExplosionType.h>
#include <Util/Foreach.h>

#include "Common.h"

#include "../../Debug.h"

namespace BWAPI
{
  const std::string WeaponType::typeNames[WeaponTypes::Enum::MAX] = 
  {
    "Gauss_Rifle",
    "Gauss_Rifle_Jim_Raynor",
    "C_10_Canister_Rifle",
    "C_10_Canister_Rifle_Sarah_Kerrigan",
    "Fragmentation_Grenade",
    "Fragmentation_Grenade_Jim_Raynor",
    "Spider_Mines",
    "Twin_Autocannons",
    "Hellfire_Missile_Pack",
    "Twin_Autocannons_Alan_Schezar",
    "Hellfire_Missile_Pack_Alan_Schezar",
    "Arclite_Cannon",
    "Arclite_Cannon_Edmund_Duke",
    "Fusion_Cutter",
    "",
    "Gemini_Missiles",
    "Burst_Lasers",
    "Gemini_Missiles_Tom_Kazansky",
    "Burst_Lasers_Tom_Kazansky",
    "ATS_Laser_Battery",
    "ATA_Laser_Battery",
    "ATS_Laser_Battery_Hero",
    "ATA_Laser_Battery_Hero",
    "ATS_Laser_Battery_Hyperion",
    "ATA_Laser_Battery_Hyperion",
    "Flame_Thrower",
    "Flame_Thrower_Gui_Montag",
    "Arclite_Shock_Cannon",
    "Arclite_Shock_Cannon_Edmund_Duke",
    "Longbolt_Missile",
    "Yamato_Gun",
    "Nuclear_Strike",
    "Lockdown",
    "EMP_Shockwave",
    "Irradiate",
    "Claws",
    "Claws_Devouring_One",
    "Claws_Infested_Kerrigan",
    "Needle_Spines",
    "Needle_Spines_Hunter_Killer",
    "Kaiser_Blades",
    "Kaiser_Blades_Torrasque",
    "Toxic_Spores",
    "Spines",
    "",
    "",
    "Acid_Spore",
    "Acid_Spore_Kukulza",
    "Glave_Wurm",
    "Glave_Wurm_Kukulza",
    "",
    "",
    "Seeker_Spores",
    "Subterranean_Tentacle",
    "Suicide_Infested_Terran",
    "Suicide_Scourge",
    "Parasite",
    "Spawn_Broodlings",
    "Ensnare",
    "Dark_Swarm",
    "Plague",
    "Consume",
    "Particle_Beam",
    "",
    "Psi_Blades",
    "Psi_Blades_Fenix",
    "Phase_Disruptor",
    "Phase_Disruptor_Fenix",
    "",
    "Psi_Assault",
    "Psionic_Shockwave",
    "Psionic_Shockwave_TZ_Archon",
    "",
    "Dual_Photon_Blasters",
    "Anti_Matter_Missiles",
    "Dual_Photon_Blasters_Mojo",
    "Anti_Matter_Missiles_Mojo",
    "Phase_Disruptor_Cannon",
    "Phase_Disruptor_Cannon_Danimoth",
    "Pulse_Cannon",
    "STS_Photon_Cannon",
    "STA_Photon_Cannon",
    "Scarab",
    "Stasis_Field",
    "Psionic_Storm",
    "Warp_Blades_Zeratul",
    "Warp_Blades_Hero",
    "",
    "",
    "",
    "",
    "",
    "",
    "Independant_Laser_Battery",
    "",
    "",
    "Twin_Autocannons_Floor_Trap",
    "Hellfire_Missile_Pack_Wall_Trap",
    "Flame_Thrower_Wall_Trap",
    "Hellfire_Missile_Pack_Floor_Trap",
    "Neutron_Flare",
    "Disruption_Web",
    "Restoration",
    "Halo_Rockets",
    "Corrosive_Acid",
    "Mind_Control",
    "Feedback",
    "Optical_Flare",
    "Maelstrom",
    "Subterranean_Spines",
    "",
    "Warp_Blades",
    "C_10_Canister_Rifle_Samir_Duran",
    "C_10_Canister_Rifle_Infested_Duran",
    "Dual_Photon_Blasters_Artanis",
    "Anti_Matter_Missiles_Artanis",
    "C_10_Canister_Rifle_Alexei_Stukov",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "None",
    "Unknown"
  };

  class WeaponTypeInternal
  {
    public:
      WeaponTypeInternal() {valid = false;}
      void set(const char* name, TechType techType, int damageAmount, int damageBonus, int damageCooldown, int damageFactor, UpgradeType upgradeType, DamageType damageType, ExplosionType explosionType, int minRange, int maxRange, int innerSplashRadius, int medianSplashRadius, int outerSplashRadius, bool targetsAir, bool targetsGround, bool targetsMechanical, bool targetsOrganic, bool targetsNonBuilding, bool targetsNonRobotic, bool targetsTerrain, bool targetsOrgOrMech, bool targetsOwn, UnitType whatUses)
      {
        this->name               = name;
        this->techType           = techType;
        this->damageAmount       = damageAmount;
        this->damageBonus        = damageBonus;
        this->damageCooldown     = damageCooldown;
        this->damageFactor       = damageFactor;
        this->upgradeType        = upgradeType;
        this->damageType         = damageType;
        this->explosionType      = explosionType;
        this->minRange           = minRange;
        this->maxRange           = maxRange;
        this->innerSplashRadius  = innerSplashRadius;
        this->medianSplashRadius = medianSplashRadius;
        this->outerSplashRadius  = outerSplashRadius;
        this->targetsAir         = targetsAir;
        this->targetsGround      = targetsGround;
        this->targetsMechanical  = targetsMechanical;
        this->targetsOrganic     = targetsOrganic;
        this->targetsNonBuilding = targetsNonBuilding;
        this->targetsNonRobotic  = targetsNonRobotic;
        this->targetsTerrain     = targetsTerrain;
        this->targetsOrgOrMech   = targetsOrgOrMech;
        this->targetsOwn         = targetsOwn;
        this->whatUses           = whatUses;
        this->valid              = true;
      }
      std::string name;
      TechType techType;
      UnitType whatUses;
      int damageAmount;
      int damageBonus;
      int damageCooldown;
      int damageFactor;
      UpgradeType upgradeType;
      DamageType damageType;
      ExplosionType explosionType;
      int minRange;
      int maxRange;
      int innerSplashRadius;
      int medianSplashRadius;
      int outerSplashRadius;
      bool targetsAir;
      bool targetsGround;
      bool targetsMechanical;
      bool targetsOrganic;
      bool targetsNonBuilding;
      bool targetsNonRobotic;
      bool targetsTerrain;
      bool targetsOrgOrMech;
      bool targetsOwn;
      bool valid;
  };
  WeaponTypeInternal weaponTypeData[132];
  namespace WeaponTypesSet
  {
    using namespace WeaponTypes::Enum;
    BWAPI_TYPESET(specialWeaponTypeSet, WeaponType, Yamato_Gun, Nuclear_Strike, Lockdown, EMP_Shockwave, 
                  Irradiate, Parasite, Spawn_Broodlings, Ensnare, Dark_Swarm, 
                  Plague, Consume, Stasis_Field, Psionic_Storm, Disruption_Web, 
                  Restoration, Mind_Control, Feedback, Optical_Flare, Maelstrom );
    BWAPI_TYPESET(normalWeaponTypeSet, WeaponType, Gauss_Rifle, Gauss_Rifle_Jim_Raynor, C_10_Canister_Rifle,
                  C_10_Canister_Rifle_Sarah_Kerrigan, C_10_Canister_Rifle_Samir_Duran, 
                  C_10_Canister_Rifle_Infested_Duran, C_10_Canister_Rifle_Alexei_Stukov, 
                  Fragmentation_Grenade, Fragmentation_Grenade_Jim_Raynor, Spider_Mines, 
                  Twin_Autocannons, Twin_Autocannons_Alan_Schezar, Hellfire_Missile_Pack, 
                  Hellfire_Missile_Pack_Alan_Schezar, Arclite_Cannon, Arclite_Cannon_Edmund_Duke, 
                  Fusion_Cutter, Gemini_Missiles, Gemini_Missiles_Tom_Kazansky, Burst_Lasers, 
                  Burst_Lasers_Tom_Kazansky, ATS_Laser_Battery, ATS_Laser_Battery_Hero, 
                  ATS_Laser_Battery_Hyperion, ATA_Laser_Battery, ATA_Laser_Battery_Hero, 
                  ATA_Laser_Battery_Hyperion, Flame_Thrower, Flame_Thrower_Gui_Montag, 
                  Arclite_Shock_Cannon, Arclite_Shock_Cannon_Edmund_Duke, Longbolt_Missile, 
                  Claws, Claws_Devouring_One, Claws_Infested_Kerrigan, Needle_Spines, 
                  Needle_Spines_Hunter_Killer, Kaiser_Blades, Kaiser_Blades_Torrasque, 
                  Toxic_Spores, Spines, Acid_Spore, Acid_Spore_Kukulza, Glave_Wurm, 
                  Glave_Wurm_Kukulza, Seeker_Spores, Subterranean_Tentacle, 
                  Suicide_Infested_Terran, Suicide_Scourge, Particle_Beam, Psi_Blades, 
                  Psi_Blades_Fenix, Phase_Disruptor, Phase_Disruptor_Fenix, Psi_Assault, 
                  Psionic_Shockwave, Psionic_Shockwave_TZ_Archon, Dual_Photon_Blasters, 
                  Dual_Photon_Blasters_Mojo, Dual_Photon_Blasters_Artanis, Anti_Matter_Missiles, 
                  Anti_Matter_Missiles_Mojo, Anti_Matter_Missiles_Artanis, Phase_Disruptor_Cannon, 
                  Phase_Disruptor_Cannon_Danimoth, Pulse_Cannon, STS_Photon_Cannon, 
                  STA_Photon_Cannon, Scarab, Neutron_Flare, Halo_Rockets, Corrosive_Acid, 
                  Subterranean_Spines, Warp_Blades, Warp_Blades_Hero, Warp_Blades_Zeratul, 
                  Independant_Laser_Battery, Twin_Autocannons_Floor_Trap, Hellfire_Missile_Pack_Wall_Trap, 
                  Hellfire_Missile_Pack_Floor_Trap, Flame_Thrower_Wall_Trap);
    static const WeaponType::set weaponTypeSet( specialWeaponTypeSet | normalWeaponTypeSet | WeaponTypes::None | WeaponTypes::Unknown );

  }
  namespace WeaponTypes
  {
    BWAPI_TYPEDEF(WeaponType,Gauss_Rifle);
    BWAPI_TYPEDEF(WeaponType,Gauss_Rifle_Jim_Raynor);
    BWAPI_TYPEDEF(WeaponType,C_10_Canister_Rifle);
    BWAPI_TYPEDEF(WeaponType,C_10_Canister_Rifle_Sarah_Kerrigan);
    BWAPI_TYPEDEF(WeaponType,C_10_Canister_Rifle_Samir_Duran);
    BWAPI_TYPEDEF(WeaponType,C_10_Canister_Rifle_Infested_Duran);
    BWAPI_TYPEDEF(WeaponType,C_10_Canister_Rifle_Alexei_Stukov);
    BWAPI_TYPEDEF(WeaponType,Fragmentation_Grenade);
    BWAPI_TYPEDEF(WeaponType,Fragmentation_Grenade_Jim_Raynor);
    BWAPI_TYPEDEF(WeaponType,Spider_Mines);
    BWAPI_TYPEDEF(WeaponType,Twin_Autocannons);
    BWAPI_TYPEDEF(WeaponType,Twin_Autocannons_Alan_Schezar);
    BWAPI_TYPEDEF(WeaponType,Hellfire_Missile_Pack);
    BWAPI_TYPEDEF(WeaponType,Hellfire_Missile_Pack_Alan_Schezar);
    BWAPI_TYPEDEF(WeaponType,Arclite_Cannon);
    BWAPI_TYPEDEF(WeaponType,Arclite_Cannon_Edmund_Duke);
    BWAPI_TYPEDEF(WeaponType,Fusion_Cutter);
    BWAPI_TYPEDEF(WeaponType,Gemini_Missiles);
    BWAPI_TYPEDEF(WeaponType,Gemini_Missiles_Tom_Kazansky);
    BWAPI_TYPEDEF(WeaponType,Burst_Lasers);
    BWAPI_TYPEDEF(WeaponType,Burst_Lasers_Tom_Kazansky);
    BWAPI_TYPEDEF(WeaponType,ATS_Laser_Battery);
    BWAPI_TYPEDEF(WeaponType,ATS_Laser_Battery_Hero);
    BWAPI_TYPEDEF(WeaponType,ATS_Laser_Battery_Hyperion);
    BWAPI_TYPEDEF(WeaponType,ATA_Laser_Battery);
    BWAPI_TYPEDEF(WeaponType,ATA_Laser_Battery_Hero);
    BWAPI_TYPEDEF(WeaponType,ATA_Laser_Battery_Hyperion);
    BWAPI_TYPEDEF(WeaponType,Flame_Thrower);
    BWAPI_TYPEDEF(WeaponType,Flame_Thrower_Gui_Montag);
    BWAPI_TYPEDEF(WeaponType,Arclite_Shock_Cannon);
    BWAPI_TYPEDEF(WeaponType,Arclite_Shock_Cannon_Edmund_Duke);
    BWAPI_TYPEDEF(WeaponType,Longbolt_Missile);
    BWAPI_TYPEDEF(WeaponType,Claws);
    BWAPI_TYPEDEF(WeaponType,Claws_Devouring_One);
    BWAPI_TYPEDEF(WeaponType,Claws_Infested_Kerrigan);
    BWAPI_TYPEDEF(WeaponType,Needle_Spines);
    BWAPI_TYPEDEF(WeaponType,Needle_Spines_Hunter_Killer);
    BWAPI_TYPEDEF(WeaponType,Kaiser_Blades);
    BWAPI_TYPEDEF(WeaponType,Kaiser_Blades_Torrasque);
    BWAPI_TYPEDEF(WeaponType,Toxic_Spores);
    BWAPI_TYPEDEF(WeaponType,Spines);
    BWAPI_TYPEDEF(WeaponType,Acid_Spore);
    BWAPI_TYPEDEF(WeaponType,Acid_Spore_Kukulza);
    BWAPI_TYPEDEF(WeaponType,Glave_Wurm);
    BWAPI_TYPEDEF(WeaponType,Glave_Wurm_Kukulza);
    BWAPI_TYPEDEF(WeaponType,Seeker_Spores);
    BWAPI_TYPEDEF(WeaponType,Subterranean_Tentacle);
    BWAPI_TYPEDEF(WeaponType,Suicide_Infested_Terran);
    BWAPI_TYPEDEF(WeaponType,Suicide_Scourge);
    BWAPI_TYPEDEF(WeaponType,Particle_Beam);
    BWAPI_TYPEDEF(WeaponType,Psi_Blades);
    BWAPI_TYPEDEF(WeaponType,Psi_Blades_Fenix);
    BWAPI_TYPEDEF(WeaponType,Phase_Disruptor);
    BWAPI_TYPEDEF(WeaponType,Phase_Disruptor_Fenix);
    BWAPI_TYPEDEF(WeaponType,Psi_Assault);
    BWAPI_TYPEDEF(WeaponType,Psionic_Shockwave);
    BWAPI_TYPEDEF(WeaponType,Psionic_Shockwave_TZ_Archon);
    BWAPI_TYPEDEF(WeaponType,Dual_Photon_Blasters);
    BWAPI_TYPEDEF(WeaponType,Dual_Photon_Blasters_Mojo);
    BWAPI_TYPEDEF(WeaponType,Dual_Photon_Blasters_Artanis);
    BWAPI_TYPEDEF(WeaponType,Anti_Matter_Missiles);
    BWAPI_TYPEDEF(WeaponType,Anti_Matter_Missiles_Mojo);
    BWAPI_TYPEDEF(WeaponType,Anti_Matter_Missiles_Artanis);
    BWAPI_TYPEDEF(WeaponType,Phase_Disruptor_Cannon);
    BWAPI_TYPEDEF(WeaponType,Phase_Disruptor_Cannon_Danimoth);
    BWAPI_TYPEDEF(WeaponType,Pulse_Cannon);
    BWAPI_TYPEDEF(WeaponType,STS_Photon_Cannon);
    BWAPI_TYPEDEF(WeaponType,STA_Photon_Cannon);
    BWAPI_TYPEDEF(WeaponType,Scarab);
    BWAPI_TYPEDEF(WeaponType,Neutron_Flare);
    BWAPI_TYPEDEF(WeaponType,Halo_Rockets);
    BWAPI_TYPEDEF(WeaponType,Corrosive_Acid);
    BWAPI_TYPEDEF(WeaponType,Subterranean_Spines);
    BWAPI_TYPEDEF(WeaponType,Warp_Blades);
    BWAPI_TYPEDEF(WeaponType,Warp_Blades_Hero);
    BWAPI_TYPEDEF(WeaponType,Warp_Blades_Zeratul);
    BWAPI_TYPEDEF(WeaponType,Independant_Laser_Battery);
    BWAPI_TYPEDEF(WeaponType,Twin_Autocannons_Floor_Trap);
    BWAPI_TYPEDEF(WeaponType,Hellfire_Missile_Pack_Wall_Trap);
    BWAPI_TYPEDEF(WeaponType,Flame_Thrower_Wall_Trap);
    BWAPI_TYPEDEF(WeaponType,Hellfire_Missile_Pack_Floor_Trap);

    BWAPI_TYPEDEF(WeaponType,Yamato_Gun);
    BWAPI_TYPEDEF(WeaponType,Nuclear_Strike);
    BWAPI_TYPEDEF(WeaponType,Lockdown);
    BWAPI_TYPEDEF(WeaponType,EMP_Shockwave);
    BWAPI_TYPEDEF(WeaponType,Irradiate);
    BWAPI_TYPEDEF(WeaponType,Parasite);
    BWAPI_TYPEDEF(WeaponType,Spawn_Broodlings);
    BWAPI_TYPEDEF(WeaponType,Ensnare);
    BWAPI_TYPEDEF(WeaponType,Dark_Swarm);
    BWAPI_TYPEDEF(WeaponType,Plague);
    BWAPI_TYPEDEF(WeaponType,Consume);
    BWAPI_TYPEDEF(WeaponType,Stasis_Field);
    BWAPI_TYPEDEF(WeaponType,Psionic_Storm);
    BWAPI_TYPEDEF(WeaponType,Disruption_Web);
    BWAPI_TYPEDEF(WeaponType,Restoration);
    BWAPI_TYPEDEF(WeaponType,Mind_Control);
    BWAPI_TYPEDEF(WeaponType,Feedback);
    BWAPI_TYPEDEF(WeaponType,Optical_Flare);
    BWAPI_TYPEDEF(WeaponType,Maelstrom);

    BWAPI_TYPEDEF(WeaponType,None);
    BWAPI_TYPEDEF(WeaponType,Unknown);

    void init()
    {
      weaponTypeData[Gauss_Rifle].set("Gauss Rifle", TechTypes::None, 6, 1, 15, 1, UpgradeTypes::Terran_Infantry_Weapons, DamageTypes::Normal, ExplosionTypes::Normal, 0, 128, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Terran_Marine);
      weaponTypeData[Gauss_Rifle_Jim_Raynor].set("Gauss Rifle (Jim Raynor)", TechTypes::None, 18, 1, 15, 1, UpgradeTypes::Terran_Infantry_Weapons, DamageTypes::Normal, ExplosionTypes::Normal, 0, 160, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Hero_Jim_Raynor_Marine);
      weaponTypeData[C_10_Canister_Rifle].set("C-10 Canister Rifle", TechTypes::None, 10, 1, 22, 1, UpgradeTypes::Terran_Infantry_Weapons, DamageTypes::Concussive, ExplosionTypes::Normal, 0, 224, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Terran_Ghost);
      weaponTypeData[C_10_Canister_Rifle_Sarah_Kerrigan].set("C-10 Canister Rifle (Sarah Kerrigan)", TechTypes::None, 30, 1, 22, 1, UpgradeTypes::Terran_Infantry_Weapons, DamageTypes::Concussive, ExplosionTypes::Normal, 0, 192, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Hero_Sarah_Kerrigan);
      weaponTypeData[C_10_Canister_Rifle_Samir_Duran].set("C-10 Canister Rifle (Samir Duran)", TechTypes::None, 25, 1, 22, 1, UpgradeTypes::Terran_Infantry_Weapons, DamageTypes::Concussive, ExplosionTypes::Normal, 0, 192, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Hero_Samir_Duran);
      weaponTypeData[C_10_Canister_Rifle_Infested_Duran].set("C-10 Canister Rifle (Infested Duran)", TechTypes::None, 25, 1, 22, 1, UpgradeTypes::Terran_Infantry_Weapons, DamageTypes::Concussive, ExplosionTypes::Normal, 0, 192, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Hero_Infested_Duran);
      weaponTypeData[C_10_Canister_Rifle_Alexei_Stukov].set("C-10 Canister Rifle (Alexei Stukov)", TechTypes::None, 30, 1, 22, 1, UpgradeTypes::Terran_Infantry_Weapons, DamageTypes::Concussive, ExplosionTypes::Normal, 0, 192, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Hero_Alexei_Stukov);
      weaponTypeData[Fragmentation_Grenade].set("Fragmentation Grenade", TechTypes::None, 20, 2, 30, 1, UpgradeTypes::Terran_Vehicle_Weapons, DamageTypes::Concussive, ExplosionTypes::Normal, 0, 160, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Terran_Vulture);
      weaponTypeData[Fragmentation_Grenade_Jim_Raynor].set("Fragmentation Grenade (Jim Raynor)", TechTypes::None, 30, 2, 22, 1, UpgradeTypes::Terran_Vehicle_Weapons, DamageTypes::Concussive, ExplosionTypes::Normal, 0, 160, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Hero_Jim_Raynor_Vulture);
      weaponTypeData[Spider_Mines].set("Spider Mines", TechTypes::Spider_Mines, 125, 0, 22, 1, UpgradeTypes::None, DamageTypes::Explosive, ExplosionTypes::Radial_Splash, 0, 10, 50, 75, 100, 0, 1, 0, 0, 1, 0, 0, 0, 0, UnitTypes::Terran_Vulture_Spider_Mine);
      weaponTypeData[Twin_Autocannons].set("Twin Autocannons", TechTypes::None, 12, 1, 22, 1, UpgradeTypes::Terran_Vehicle_Weapons, DamageTypes::Normal, ExplosionTypes::Normal, 0, 192, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Terran_Goliath);
      weaponTypeData[Twin_Autocannons_Alan_Schezar].set("Twin Autocannons (Alan Schezar)", TechTypes::None, 24, 1, 22, 1, UpgradeTypes::Terran_Vehicle_Weapons, DamageTypes::Normal, ExplosionTypes::Normal, 0, 160, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Hero_Alan_Schezar);
      weaponTypeData[Hellfire_Missile_Pack].set("Hellfire Missile Pack", TechTypes::None, 10, 2, 22, 2, UpgradeTypes::Terran_Vehicle_Weapons, DamageTypes::Explosive, ExplosionTypes::Normal, 0, 160, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Terran_Goliath);
      weaponTypeData[Hellfire_Missile_Pack_Alan_Schezar].set("Hellfire Missile Pack (Alan Schezar)", TechTypes::None, 20, 1, 22, 2, UpgradeTypes::Terran_Vehicle_Weapons, DamageTypes::Explosive, ExplosionTypes::Normal, 0, 160, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Hero_Alan_Schezar);
      weaponTypeData[Arclite_Cannon].set("Arclite Cannon", TechTypes::None, 30, 3, 37, 1, UpgradeTypes::Terran_Vehicle_Weapons, DamageTypes::Explosive, ExplosionTypes::Normal, 0, 224, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Terran_Siege_Tank_Tank_Mode);
      weaponTypeData[Arclite_Cannon_Edmund_Duke].set("Arclite Cannon (Edmund Duke)", TechTypes::None, 70, 3, 37, 1, UpgradeTypes::Terran_Vehicle_Weapons, DamageTypes::Explosive, ExplosionTypes::Normal, 0, 224, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Hero_Edmund_Duke_Tank_Mode);
      weaponTypeData[Fusion_Cutter].set("Fusion Cutter", TechTypes::None, 5, 1, 15, 1, UpgradeTypes::None, DamageTypes::Normal, ExplosionTypes::Normal, 0, 10, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Terran_SCV);
      weaponTypeData[Gemini_Missiles].set("Gemini Missiles", TechTypes::None, 20, 2, 22, 1, UpgradeTypes::Terran_Ship_Weapons, DamageTypes::Explosive, ExplosionTypes::Normal, 0, 160, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Terran_Wraith);
      weaponTypeData[Gemini_Missiles_Tom_Kazansky].set("Gemini Missiles (Tom Kazansky)", TechTypes::None, 40, 2, 22, 1, UpgradeTypes::Terran_Ship_Weapons, DamageTypes::Explosive, ExplosionTypes::Normal, 0, 160, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Hero_Tom_Kazansky);
      weaponTypeData[Burst_Lasers].set("Burst Lasers", TechTypes::None, 8, 1, 30, 1, UpgradeTypes::Terran_Ship_Weapons, DamageTypes::Normal, ExplosionTypes::Normal, 0, 160, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Terran_Wraith);
      weaponTypeData[Burst_Lasers_Tom_Kazansky].set("Burst Lasers (Tom Kazansky)", TechTypes::None, 16, 1, 30, 1, UpgradeTypes::Terran_Ship_Weapons, DamageTypes::Normal, ExplosionTypes::Normal, 0, 160, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Hero_Tom_Kazansky);
      weaponTypeData[ATS_Laser_Battery].set("ATS Laser Battery", TechTypes::None, 25, 3, 30, 1, UpgradeTypes::Terran_Ship_Weapons, DamageTypes::Normal, ExplosionTypes::Normal, 0, 192, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Terran_Battlecruiser);
      weaponTypeData[ATS_Laser_Battery_Hero].set("ATS Laser Battery (Hero)", TechTypes::None, 50, 3, 30, 1, UpgradeTypes::Terran_Ship_Weapons, DamageTypes::Normal, ExplosionTypes::Normal, 0, 192, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Hero_Norad_II);
      weaponTypeData[ATS_Laser_Battery_Hyperion].set("ATS Laser Battery (Hyperion)", TechTypes::None, 30, 3, 22, 1, UpgradeTypes::Terran_Ship_Weapons, DamageTypes::Normal, ExplosionTypes::Normal, 0, 192, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Hero_Hyperion);
      weaponTypeData[ATA_Laser_Battery].set("ATA Laser Battery", TechTypes::None, 25, 3, 30, 1, UpgradeTypes::Terran_Ship_Weapons, DamageTypes::Normal, ExplosionTypes::Normal, 0, 192, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Terran_Battlecruiser);
      weaponTypeData[ATA_Laser_Battery_Hero].set("ATA Laser Battery (Hero)", TechTypes::None, 50, 3, 30, 1, UpgradeTypes::Terran_Ship_Weapons, DamageTypes::Normal, ExplosionTypes::Normal, 0, 192, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Hero_Norad_II);
      weaponTypeData[ATA_Laser_Battery_Hyperion].set("ATA Laser Battery (Hyperion)", TechTypes::None, 30, 3, 22, 1, UpgradeTypes::Terran_Ship_Weapons, DamageTypes::Normal, ExplosionTypes::Normal, 0, 192, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Hero_Hyperion);
      weaponTypeData[Flame_Thrower].set("Flame Thrower", TechTypes::None, 8, 1, 22, 1, UpgradeTypes::Terran_Infantry_Weapons, DamageTypes::Concussive, ExplosionTypes::Enemy_Splash, 0, 32, 15, 20, 25, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Terran_Firebat);
      weaponTypeData[Flame_Thrower_Gui_Montag].set("Flame Thrower (Gui Montag)", TechTypes::None, 16, 1, 22, 1, UpgradeTypes::Terran_Infantry_Weapons, DamageTypes::Concussive, ExplosionTypes::Enemy_Splash, 0, 32, 15, 20, 25, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Terran_Firebat);
      weaponTypeData[Arclite_Shock_Cannon].set("Arclite Shock Cannon", TechTypes::None, 70, 5, 75, 1, UpgradeTypes::Terran_Vehicle_Weapons, DamageTypes::Explosive, ExplosionTypes::Radial_Splash, 64, 384, 10, 25, 40, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Terran_Siege_Tank_Siege_Mode);
      weaponTypeData[Arclite_Shock_Cannon_Edmund_Duke].set("Arclite Shock Cannon (Edmund Duke)", TechTypes::None, 150, 5, 75, 1, UpgradeTypes::Terran_Vehicle_Weapons, DamageTypes::Explosive, ExplosionTypes::Radial_Splash, 64, 384, 10, 25, 40, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Hero_Edmund_Duke_Siege_Mode);
      weaponTypeData[Longbolt_Missile].set("Longbolt Missile", TechTypes::None, 20, 0, 15, 1, UpgradeTypes::None, DamageTypes::Explosive, ExplosionTypes::Normal, 0, 224, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Terran_Missile_Turret);
      weaponTypeData[Yamato_Gun].set("Yamato Gun", TechTypes::Yamato_Gun, 260, 0, 15, 1, UpgradeTypes::None, DamageTypes::Explosive, ExplosionTypes::Yamato_Gun, 0, 320, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Terran_Battlecruiser);
      weaponTypeData[Nuclear_Strike].set("Nuclear Strike", TechTypes::Nuclear_Strike, 600, 0, 1, 1, UpgradeTypes::None, DamageTypes::Explosive, ExplosionTypes::Nuclear_Missile, 0, 3, 128, 192, 256, 1, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Terran_Ghost);
      weaponTypeData[Lockdown].set("Lockdown", TechTypes::Lockdown, 0, 0, 1, 1, UpgradeTypes::None, DamageTypes::Concussive, ExplosionTypes::Lockdown, 0, 256, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, UnitTypes::Terran_Ghost);
      weaponTypeData[EMP_Shockwave].set("EMP Shockwave", TechTypes::EMP_Shockwave, 0, 0, 1, 1, UpgradeTypes::None, DamageTypes::Concussive, ExplosionTypes::EMP_Shockwave, 0, 256, 64, 64, 64, 1, 1, 0, 0, 0, 0, 1, 0, 0, UnitTypes::Terran_Science_Vessel);
      weaponTypeData[Irradiate].set("Irradiate", TechTypes::Irradiate, 250, 0, 75, 1, UpgradeTypes::None, DamageTypes::Ignore_Armor, ExplosionTypes::Irradiate, 0, 288, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, UnitTypes::Terran_Science_Vessel);
      weaponTypeData[Claws].set("Claws", TechTypes::None, 5, 1, 8, 1, UpgradeTypes::Zerg_Melee_Attacks, DamageTypes::Normal, ExplosionTypes::Normal, 0, 15, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Zerg_Zergling);
      weaponTypeData[Claws_Devouring_One].set("Claws (Devouring One)", TechTypes::None, 10, 1, 8, 1, UpgradeTypes::Zerg_Melee_Attacks, DamageTypes::Normal, ExplosionTypes::Normal, 0, 15, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Hero_Devouring_One);
      weaponTypeData[Claws_Infested_Kerrigan].set("Claws (Infested Kerrigan)", TechTypes::None, 50, 1, 15, 1, UpgradeTypes::Zerg_Melee_Attacks, DamageTypes::Normal, ExplosionTypes::Normal, 0, 15, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Hero_Infested_Kerrigan);
      weaponTypeData[Needle_Spines].set("Needle Spines", TechTypes::None, 10, 1, 15, 1, UpgradeTypes::Zerg_Missile_Attacks, DamageTypes::Explosive, ExplosionTypes::Normal, 0, 128, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Zerg_Hydralisk);
      weaponTypeData[Needle_Spines_Hunter_Killer].set("Needle Spines (Hunter Killer)", TechTypes::None, 20, 1, 15, 1, UpgradeTypes::Zerg_Missile_Attacks, DamageTypes::Explosive, ExplosionTypes::Normal, 0, 160, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Hero_Hunter_Killer);
      weaponTypeData[Kaiser_Blades].set("Kaiser Blades", TechTypes::None, 20, 3, 15, 1, UpgradeTypes::Zerg_Melee_Attacks, DamageTypes::Normal, ExplosionTypes::Normal, 0, 25, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Zerg_Ultralisk);
      weaponTypeData[Kaiser_Blades_Torrasque].set("Kaiser Blades (Torrasque)", TechTypes::None, 50, 3, 15, 1, UpgradeTypes::Zerg_Melee_Attacks, DamageTypes::Normal, ExplosionTypes::Normal, 0, 25, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Hero_Torrasque);
      weaponTypeData[Toxic_Spores].set("Toxic Spores", TechTypes::None, 4, 1, 15, 1, UpgradeTypes::Zerg_Melee_Attacks, DamageTypes::Normal, ExplosionTypes::Normal, 0, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Zerg_Broodling);
      weaponTypeData[Spines].set("Spines", TechTypes::None, 5, 0, 22, 1, UpgradeTypes::None, DamageTypes::Normal, ExplosionTypes::Normal, 0, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Zerg_Drone);
      weaponTypeData[Acid_Spore].set("Acid Spore", TechTypes::None, 20, 2, 30, 1, UpgradeTypes::Zerg_Flyer_Attacks, DamageTypes::Normal, ExplosionTypes::Normal, 0, 256, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Zerg_Guardian);
      weaponTypeData[Acid_Spore_Kukulza].set("Acid Spore (Kukulza)", TechTypes::None, 40, 2, 30, 1, UpgradeTypes::Zerg_Flyer_Attacks, DamageTypes::Normal, ExplosionTypes::Normal, 0, 256, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Hero_Kukulza_Guardian);
      weaponTypeData[Glave_Wurm].set("Glave Wurm", TechTypes::None, 9, 1, 30, 1, UpgradeTypes::Zerg_Flyer_Attacks, DamageTypes::Normal, ExplosionTypes::Normal, 0, 96, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Zerg_Mutalisk);
      weaponTypeData[Glave_Wurm_Kukulza].set("Glave Wurm (Kukulza)", TechTypes::None, 18, 1, 30, 1, UpgradeTypes::Zerg_Flyer_Attacks, DamageTypes::Normal, ExplosionTypes::Normal, 0, 96, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Hero_Kukulza_Mutalisk);
      weaponTypeData[Seeker_Spores].set("Seeker Spores", TechTypes::None, 15, 0, 15, 1, UpgradeTypes::None, DamageTypes::Normal, ExplosionTypes::Normal, 0, 224, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Zerg_Spore_Colony);
      weaponTypeData[Subterranean_Tentacle].set("Subterranean Tentacle", TechTypes::None, 40, 0, 32, 1, UpgradeTypes::None, DamageTypes::Explosive, ExplosionTypes::Normal, 0, 224, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Zerg_Sunken_Colony);
      weaponTypeData[Suicide_Infested_Terran].set("Suicide Infested Terran", TechTypes::None, 500, 0, 1, 1, UpgradeTypes::None, DamageTypes::Explosive, ExplosionTypes::Radial_Splash, 0, 3, 20, 40, 60, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Zerg_Infested_Terran);
      weaponTypeData[Suicide_Scourge].set("Suicide Scourge", TechTypes::None, 110, 0, 1, 1, UpgradeTypes::None, DamageTypes::Normal, ExplosionTypes::Normal, 0, 3, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Zerg_Scourge);
      weaponTypeData[Parasite].set("Parasite", TechTypes::Parasite, 0, 0, 1, 1, UpgradeTypes::None, DamageTypes::Independent, ExplosionTypes::Parasite, 0, 384, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, UnitTypes::Zerg_Queen);
      weaponTypeData[Spawn_Broodlings].set("Spawn Broodlings", TechTypes::Spawn_Broodlings, 0, 0, 1, 1, UpgradeTypes::None, DamageTypes::Independent, ExplosionTypes::Broodlings, 0, 288, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, UnitTypes::Zerg_Queen);
      weaponTypeData[Ensnare].set("Ensnare", TechTypes::Ensnare, 0, 0, 1, 1, UpgradeTypes::None, DamageTypes::Independent, ExplosionTypes::Ensnare, 0, 288, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, UnitTypes::Zerg_Queen);
      weaponTypeData[Dark_Swarm].set("Dark Swarm", TechTypes::Dark_Swarm, 0, 0, 1, 1, UpgradeTypes::None, DamageTypes::Independent, ExplosionTypes::Dark_Swarm, 0, 288, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Zerg_Defiler);
      weaponTypeData[Plague].set("Plague", TechTypes::Plague, 300, 0, 1, 1, UpgradeTypes::None, DamageTypes::Independent, ExplosionTypes::Plague, 0, 288, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, UnitTypes::Zerg_Defiler);
      weaponTypeData[Consume].set("Consume", TechTypes::Consume, 0, 0, 1, 1, UpgradeTypes::None, DamageTypes::Independent, ExplosionTypes::Consume, 0, 16, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, UnitTypes::Zerg_Defiler);
      weaponTypeData[Particle_Beam].set("Particle Beam", TechTypes::None, 5, 0, 22, 1, UpgradeTypes::None, DamageTypes::Normal, ExplosionTypes::Normal, 0, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Protoss_Probe);
      weaponTypeData[Psi_Blades].set("Psi Blades", TechTypes::None, 8, 1, 22, 1, UpgradeTypes::Protoss_Ground_Weapons, DamageTypes::Normal, ExplosionTypes::Normal, 0, 15, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Protoss_Zealot);
      weaponTypeData[Psi_Blades_Fenix].set("Psi Blades (Fenix)", TechTypes::None, 20, 1, 22, 1, UpgradeTypes::Protoss_Ground_Weapons, DamageTypes::Normal, ExplosionTypes::Normal, 0, 15, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Hero_Fenix_Zealot);
      weaponTypeData[Phase_Disruptor].set("Phase Disruptor", TechTypes::None, 20, 2, 30, 1, UpgradeTypes::Protoss_Ground_Weapons, DamageTypes::Explosive, ExplosionTypes::Normal, 0, 128, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Protoss_Dragoon);
      weaponTypeData[Phase_Disruptor_Fenix].set("Phase Disruptor (Fenix)", TechTypes::None, 45, 2, 22, 1, UpgradeTypes::Protoss_Ground_Weapons, DamageTypes::Explosive, ExplosionTypes::Normal, 0, 128, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Hero_Fenix_Dragoon);
      weaponTypeData[Psi_Assault].set("Psi Assault", TechTypes::None, 20, 1, 22, 1, UpgradeTypes::Protoss_Ground_Weapons, DamageTypes::Normal, ExplosionTypes::Normal, 0, 96, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Hero_Tassadar);
      weaponTypeData[Psionic_Shockwave].set("Psionic Shockwave", TechTypes::None, 30, 3, 20, 1, UpgradeTypes::Protoss_Ground_Weapons, DamageTypes::Normal, ExplosionTypes::Enemy_Splash, 0, 64, 3, 15, 30, 1, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Protoss_Archon);
      weaponTypeData[Psionic_Shockwave_TZ_Archon].set("Psionic Shockwave (Tassadar/Zeratul Archon)", TechTypes::None, 60, 3, 20, 1, UpgradeTypes::Protoss_Ground_Weapons, DamageTypes::Normal, ExplosionTypes::Enemy_Splash, 0, 64, 3, 15, 30, 1, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Hero_Tassadar_Zeratul_Archon);
      weaponTypeData[Dual_Photon_Blasters].set("Dual Photon Blasters", TechTypes::None, 8, 1, 30, 1, UpgradeTypes::Protoss_Air_Weapons, DamageTypes::Normal, ExplosionTypes::Normal, 0, 128, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Protoss_Scout);
      weaponTypeData[Dual_Photon_Blasters_Mojo].set("Dual Photon Blasters (Mojo)", TechTypes::None, 20, 1, 30, 1, UpgradeTypes::Protoss_Air_Weapons, DamageTypes::Normal, ExplosionTypes::Normal, 0, 128, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Hero_Mojo);
      weaponTypeData[Dual_Photon_Blasters_Artanis].set("Dual Photon Blasters (Artanis)", TechTypes::None, 20, 1, 30, 1, UpgradeTypes::Protoss_Air_Weapons, DamageTypes::Normal, ExplosionTypes::Normal, 0, 128, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Hero_Artanis);
      weaponTypeData[Anti_Matter_Missiles].set("Anti-Matter Missiles", TechTypes::None, 14, 1, 22, 2, UpgradeTypes::Protoss_Air_Weapons, DamageTypes::Explosive, ExplosionTypes::Normal, 0, 128, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Protoss_Scout);
      weaponTypeData[Anti_Matter_Missiles_Mojo].set("Anti-Matter Missiles (Mojo)", TechTypes::None, 28, 1, 22, 2, UpgradeTypes::Protoss_Air_Weapons, DamageTypes::Explosive, ExplosionTypes::Normal, 0, 128, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Hero_Mojo);
      weaponTypeData[Anti_Matter_Missiles_Artanis].set("Anti-Matter Missiles (Artanis)", TechTypes::None, 28, 1, 22, 2, UpgradeTypes::Protoss_Air_Weapons, DamageTypes::Explosive, ExplosionTypes::Normal, 0, 128, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Hero_Artanis);
      weaponTypeData[Phase_Disruptor_Cannon].set("Phase Disruptor Cannon", TechTypes::None, 10, 1, 45, 1, UpgradeTypes::Protoss_Air_Weapons, DamageTypes::Explosive, ExplosionTypes::Normal, 0, 160, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Protoss_Arbiter);
      weaponTypeData[Phase_Disruptor_Cannon_Danimoth].set("Phase Disruptor Cannon (Danimoth)", TechTypes::None, 20, 1, 45, 1, UpgradeTypes::Protoss_Air_Weapons, DamageTypes::Explosive, ExplosionTypes::Normal, 0, 160, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Hero_Danimoth);
      weaponTypeData[Pulse_Cannon].set("Pulse Cannon", TechTypes::None, 6, 1, 1, 1, UpgradeTypes::Protoss_Air_Weapons, DamageTypes::Normal, ExplosionTypes::Normal, 0, 128, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Protoss_Interceptor);
      weaponTypeData[STS_Photon_Cannon].set("STS Photon Cannon", TechTypes::None, 20, 0, 22, 1, UpgradeTypes::None, DamageTypes::Normal, ExplosionTypes::Normal, 0, 224, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Protoss_Photon_Cannon);
      weaponTypeData[STA_Photon_Cannon].set("STA Photon Cannon", TechTypes::None, 20, 0, 22, 1, UpgradeTypes::None, DamageTypes::Normal, ExplosionTypes::Normal, 0, 224, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Protoss_Photon_Cannon);
      weaponTypeData[Scarab].set("Scarab", TechTypes::None, 100, 25, 1, 1, UpgradeTypes::Scarab_Damage, DamageTypes::Normal, ExplosionTypes::Enemy_Splash, 0, 128, 20, 40, 60, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Protoss_Scarab);
      weaponTypeData[Stasis_Field].set("Stasis Field", TechTypes::Stasis_Field, 0, 1, 1, 1, UpgradeTypes::None, DamageTypes::Independent, ExplosionTypes::Stasis_Field, 0, 288, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, UnitTypes::Protoss_Arbiter);
      weaponTypeData[Psionic_Storm].set("Psionic Storm", TechTypes::Psionic_Storm, 14, 1, 45, 1, UpgradeTypes::None, DamageTypes::Ignore_Armor, ExplosionTypes::Radial_Splash, 0, 288, 48, 48, 48, 1, 1, 0, 0, 1, 0, 1, 0, 0, UnitTypes::Protoss_High_Templar);
      weaponTypeData[Neutron_Flare].set("Neutron Flare", TechTypes::None, 5, 1, 8, 1, UpgradeTypes::Protoss_Air_Weapons, DamageTypes::Explosive, ExplosionTypes::Air_Splash, 0, 160, 5, 50, 100, 1, 0, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Protoss_Corsair);
      weaponTypeData[Disruption_Web].set("Disruption Web", TechTypes::Disruption_Web, 0, 0, 22, 1, UpgradeTypes::None, DamageTypes::Ignore_Armor, ExplosionTypes::Disruption_Web, 0, 288, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Protoss_Corsair);
      weaponTypeData[Restoration].set("Restoration", TechTypes::Restoration, 20, 0, 22, 1, UpgradeTypes::None, DamageTypes::Ignore_Armor, ExplosionTypes::Restoration, 0, 192, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, UnitTypes::Terran_Medic);
      weaponTypeData[Halo_Rockets].set("Halo Rockets", TechTypes::None, 6, 1, 64, 2, UpgradeTypes::Terran_Ship_Weapons, DamageTypes::Explosive, ExplosionTypes::Air_Splash, 0, 192, 5, 50, 100, 1, 0, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Terran_Valkyrie);
      weaponTypeData[Corrosive_Acid].set("Corrosive Acid", TechTypes::None, 25, 2, 100, 1, UpgradeTypes::Zerg_Flyer_Attacks, DamageTypes::Explosive, ExplosionTypes::Corrosive_Acid, 0, 192, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Zerg_Devourer);
      weaponTypeData[Mind_Control].set("Mind Control", TechTypes::Mind_Control, 8, 1, 22, 1, UpgradeTypes::None, DamageTypes::Normal, ExplosionTypes::Mind_Control, 0, 256, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, UnitTypes::Protoss_Dark_Archon);
      weaponTypeData[Feedback].set("Feedback", TechTypes::Feedback, 8, 1, 22, 1, UpgradeTypes::None, DamageTypes::Ignore_Armor, ExplosionTypes::Feedback, 0, 320, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, UnitTypes::Protoss_Dark_Archon);
      weaponTypeData[Optical_Flare].set("Optical Flare", TechTypes::Optical_Flare, 8, 1, 22, 1, UpgradeTypes::None, DamageTypes::Independent, ExplosionTypes::Optical_Flare, 0, 288, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Terran_Medic);
      weaponTypeData[Maelstrom].set("Maelstrom", TechTypes::Maelstrom, 0, 1, 1, 1, UpgradeTypes::None, DamageTypes::Independent, ExplosionTypes::Maelstrom, 0, 320, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, UnitTypes::Protoss_Dark_Archon);
      weaponTypeData[Subterranean_Spines].set("Subterranean Spines", TechTypes::None, 20, 2, 37, 1, UpgradeTypes::Zerg_Missile_Attacks, DamageTypes::Normal, ExplosionTypes::Enemy_Splash, 0, 192, 20, 20, 20, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Zerg_Lurker);
      weaponTypeData[Warp_Blades].set("Warp Blades", TechTypes::None, 40, 3, 30, 1, UpgradeTypes::Protoss_Ground_Weapons, DamageTypes::Normal, ExplosionTypes::Normal, 0, 15, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Protoss_Dark_Templar);
      weaponTypeData[Warp_Blades_Hero].set("Warp Blades (Hero)", TechTypes::None, 45, 1, 30, 1, UpgradeTypes::Protoss_Ground_Weapons, DamageTypes::Normal, ExplosionTypes::Normal, 0, 15, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Hero_Dark_Templar);
      weaponTypeData[Warp_Blades_Zeratul].set("Warp Blades (Zeratul)", TechTypes::None, 100, 1, 22, 1, UpgradeTypes::Protoss_Ground_Weapons, DamageTypes::Normal, ExplosionTypes::Normal, 0, 15, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Hero_Zeratul);
      weaponTypeData[Independant_Laser_Battery].set("Independant Laser Battery", TechTypes::None, 7, 1, 22, 1, UpgradeTypes::None, DamageTypes::Normal, ExplosionTypes::Normal, 0, 8, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, UnitTypes::Special_Independant_Starport);
      weaponTypeData[Twin_Autocannons_Floor_Trap].set("Twin Autocannons (Floor Trap)"          , TechTypes::None  , 10              , 1              , 22                , 1               , UpgradeTypes::Terran_Vehicle_Weapons , DamageTypes::Normal    , ExplosionTypes::Normal      , 0           , 160         , 0                    , 0                     , 0                    , 0              , 1                 , 0                     , 0                  , 0                      , 0                     , 0                  , 0                    , 0              , UnitTypes::Special_Floor_Gun_Trap);
      weaponTypeData[Hellfire_Missile_Pack_Wall_Trap].set("Hellfire Missile Pack (Wall Trap)"  , TechTypes::None  , 10              , 1              , 22                , 2               , UpgradeTypes::Terran_Vehicle_Weapons , DamageTypes::Explosive , ExplosionTypes::Normal      , 0           , 160         , 0                    , 0                     , 0                    , 0              , 1                 , 0                     , 0                  , 0                      , 0                     , 0                  , 0                    , 0              , UnitTypes::Special_Wall_Missile_Trap);
      weaponTypeData[Hellfire_Missile_Pack_Floor_Trap].set("Hellfire Missile Pack (Floor Trap)", TechTypes::None  , 10              , 1              , 22                , 2               , UpgradeTypes::Terran_Vehicle_Weapons , DamageTypes::Explosive , ExplosionTypes::Normal      , 0           , 160         , 0                    , 0                     , 0                    , 0              , 1                 , 0                     , 0                  , 0                      , 0                     , 0                  , 0                    , 0              , UnitTypes::Special_Floor_Missile_Trap);
      weaponTypeData[Flame_Thrower_Wall_Trap].set("Flame Thrower (Wall Trap)"                  , TechTypes::None  , 8               , 1              , 22                , 1               , UpgradeTypes::Terran_Infantry_Weapons, DamageTypes::Concussive, ExplosionTypes::Enemy_Splash, 0           , 64          , 15                   , 20                    , 25                   , 0              , 1                 , 0                     , 0                  , 0                      , 0                     , 0                  , 0                    , 0              , UnitTypes::Special_Wall_Flame_Trap);

      weaponTypeData[None].set("None", TechTypes::None, 0, 0, 0, 0, UpgradeTypes::None, DamageTypes::None, ExplosionTypes::None, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, UnitTypes::None);
      weaponTypeData[Unknown].set("Unknown", TechTypes::None, 0, 0, 0, 0, UpgradeTypes::None, DamageTypes::None, ExplosionTypes::None, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, UnitTypes::None);
    }
  }
  WeaponType::WeaponType(int id) : Type( id )
  {
  }
  TechType WeaponType::getTech() const
  {
    return weaponTypeData[this->getID()].techType;
  }
  UnitType WeaponType::whatUses() const
  {
    return weaponTypeData[this->getID()].whatUses;
  }
  int WeaponType::damageAmount() const
  {
    return weaponTypeData[this->getID()].damageAmount;
  }
  int WeaponType::damageBonus() const
  {
    return weaponTypeData[this->getID()].damageBonus;
  }
  int WeaponType::damageCooldown() const
  {
    return weaponTypeData[this->getID()].damageCooldown;
  }
  int WeaponType::damageFactor() const
  {
    return weaponTypeData[this->getID()].damageFactor;
  }
  UpgradeType WeaponType::upgradeType() const
  {
    return weaponTypeData[this->getID()].upgradeType;
  }
  DamageType WeaponType::damageType() const
  {
    return weaponTypeData[this->getID()].damageType;
  }
  ExplosionType WeaponType::explosionType() const
  {
    return weaponTypeData[this->getID()].explosionType;
  }
  int WeaponType::minRange() const
  {
    return weaponTypeData[this->getID()].minRange;
  }
  int WeaponType::maxRange() const
  {
    return weaponTypeData[this->getID()].maxRange;
  }
  int WeaponType::innerSplashRadius() const
  {
    return weaponTypeData[this->getID()].innerSplashRadius;
  }
  int WeaponType::medianSplashRadius() const
  {
    return weaponTypeData[this->getID()].medianSplashRadius;
  }
  int WeaponType::outerSplashRadius() const
  {
    return weaponTypeData[this->getID()].outerSplashRadius;
  }
  bool WeaponType::targetsAir() const
  {
    return weaponTypeData[this->getID()].targetsAir;
  }
  bool WeaponType::targetsGround() const
  {
    return weaponTypeData[this->getID()].targetsGround;
  }
  bool WeaponType::targetsMechanical() const
  {
    return weaponTypeData[this->getID()].targetsMechanical;
  }
  bool WeaponType::targetsOrganic() const
  {
    return weaponTypeData[this->getID()].targetsOrganic;
  }
  bool WeaponType::targetsNonBuilding() const
  {
    return weaponTypeData[this->getID()].targetsNonBuilding;
  }
  bool WeaponType::targetsNonRobotic() const
  {
    return weaponTypeData[this->getID()].targetsNonRobotic;
  }
  bool WeaponType::targetsTerrain() const
  {
    return weaponTypeData[this->getID()].targetsTerrain;
  }
  bool WeaponType::targetsOrgOrMech() const
  {
    return weaponTypeData[this->getID()].targetsOrgOrMech;
  }
  bool WeaponType::targetsOwn() const
  {
    return weaponTypeData[this->getID()].targetsOwn;
  }
  const WeaponType::set& WeaponTypes::allWeaponTypes()
  {
    return WeaponTypesSet::weaponTypeSet;
  }
  const WeaponType::const_set& WeaponTypes::normalWeaponTypes()
  {
    return WeaponTypesSet::normalWeaponTypeSet;
  }
  const WeaponType::const_set& WeaponTypes::specialWeaponTypes()
  {
    return WeaponTypesSet::specialWeaponTypeSet;
  }
}
