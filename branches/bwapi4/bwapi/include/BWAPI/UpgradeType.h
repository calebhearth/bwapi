#pragma once
#include "Type.h"
#include <BWAPI/UnitType.h>

#define BWAPI_UPGTYPEDEF(x) static const UpgradeType x(Enum::x) /** ref x */

namespace BWAPI
{
  class Race;

  namespace UpgradeTypes
  {
    namespace Enum
    {
      enum Enum
      {
        Terran_Infantry_Armor   = 0,
        Terran_Vehicle_Plating  = 1,
        Terran_Ship_Plating     = 2,
        Zerg_Carapace           = 3,
        Zerg_Flyer_Carapace     = 4,
        Protoss_Ground_Armor    = 5,
        Protoss_Air_Armor       = 6,
        Terran_Infantry_Weapons = 7,
        Terran_Vehicle_Weapons  = 8,
        Terran_Ship_Weapons     = 9,
        Zerg_Melee_Attacks      = 10,
        Zerg_Missile_Attacks    = 11,
        Zerg_Flyer_Attacks      = 12,
        Protoss_Ground_Weapons  = 13,
        Protoss_Air_Weapons     = 14,
        Protoss_Plasma_Shields  = 15,
        U_238_Shells            = 16,
        Ion_Thrusters           = 17,
    
        Titan_Reactor           = 19,
        Ocular_Implants         = 20,
        Moebius_Reactor         = 21,
        Apollo_Reactor          = 22,
        Colossus_Reactor        = 23,
        Ventral_Sacs            = 24,
        Antennae                = 25,
        Pneumatized_Carapace    = 26,
        Metabolic_Boost         = 27,
        Adrenal_Glands          = 28,
        Muscular_Augments       = 29,
        Grooved_Spines          = 30,
        Gamete_Meiosis          = 31,
        Metasynaptic_Node       = 32,
        Singularity_Charge      = 33,
        Leg_Enhancements        = 34,
        Scarab_Damage           = 35,
        Reaver_Capacity         = 36,
        Gravitic_Drive          = 37,
        Sensor_Array            = 38,
        Gravitic_Boosters       = 39,
        Khaydarin_Amulet        = 40,
        Apial_Sensors           = 41,
        Gravitic_Thrusters      = 42,
        Carrier_Capacity        = 43,
        Khaydarin_Core          = 44,
    
        Argus_Jewel             = 47,
    
        Argus_Talisman          = 49,
    
        Caduceus_Reactor        = 51,
        Chitinous_Plating       = 52,
        Anabolic_Synthesis      = 53,
        Charon_Boosters         = 54,
    
        None                    = 61,
        Unknown,
        MAX
      };
    }
  }
  class UpgradeType : public Type<UpgradeType, UpgradeTypes::Enum::Unknown>
  {
  public:
    UpgradeType(int id = UpgradeTypes::Enum::None);

    /** Returns the name for the upgrade type. */
    const std::string &getName() const;
    const char *c_str() const;

    /** Returns the race the upgrade is for. For example, UpgradeTypes::Terran_Infantry_Armor.getRace()
     * will return Races::Terran. */
    Race getRace() const;

    /** Returns the mineral price for the first upgrade. */
    int mineralPrice(int level = 1) const;

    /** Returns the amount that the mineral price increases for each additional upgrade. */
    int mineralPriceFactor() const;

    /** Returns the vespene gas price for the first upgrade. */
    int gasPrice(int level = 1) const;

    /** Returns the amount that the vespene gas price increases for each additional upgrade. */
    int gasPriceFactor() const;

    /** Returns the number of frames needed to research the first upgrade. */
    int upgradeTime(int level = 1) const;

    /** Returns the number of frames that the upgrade time increases for each additional upgrade. */
    int upgradeTimeFactor() const;

    /** Returns the maximum number of times the upgrade can be researched. */
    int maxRepeats() const;

    /** Returns the type of unit that researches the upgrade. */
    UnitType whatUpgrades() const;

    /** Returns the type of unit that is additionally required for the upgrade. */
    UnitType whatsRequired(int level = 1) const;

