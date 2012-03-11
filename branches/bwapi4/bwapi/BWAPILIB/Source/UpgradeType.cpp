#include <string>
#include <map>
#include <set>
#include <BWAPI/UpgradeType.h>
#include <BWAPI/Race.h>
#include <BWAPI/UnitType.h>
#include <Util/Foreach.h>

#include "Common.h"

#include "../../Debug.h"

namespace BWAPI
{
  class UpgradeTypeInternal
  {
    public:
      UpgradeTypeInternal() {valid = false;}
      void set(const char* name, int mineralPriceBase, int mineralPriceFactor, int gasPriceBase, int gasPriceFactor, int upgradeTimeBase, int upgradeTimeFactor, BWAPI::UnitType whatUpgrades, Race race, BWAPI::UnitType whatUses, int maxRepeats = 1, BWAPI::UnitType reqLv1 = UnitTypes::None, BWAPI::UnitType reqLv2 = UnitTypes::None, BWAPI::UnitType reqLv3 = UnitTypes::None)
      {
        this->name               = name;
        this->mineralPriceBase   = mineralPriceBase;
        this->mineralPriceFactor = mineralPriceFactor;
        this->gasPriceBase       = gasPriceBase;
        this->gasPriceFactor     = gasPriceFactor;
        this->upgradeTimeBase    = upgradeTimeBase;
        this->upgradeTimeFactor  = upgradeTimeFactor;
        this->whatUpgrades       = whatUpgrades;
        this->race               = race;
        if (whatUses != UnitTypes::None)
          this->whatUses.insert(whatUses);

        this->maxRepeats = maxRepeats;
        this->valid      = true;
          
        this->requirement[0] = reqLv1;
        this->requirement[1] = reqLv2;
        this->requirement[2] = reqLv3;
      }
      std::string name;
      int mineralPriceBase;
      int mineralPriceFactor;
      int gasPriceBase;
      int gasPriceFactor;
      int upgradeTimeBase;
      int upgradeTimeFactor;
      BWAPI::UnitType whatUpgrades;
      Race race;
      int maxRepeats;
      BWAPI::UnitType::set whatUses;
      BWAPI::UnitType requirement[3];
      bool valid;
  };
  UpgradeTypeInternal upgradeTypeData[63];
  std::map<std::string, UpgradeType> upgradeTypeMap;
  namespace UpgradeTypes
  {
    static const UpgradeType::set upgradeTypeSet( Terran_Infantry_Armor | Terran_Vehicle_Plating | Terran_Ship_Plating |
                                                Zerg_Carapace | Zerg_Flyer_Carapace | Protoss_Ground_Armor | Protoss_Air_Armor |
                                                Terran_Infantry_Weapons | Terran_Vehicle_Weapons | Terran_Ship_Weapons |
                                                Zerg_Melee_Attacks | Zerg_Missile_Attacks | Zerg_Flyer_Attacks | 
                                                Protoss_Ground_Weapons | Protoss_Air_Weapons | Protoss_Plasma_Shields | 
                                                U_238_Shells | Ion_Thrusters | Titan_Reactor | Ocular_Implants | 
                                                Moebius_Reactor | Apollo_Reactor | Colossus_Reactor | Ventral_Sacs | 
                                                Antennae | Pneumatized_Carapace | Metabolic_Boost | Adrenal_Glands | 
                                                Muscular_Augments | Grooved_Spines | Gamete_Meiosis | Metasynaptic_Node | 
                                                Singularity_Charge | Leg_Enhancements | Scarab_Damage | Reaver_Capacity | 
                                                Gravitic_Drive | Sensor_Array | Gravitic_Boosters | Khaydarin_Amulet | 
                                                Apial_Sensors | Gravitic_Thrusters | Carrier_Capacity | Khaydarin_Core | 
                                                Argus_Jewel | Argus_Talisman | Caduceus_Reactor | Chitinous_Plating | 
                                                Anabolic_Synthesis | Charon_Boosters | None | Unknown );