    /** Returns the set of units that are affected by this upgrade. */
    const UnitType::set& whatUses() const;
  };
  namespace UpgradeTypes
  {
    /** Given a string, this will return the upgrade type. */
    UpgradeType getUpgradeType(std::string name);

    /** Returns the set of all the UpgradeTypes. */
    const UpgradeType::set& allUpgradeTypes();

    void init();

    BWAPI_UPGTYPEDEF(Terran_Infantry_Armor);
    BWAPI_UPGTYPEDEF(Terran_Vehicle_Plating);
    BWAPI_UPGTYPEDEF(Terran_Ship_Plating);
    BWAPI_UPGTYPEDEF(Zerg_Carapace);
    BWAPI_UPGTYPEDEF(Zerg_Flyer_Carapace);
    BWAPI_UPGTYPEDEF(Protoss_Ground_Armor);
    BWAPI_UPGTYPEDEF(Protoss_Air_Armor);
    BWAPI_UPGTYPEDEF(Terran_Infantry_Weapons);
    BWAPI_UPGTYPEDEF(Terran_Vehicle_Weapons);
    BWAPI_UPGTYPEDEF(Terran_Ship_Weapons);
    BWAPI_UPGTYPEDEF(Zerg_Melee_Attacks);
    BWAPI_UPGTYPEDEF(Zerg_Missile_Attacks);
    BWAPI_UPGTYPEDEF(Zerg_Flyer_Attacks);
    BWAPI_UPGTYPEDEF(Protoss_Ground_Weapons);
    BWAPI_UPGTYPEDEF(Protoss_Air_Weapons);
    BWAPI_UPGTYPEDEF(Protoss_Plasma_Shields);
    BWAPI_UPGTYPEDEF(U_238_Shells);
    BWAPI_UPGTYPEDEF(Ion_Thrusters);
    BWAPI_UPGTYPEDEF(Titan_Reactor);
    BWAPI_UPGTYPEDEF(Ocular_Implants);
    BWAPI_UPGTYPEDEF(Moebius_Reactor);
    BWAPI_UPGTYPEDEF(Apollo_Reactor);
    BWAPI_UPGTYPEDEF(Colossus_Reactor);
    BWAPI_UPGTYPEDEF(Ventral_Sacs);
    BWAPI_UPGTYPEDEF(Antennae);
    BWAPI_UPGTYPEDEF(Pneumatized_Carapace);
    BWAPI_UPGTYPEDEF(Metabolic_Boost);
    BWAPI_UPGTYPEDEF(Adrenal_Glands);
    BWAPI_UPGTYPEDEF(Muscular_Augments);
    BWAPI_UPGTYPEDEF(Grooved_Spines);
    BWAPI_UPGTYPEDEF(Gamete_Meiosis);
    BWAPI_UPGTYPEDEF(Metasynaptic_Node);
    BWAPI_UPGTYPEDEF(Singularity_Charge);
    BWAPI_UPGTYPEDEF(Leg_Enhancements);
    BWAPI_UPGTYPEDEF(Scarab_Damage);
    BWAPI_UPGTYPEDEF(Reaver_Capacity);
    BWAPI_UPGTYPEDEF(Gravitic_Drive);
    BWAPI_UPGTYPEDEF(Sensor_Array);
    BWAPI_UPGTYPEDEF(Gravitic_Boosters);
    BWAPI_UPGTYPEDEF(Khaydarin_Amulet);
    BWAPI_UPGTYPEDEF(Apial_Sensors);
    BWAPI_UPGTYPEDEF(Gravitic_Thrusters);
    BWAPI_UPGTYPEDEF(Carrier_Capacity);
    BWAPI_UPGTYPEDEF(Khaydarin_Core);
    BWAPI_UPGTYPEDEF(Argus_Jewel);
    BWAPI_UPGTYPEDEF(Argus_Talisman);
    BWAPI_UPGTYPEDEF(Caduceus_Reactor);
    BWAPI_UPGTYPEDEF(Chitinous_Plating);
    BWAPI_UPGTYPEDEF(Anabolic_Synthesis);
    BWAPI_UPGTYPEDEF(Charon_Boosters);
    BWAPI_UPGTYPEDEF(None);
    BWAPI_UPGTYPEDEF(Unknown);
  }
}