    void init()
    {
      upgradeTypeData[Terran_Infantry_Armor].set("Terran Infantry Armor"    , 100, 75 , 100, 75 , 4000, 480, UnitTypes::Terran_Engineering_Bay      , Races::Terran , UnitTypes::None                 , 3, UnitTypes::None, UnitTypes::Terran_Science_Facility, UnitTypes::Terran_Science_Facility);
      upgradeTypeData[Terran_Vehicle_Plating].set("Terran Vehicle Plating"  , 100, 75 , 100, 75 , 4000, 480, UnitTypes::Terran_Armory               , Races::Terran , UnitTypes::None                 , 3, UnitTypes::None, UnitTypes::Terran_Science_Facility, UnitTypes::Terran_Science_Facility);
      upgradeTypeData[Terran_Ship_Plating].set("Terran Ship Plating"        , 150, 75 , 150, 75 , 4000, 480, UnitTypes::Terran_Armory               , Races::Terran , UnitTypes::None                 , 3, UnitTypes::None, UnitTypes::Terran_Science_Facility, UnitTypes::Terran_Science_Facility);
      upgradeTypeData[Zerg_Carapace].set("Zerg Carapace"                    , 150, 75 , 150, 75 , 4000, 480, UnitTypes::Zerg_Evolution_Chamber      , Races::Zerg   , UnitTypes::None                 , 3, UnitTypes::None, UnitTypes::Zerg_Lair, UnitTypes::Zerg_Hive);
      upgradeTypeData[Zerg_Flyer_Carapace].set("Zerg Flyer Carapace"        , 150, 75 , 150, 75 , 4000, 480, UnitTypes::Zerg_Spire                  , Races::Zerg   , UnitTypes::None                 , 3, UnitTypes::None, UnitTypes::Zerg_Lair, UnitTypes::Zerg_Hive);
      upgradeTypeData[Protoss_Ground_Armor].set("Protoss Ground Armor"      , 100, 75 , 100, 75 , 4000, 480, UnitTypes::Protoss_Forge               , Races::Protoss, UnitTypes::None                 , 3, UnitTypes::None, UnitTypes::Protoss_Templar_Archives, UnitTypes::Protoss_Templar_Archives);
      upgradeTypeData[Protoss_Air_Armor].set("Protoss Air Armor"            , 150, 75 , 150, 75 , 4000, 480, UnitTypes::Protoss_Cybernetics_Core    , Races::Protoss, UnitTypes::None                 , 3, UnitTypes::None, UnitTypes::Protoss_Fleet_Beacon, UnitTypes::Protoss_Fleet_Beacon);
      upgradeTypeData[Terran_Infantry_Weapons].set("Terran Infantry Weapons", 100, 75 , 100, 75 , 4000, 480, UnitTypes::Terran_Engineering_Bay      , Races::Terran , UnitTypes::None                 , 3, UnitTypes::None, UnitTypes::Terran_Science_Facility, UnitTypes::Terran_Science_Facility);
      upgradeTypeData[Terran_Vehicle_Weapons].set("Terran Vehicle Weapons"  , 100, 75 , 100, 75 , 4000, 480, UnitTypes::Terran_Armory               , Races::Terran , UnitTypes::None                 , 3, UnitTypes::None, UnitTypes::Terran_Science_Facility, UnitTypes::Terran_Science_Facility);
      upgradeTypeData[Terran_Ship_Weapons].set("Terran Ship Weapons"        , 100, 50 , 100, 50 , 4000, 480, UnitTypes::Terran_Armory               , Races::Terran , UnitTypes::None                 , 3, UnitTypes::None, UnitTypes::Terran_Science_Facility, UnitTypes::Terran_Science_Facility);
      upgradeTypeData[Zerg_Melee_Attacks].set("Zerg Melee Attacks"          , 100, 50 , 100, 50 , 4000, 480, UnitTypes::Zerg_Evolution_Chamber      , Races::Zerg   , UnitTypes::None                 , 3, UnitTypes::None, UnitTypes::Zerg_Lair, UnitTypes::Zerg_Hive);
      upgradeTypeData[Zerg_Missile_Attacks].set("Zerg Missile Attacks"      , 100, 50 , 100, 50 , 4000, 480, UnitTypes::Zerg_Evolution_Chamber      , Races::Zerg   , UnitTypes::None                 , 3, UnitTypes::None, UnitTypes::Zerg_Lair, UnitTypes::Zerg_Hive);
      upgradeTypeData[Zerg_Flyer_Attacks].set("Zerg Flyer Attacks"          , 100, 75 , 100, 75 , 4000, 480, UnitTypes::Zerg_Spire                  , Races::Zerg   , UnitTypes::None                 , 3, UnitTypes::None, UnitTypes::Zerg_Lair, UnitTypes::Zerg_Hive);
      upgradeTypeData[Protoss_Ground_Weapons].set("Protoss Ground Weapons"  , 100, 50 , 100, 50 , 4000, 480, UnitTypes::Protoss_Forge               , Races::Protoss, UnitTypes::None                 , 3, UnitTypes::None, UnitTypes::Protoss_Templar_Archives, UnitTypes::Protoss_Templar_Archives);
      upgradeTypeData[Protoss_Air_Weapons].set("Protoss Air Weapons"        , 100, 75 , 100, 75 , 4000, 480, UnitTypes::Protoss_Cybernetics_Core    , Races::Protoss, UnitTypes::None                 , 3, UnitTypes::None, UnitTypes::Protoss_Fleet_Beacon, UnitTypes::Protoss_Fleet_Beacon);
      upgradeTypeData[Protoss_Plasma_Shields].set("Protoss Plasma Shields"  , 200, 100, 200, 100, 4000, 480, UnitTypes::Protoss_Forge               , Races::Protoss, UnitTypes::None                 , 3, UnitTypes::None, UnitTypes::Protoss_Cybernetics_Core, UnitTypes::Protoss_Cybernetics_Core);
      upgradeTypeData[U_238_Shells].set("U-238 Shells"                      , 150, 0  , 150, 0  , 1500, 0  , UnitTypes::Terran_Academy              , Races::Terran , UnitTypes::Terran_Marine);
      upgradeTypeData[Ion_Thrusters].set("Ion Thrusters"                    , 100, 0  , 100, 0  , 1500, 0  , UnitTypes::Terran_Machine_Shop         , Races::Terran , UnitTypes::Terran_Vulture);
      upgradeTypeData[Titan_Reactor].set("Titan Reactor"                    , 150, 0  , 150, 0  , 2500, 0  , UnitTypes::Terran_Science_Facility     , Races::Terran , UnitTypes::Terran_Science_Vessel);
      upgradeTypeData[Ocular_Implants].set("Ocular Implants"                , 100, 0  , 100, 0  , 2500, 0  , UnitTypes::Terran_Covert_Ops           , Races::Terran , UnitTypes::Terran_Ghost);
      upgradeTypeData[Moebius_Reactor].set("Moebius Reactor"                , 150, 0  , 150, 0  , 2500, 0  , UnitTypes::Terran_Covert_Ops           , Races::Terran , UnitTypes::Terran_Ghost);
      upgradeTypeData[Apollo_Reactor].set("Apollo Reactor"                  , 200, 0  , 200, 0  , 2500, 0  , UnitTypes::Terran_Control_Tower        , Races::Terran , UnitTypes::Terran_Wraith);
      upgradeTypeData[Colossus_Reactor].set("Colossus Reactor"              , 150, 0  , 150, 0  , 2500, 0  , UnitTypes::Terran_Physics_Lab          , Races::Terran , UnitTypes::Terran_Battlecruiser);
      upgradeTypeData[Ventral_Sacs].set("Ventral Sacs"                      , 200, 0  , 200, 0  , 2400, 0  , UnitTypes::Zerg_Lair                   , Races::Zerg   , UnitTypes::Zerg_Overlord);
      upgradeTypeData[Antennae].set("Antennae"                              , 150, 0  , 150, 0  , 2000, 0  , UnitTypes::Zerg_Lair                   , Races::Zerg   , UnitTypes::Zerg_Overlord);
      upgradeTypeData[Pneumatized_Carapace].set("Pneumatized Carapace"      , 150, 0  , 150, 0  , 2000, 0  , UnitTypes::Zerg_Lair                   , Races::Zerg   , UnitTypes::Zerg_Overlord);
      upgradeTypeData[Metabolic_Boost].set("Metabolic Boost"                , 100, 0  , 100, 0  , 1500, 0  , UnitTypes::Zerg_Spawning_Pool          , Races::Zerg   , UnitTypes::Zerg_Zergling);
      upgradeTypeData[Adrenal_Glands].set("Adrenal Glands"                  , 200, 0  , 200, 0  , 1500, 0  , UnitTypes::Zerg_Spawning_Pool          , Races::Zerg   , UnitTypes::Zerg_Zergling);
      upgradeTypeData[Muscular_Augments].set("Muscular Augments"            , 150, 0  , 150, 0  , 1500, 0  , UnitTypes::Zerg_Hydralisk_Den          , Races::Zerg   , UnitTypes::Zerg_Hydralisk);
      upgradeTypeData[Grooved_Spines].set("Grooved Spines"                  , 150, 0  , 150, 0  , 1500, 0  , UnitTypes::Zerg_Hydralisk_Den          , Races::Zerg   , UnitTypes::Zerg_Hydralisk);
      upgradeTypeData[Gamete_Meiosis].set("Gamete Meiosis"                  , 150, 0  , 150, 0  , 2500, 0  , UnitTypes::Zerg_Queens_Nest            , Races::Zerg   , UnitTypes::Zerg_Queen);
      upgradeTypeData[Metasynaptic_Node].set("Metasynaptic Node"            , 150, 0  , 150, 0  , 2500, 0  , UnitTypes::Zerg_Defiler_Mound          , Races::Zerg   , UnitTypes::Zerg_Defiler);
      upgradeTypeData[Singularity_Charge].set("Singularity Charge"          , 150, 0  , 150, 0  , 2500, 0  , UnitTypes::Protoss_Cybernetics_Core    , Races::Protoss, UnitTypes::Protoss_Dragoon);
      upgradeTypeData[Leg_Enhancements].set("Leg Enhancements"              , 150, 0  , 150, 0  , 2000, 0  , UnitTypes::Protoss_Citadel_of_Adun     , Races::Protoss, UnitTypes::Protoss_Zealot);
      upgradeTypeData[Scarab_Damage].set("Scarab Damage"                    , 200, 0  , 200, 0  , 2500, 0  , UnitTypes::Protoss_Robotics_Support_Bay, Races::Protoss, UnitTypes::Protoss_Scarab);
      upgradeTypeData[Reaver_Capacity].set("Reaver Capacity"                , 200, 0  , 200, 0  , 2500, 0  , UnitTypes::Protoss_Robotics_Support_Bay, Races::Protoss, UnitTypes::Protoss_Reaver);
      upgradeTypeData[Gravitic_Drive].set("Gravitic Drive"                  , 200, 0  , 200, 0  , 2500, 0  , UnitTypes::Protoss_Robotics_Support_Bay, Races::Protoss, UnitTypes::Protoss_Shuttle);
      upgradeTypeData[Sensor_Array].set("Sensor Array"                      , 150, 0  , 150, 0  , 2000, 0  , UnitTypes::Protoss_Observatory         , Races::Protoss, UnitTypes::Protoss_Observer);
      upgradeTypeData[Gravitic_Boosters].set("Gravitic Boosters"            , 150, 0  , 150, 0  , 2000, 0  , UnitTypes::Protoss_Observatory         , Races::Protoss, UnitTypes::Protoss_Observer);
      upgradeTypeData[Khaydarin_Amulet].set("Khaydarin Amulet"              , 150, 0  , 150, 0  , 2500, 0  , UnitTypes::Protoss_Templar_Archives    , Races::Protoss, UnitTypes::Protoss_High_Templar);
      upgradeTypeData[Apial_Sensors].set("Apial Sensors"                    , 100, 0  , 100, 0  , 2500, 0  , UnitTypes::Protoss_Fleet_Beacon        , Races::Protoss, UnitTypes::Protoss_Scout);
      upgradeTypeData[Gravitic_Thrusters].set("Gravitic Thrusters"          , 200, 0  , 200, 0  , 2500, 0  , UnitTypes::Protoss_Fleet_Beacon        , Races::Protoss, UnitTypes::Protoss_Scout);
      upgradeTypeData[Carrier_Capacity].set("Carrier Capacity"              , 100, 0  , 100, 0  , 1500, 0  , UnitTypes::Protoss_Fleet_Beacon        , Races::Protoss, UnitTypes::Protoss_Carrier);
      upgradeTypeData[Khaydarin_Core].set("Khaydarin Core"                  , 150, 0  , 150, 0  , 2500, 0  , UnitTypes::Protoss_Arbiter_Tribunal    , Races::Protoss, UnitTypes::Protoss_Arbiter);
      upgradeTypeData[Argus_Jewel].set("Argus Jewel"                        , 100, 0  , 100, 0  , 2500, 0  , UnitTypes::Protoss_Fleet_Beacon        , Races::Protoss, UnitTypes::Protoss_Corsair);
      upgradeTypeData[Argus_Talisman].set("Argus Talisman"                  , 150, 0  , 150, 0  , 2500, 0  , UnitTypes::Protoss_Templar_Archives    , Races::Protoss, UnitTypes::Protoss_Dark_Archon );
      upgradeTypeData[Caduceus_Reactor].set("Caduceus Reactor"              , 150, 0  , 150, 0  , 2500, 0  , UnitTypes::Terran_Academy              , Races::Terran , UnitTypes::Terran_Medic);
      upgradeTypeData[Chitinous_Plating].set("Chitinous Plating"            , 150, 0  , 150, 0  , 2000, 0  , UnitTypes::Zerg_Ultralisk_Cavern       , Races::Zerg   , UnitTypes::Zerg_Ultralisk);
      upgradeTypeData[Anabolic_Synthesis].set("Anabolic Synthesis"          , 200, 0  , 200, 0  , 2000, 0  , UnitTypes::Zerg_Ultralisk_Cavern       , Races::Zerg   , UnitTypes::Zerg_Ultralisk);
      upgradeTypeData[Charon_Boosters].set("Charon Boosters"                , 100, 0  , 100, 0  , 2000, 0  , UnitTypes::Terran_Machine_Shop         , Races::Terran , UnitTypes::Terran_Goliath, 1, UnitTypes::Terran_Armory);
      upgradeTypeData[None].set("None"                                      , 0  , 0  , 0  , 0  , 0   , 0  , UnitTypes::None                        , Races::None   , UnitTypes::None, 0);
      upgradeTypeData[Unknown].set("Unknown"                                , 0  , 0  , 0  , 0  , 0   , 0  , UnitTypes::None                        , Races::Unknown, UnitTypes::None, 0);

      upgradeTypeData[Terran_Infantry_Armor].whatUses.insert(UnitTypes::Terran_Firebat);
      upgradeTypeData[Terran_Infantry_Armor].whatUses.insert(UnitTypes::Terran_Ghost);
      upgradeTypeData[Terran_Infantry_Armor].whatUses.insert(UnitTypes::Terran_Marine);
      upgradeTypeData[Terran_Infantry_Armor].whatUses.insert(UnitTypes::Terran_Medic);
      upgradeTypeData[Terran_Infantry_Armor].whatUses.insert(UnitTypes::Terran_SCV);
      upgradeTypeData[Terran_Vehicle_Plating].whatUses.insert(UnitTypes::Terran_Goliath);
      upgradeTypeData[Terran_Vehicle_Plating].whatUses.insert(UnitTypes::Terran_Siege_Tank_Siege_Mode);
      upgradeTypeData[Terran_Vehicle_Plating].whatUses.insert(UnitTypes::Terran_Siege_Tank_Tank_Mode);
      upgradeTypeData[Terran_Vehicle_Plating].whatUses.insert(UnitTypes::Terran_Vulture);
      upgradeTypeData[Terran_Ship_Plating].whatUses.insert(UnitTypes::Terran_Battlecruiser);
      upgradeTypeData[Terran_Ship_Plating].whatUses.insert(UnitTypes::Terran_Dropship);
      upgradeTypeData[Terran_Ship_Plating].whatUses.insert(UnitTypes::Terran_Science_Vessel);
      upgradeTypeData[Terran_Ship_Plating].whatUses.insert(UnitTypes::Terran_Valkyrie);
      upgradeTypeData[Terran_Ship_Plating].whatUses.insert(UnitTypes::Terran_Wraith);
      upgradeTypeData[Zerg_Carapace].whatUses.insert(UnitTypes::Zerg_Broodling);
      upgradeTypeData[Zerg_Carapace].whatUses.insert(UnitTypes::Zerg_Defiler);
      upgradeTypeData[Zerg_Carapace].whatUses.insert(UnitTypes::Zerg_Drone);
      upgradeTypeData[Zerg_Carapace].whatUses.insert(UnitTypes::Zerg_Hydralisk);
      upgradeTypeData[Zerg_Carapace].whatUses.insert(UnitTypes::Zerg_Infested_Terran);
      upgradeTypeData[Zerg_Carapace].whatUses.insert(UnitTypes::Zerg_Larva);
      upgradeTypeData[Zerg_Carapace].whatUses.insert(UnitTypes::Zerg_Lurker);
      upgradeTypeData[Zerg_Carapace].whatUses.insert(UnitTypes::Zerg_Ultralisk);
      upgradeTypeData[Zerg_Carapace].whatUses.insert(UnitTypes::Zerg_Zergling);
      upgradeTypeData[Zerg_Flyer_Carapace].whatUses.insert(UnitTypes::Zerg_Devourer);
      upgradeTypeData[Zerg_Flyer_Carapace].whatUses.insert(UnitTypes::Zerg_Guardian);
      upgradeTypeData[Zerg_Flyer_Carapace].whatUses.insert(UnitTypes::Zerg_Mutalisk);
      upgradeTypeData[Zerg_Flyer_Carapace].whatUses.insert(UnitTypes::Zerg_Overlord);
      upgradeTypeData[Zerg_Flyer_Carapace].whatUses.insert(UnitTypes::Zerg_Queen);
      upgradeTypeData[Zerg_Flyer_Carapace].whatUses.insert(UnitTypes::Zerg_Scourge);
      upgradeTypeData[Protoss_Ground_Armor].whatUses.insert(UnitTypes::Protoss_Archon);
      upgradeTypeData[Protoss_Ground_Armor].whatUses.insert(UnitTypes::Protoss_Dark_Archon);
      upgradeTypeData[Protoss_Ground_Armor].whatUses.insert(UnitTypes::Protoss_Dark_Templar);
      upgradeTypeData[Protoss_Ground_Armor].whatUses.insert(UnitTypes::Protoss_Dragoon);
      upgradeTypeData[Protoss_Ground_Armor].whatUses.insert(UnitTypes::Protoss_High_Templar);
      upgradeTypeData[Protoss_Ground_Armor].whatUses.insert(UnitTypes::Protoss_Probe);
      upgradeTypeData[Protoss_Ground_Armor].whatUses.insert(UnitTypes::Protoss_Reaver);
      upgradeTypeData[Protoss_Ground_Armor].whatUses.insert(UnitTypes::Protoss_Zealot);
      upgradeTypeData[Protoss_Air_Armor].whatUses.insert(UnitTypes::Protoss_Arbiter);
      upgradeTypeData[Protoss_Air_Armor].whatUses.insert(UnitTypes::Protoss_Carrier);
      upgradeTypeData[Protoss_Air_Armor].whatUses.insert(UnitTypes::Protoss_Corsair);
      upgradeTypeData[Protoss_Air_Armor].whatUses.insert(UnitTypes::Protoss_Interceptor);
      upgradeTypeData[Protoss_Air_Armor].whatUses.insert(UnitTypes::Protoss_Observer);
      upgradeTypeData[Protoss_Air_Armor].whatUses.insert(UnitTypes::Protoss_Scout);
      upgradeTypeData[Protoss_Air_Armor].whatUses.insert(UnitTypes::Protoss_Shuttle);
      upgradeTypeData[Terran_Infantry_Weapons].whatUses.insert(UnitTypes::Terran_Firebat);
      upgradeTypeData[Terran_Infantry_Weapons].whatUses.insert(UnitTypes::Terran_Ghost);
      upgradeTypeData[Terran_Infantry_Weapons].whatUses.insert(UnitTypes::Terran_Marine);
      upgradeTypeData[Terran_Vehicle_Weapons].whatUses.insert(UnitTypes::Terran_Goliath);
      upgradeTypeData[Terran_Vehicle_Weapons].whatUses.insert(UnitTypes::Terran_Siege_Tank_Siege_Mode);
      upgradeTypeData[Terran_Vehicle_Weapons].whatUses.insert(UnitTypes::Terran_Siege_Tank_Tank_Mode);
      upgradeTypeData[Terran_Vehicle_Weapons].whatUses.insert(UnitTypes::Terran_Vulture);
      upgradeTypeData[Terran_Ship_Weapons].whatUses.insert(UnitTypes::Terran_Battlecruiser);
      upgradeTypeData[Terran_Ship_Weapons].whatUses.insert(UnitTypes::Terran_Valkyrie);
      upgradeTypeData[Terran_Ship_Weapons].whatUses.insert(UnitTypes::Terran_Wraith);
      upgradeTypeData[Zerg_Melee_Attacks].whatUses.insert(UnitTypes::Zerg_Broodling);
      upgradeTypeData[Zerg_Melee_Attacks].whatUses.insert(UnitTypes::Zerg_Ultralisk);
      upgradeTypeData[Zerg_Melee_Attacks].whatUses.insert(UnitTypes::Zerg_Zergling);
      upgradeTypeData[Zerg_Missile_Attacks].whatUses.insert(UnitTypes::Zerg_Hydralisk);
      upgradeTypeData[Zerg_Missile_Attacks].whatUses.insert(UnitTypes::Zerg_Lurker);
      upgradeTypeData[Zerg_Flyer_Attacks].whatUses.insert(UnitTypes::Zerg_Devourer);
      upgradeTypeData[Zerg_Flyer_Attacks].whatUses.insert(UnitTypes::Zerg_Guardian);
      upgradeTypeData[Zerg_Flyer_Attacks].whatUses.insert(UnitTypes::Zerg_Mutalisk);
      upgradeTypeData[Protoss_Ground_Weapons].whatUses.insert(UnitTypes::Protoss_Dark_Templar);
      upgradeTypeData[Protoss_Ground_Weapons].whatUses.insert(UnitTypes::Protoss_Dragoon);
      upgradeTypeData[Protoss_Ground_Weapons].whatUses.insert(UnitTypes::Protoss_Zealot);
      upgradeTypeData[Protoss_Air_Weapons].whatUses.insert(UnitTypes::Protoss_Arbiter);
      upgradeTypeData[Protoss_Air_Weapons].whatUses.insert(UnitTypes::Protoss_Corsair);
      upgradeTypeData[Protoss_Air_Weapons].whatUses.insert(UnitTypes::Protoss_Interceptor);
      upgradeTypeData[Protoss_Air_Weapons].whatUses.insert(UnitTypes::Protoss_Scout);
      upgradeTypeData[Protoss_Plasma_Shields].whatUses.insert(UnitTypes::Protoss_Arbiter);
      upgradeTypeData[Protoss_Plasma_Shields].whatUses.insert(UnitTypes::Protoss_Arbiter_Tribunal);
      upgradeTypeData[Protoss_Plasma_Shields].whatUses.insert(UnitTypes::Protoss_Archon);
      upgradeTypeData[Protoss_Plasma_Shields].whatUses.insert(UnitTypes::Protoss_Assimilator);
      upgradeTypeData[Protoss_Plasma_Shields].whatUses.insert(UnitTypes::Protoss_Carrier);
      upgradeTypeData[Protoss_Plasma_Shields].whatUses.insert(UnitTypes::Protoss_Citadel_of_Adun);
      upgradeTypeData[Protoss_Plasma_Shields].whatUses.insert(UnitTypes::Protoss_Corsair);
      upgradeTypeData[Protoss_Plasma_Shields].whatUses.insert(UnitTypes::Protoss_Cybernetics_Core);
      upgradeTypeData[Protoss_Plasma_Shields].whatUses.insert(UnitTypes::Protoss_Dark_Archon);
      upgradeTypeData[Protoss_Plasma_Shields].whatUses.insert(UnitTypes::Protoss_Dark_Templar);
      upgradeTypeData[Protoss_Plasma_Shields].whatUses.insert(UnitTypes::Protoss_Dragoon);
      upgradeTypeData[Protoss_Plasma_Shields].whatUses.insert(UnitTypes::Protoss_Fleet_Beacon);
      upgradeTypeData[Protoss_Plasma_Shields].whatUses.insert(UnitTypes::Protoss_Forge);
      upgradeTypeData[Protoss_Plasma_Shields].whatUses.insert(UnitTypes::Protoss_Gateway);
      upgradeTypeData[Protoss_Plasma_Shields].whatUses.insert(UnitTypes::Protoss_High_Templar);
      upgradeTypeData[Protoss_Plasma_Shields].whatUses.insert(UnitTypes::Protoss_Interceptor);
      upgradeTypeData[Protoss_Plasma_Shields].whatUses.insert(UnitTypes::Protoss_Nexus);
      upgradeTypeData[Protoss_Plasma_Shields].whatUses.insert(UnitTypes::Protoss_Observatory);
      upgradeTypeData[Protoss_Plasma_Shields].whatUses.insert(UnitTypes::Protoss_Observer);
      upgradeTypeData[Protoss_Plasma_Shields].whatUses.insert(UnitTypes::Protoss_Photon_Cannon);
      upgradeTypeData[Protoss_Plasma_Shields].whatUses.insert(UnitTypes::Protoss_Probe);
      upgradeTypeData[Protoss_Plasma_Shields].whatUses.insert(UnitTypes::Protoss_Pylon);
      upgradeTypeData[Protoss_Plasma_Shields].whatUses.insert(UnitTypes::Protoss_Reaver);
      upgradeTypeData[Protoss_Plasma_Shields].whatUses.insert(UnitTypes::Protoss_Robotics_Facility);
      upgradeTypeData[Protoss_Plasma_Shields].whatUses.insert(UnitTypes::Protoss_Robotics_Support_Bay);
      upgradeTypeData[Protoss_Plasma_Shields].whatUses.insert(UnitTypes::Protoss_Scarab);
      upgradeTypeData[Protoss_Plasma_Shields].whatUses.insert(UnitTypes::Protoss_Scout);
      upgradeTypeData[Protoss_Plasma_Shields].whatUses.insert(UnitTypes::Protoss_Shield_Battery);
      upgradeTypeData[Protoss_Plasma_Shields].whatUses.insert(UnitTypes::Protoss_Shuttle);
      upgradeTypeData[Protoss_Plasma_Shields].whatUses.insert(UnitTypes::Protoss_Stargate);
      upgradeTypeData[Protoss_Plasma_Shields].whatUses.insert(UnitTypes::Protoss_Templar_Archives);
      upgradeTypeData[Protoss_Plasma_Shields].whatUses.insert(UnitTypes::Protoss_Zealot);

      foreach(UpgradeType i, upgradeTypeSet)
      {
        std::string name(i.getName());
        fixName(&name);
        upgradeTypeMap.insert(std::make_pair(name, i));
      }
    }
  }
  UpgradeType::UpgradeType(int id) : Type( id )
  {
  }
  const std::string &UpgradeType::getName() const
  {
    return upgradeTypeData[this->getID()].name;
  }
  const char *UpgradeType::c_str() const
  {
    return upgradeTypeData[this->getID()].name.c_str();
  }
  Race UpgradeType::getRace() const
  {
    return upgradeTypeData[this->getID()].race;
  }
  int UpgradeType::mineralPrice(int level) const
  {
    return upgradeTypeData[this->getID()].mineralPriceBase + 
           (level > 0 ? level - 1 : 0) * this->mineralPriceFactor();
  }
  int UpgradeType::mineralPriceFactor() const
  {
    return upgradeTypeData[this->getID()].mineralPriceFactor;
  }
  int UpgradeType::gasPrice(int level) const
  {
    return upgradeTypeData[this->getID()].gasPriceBase + 
           (level > 0 ? level - 1 : 0) * this->gasPriceFactor();
  }
  int UpgradeType::gasPriceFactor() const
  {
    return upgradeTypeData[this->getID()].gasPriceFactor;
  }
  int UpgradeType::upgradeTime(int level) const
  {
    return upgradeTypeData[this->getID()].upgradeTimeBase + (level > 0 ? level - 1 : 0) * this->upgradeTimeFactor();
  }
  int UpgradeType::upgradeTimeFactor() const
  {
    return upgradeTypeData[this->getID()].upgradeTimeFactor;
  }
  UnitType UpgradeType::whatUpgrades() const
  {
    return upgradeTypeData[this->getID()].whatUpgrades;
  }
  const UnitType::set& UpgradeType::whatUses() const
  {
    return upgradeTypeData[this->getID()].whatUses;
  }
  int UpgradeType::maxRepeats() const
  {
    return upgradeTypeData[this->getID()].maxRepeats;
  }
  UnitType UpgradeType::whatsRequired(int level) const
  {
    if ( level >= 1 && level <= 3)
      return upgradeTypeData[this->getID()].requirement[level - 1];
    return UnitTypes::None;
  }
  UpgradeType UpgradeTypes::getUpgradeType(std::string name)
  {
    fixName(&name);
    std::map<std::string, UpgradeType>::iterator i = upgradeTypeMap.find(name);
    if (i == upgradeTypeMap.end())
      return UpgradeTypes::Unknown;
    return (*i).second;
  }
  const UpgradeType::set& UpgradeTypes::allUpgradeTypes()
  {
    return upgradeTypeSet;
  }
}
