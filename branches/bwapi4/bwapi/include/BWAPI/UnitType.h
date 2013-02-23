#pragma once
#include <map>

#include <BWAPI/Position.h>
#include <BWAPI/Type.h>

namespace BWAPI
{
  // Forward Declarations
  class TechType;
  class UpgradeType;
  class WeaponType;
  class UnitSizeType;
  class Race;

  namespace UnitTypes
  {
    /// Enumeration of unit types
    namespace Enum
    {
      enum Enum
      {
        Terran_Marine = 0,
        Terran_Ghost,
        Terran_Vulture,
        Terran_Goliath,
        Terran_Goliath_Turret,
        Terran_Siege_Tank_Tank_Mode,
        Terran_Siege_Tank_Tank_Mode_Turret,
        Terran_SCV,
        Terran_Wraith,
        Terran_Science_Vessel,
        Hero_Gui_Montag,
        Terran_Dropship,
        Terran_Battlecruiser,
        Terran_Vulture_Spider_Mine,
        Terran_Nuclear_Missile,
        Terran_Civilian,
        Hero_Sarah_Kerrigan,
        Hero_Alan_Schezar,
        Hero_Alan_Schezar_Turret,
        Hero_Jim_Raynor_Vulture,
        Hero_Jim_Raynor_Marine,
        Hero_Tom_Kazansky,
        Hero_Magellan,
        Hero_Edmund_Duke_Tank_Mode,
        Hero_Edmund_Duke_Tank_Mode_Turret,
        Hero_Edmund_Duke_Siege_Mode,
        Hero_Edmund_Duke_Siege_Mode_Turret,
        Hero_Arcturus_Mengsk,
        Hero_Hyperion,
        Hero_Norad_II,
        Terran_Siege_Tank_Siege_Mode,
        Terran_Siege_Tank_Siege_Mode_Turret,
        Terran_Firebat,
        Spell_Scanner_Sweep,
        Terran_Medic,
        Zerg_Larva,
        Zerg_Egg,
        Zerg_Zergling,
        Zerg_Hydralisk,
        Zerg_Ultralisk,
        Zerg_Broodling,
        Zerg_Drone,
        Zerg_Overlord,
        Zerg_Mutalisk,
        Zerg_Guardian,
        Zerg_Queen,
        Zerg_Defiler,
        Zerg_Scourge,
        Hero_Torrasque,
        Hero_Matriarch,
        Zerg_Infested_Terran,
        Hero_Infested_Kerrigan,
        Hero_Unclean_One,
        Hero_Hunter_Killer,
        Hero_Devouring_One,
        Hero_Kukulza_Mutalisk,
        Hero_Kukulza_Guardian,
        Hero_Yggdrasill,
        Terran_Valkyrie,
        Zerg_Cocoon,
        Protoss_Corsair,
        Protoss_Dark_Templar,
        Zerg_Devourer,
        Protoss_Dark_Archon,
        Protoss_Probe,
        Protoss_Zealot,
        Protoss_Dragoon,
        Protoss_High_Templar,
        Protoss_Archon,
        Protoss_Shuttle,
        Protoss_Scout,
        Protoss_Arbiter,
        Protoss_Carrier,
        Protoss_Interceptor,
        Hero_Dark_Templar,
        Hero_Zeratul,
        Hero_Tassadar_Zeratul_Archon,
        Hero_Fenix_Zealot,
        Hero_Fenix_Dragoon,
        Hero_Tassadar,
        Hero_Mojo,
        Hero_Warbringer,
        Hero_Gantrithor,
        Protoss_Reaver,
        Protoss_Observer,
        Protoss_Scarab,
        Hero_Danimoth,
        Hero_Aldaris,
        Hero_Artanis,
        Critter_Rhynadon,
        Critter_Bengalaas,
        Special_Cargo_Ship,
        Special_Mercenary_Gunship,
        Critter_Scantid,
        Critter_Kakaru,
        Critter_Ragnasaur,
        Critter_Ursadon,
        Zerg_Lurker_Egg,
        Hero_Raszagal,
        Hero_Samir_Duran,
        Hero_Alexei_Stukov,
        Special_Map_Revealer,
        Hero_Gerard_DuGalle,
        Zerg_Lurker,
        Hero_Infested_Duran,
        Spell_Disruption_Web,
        Terran_Command_Center,
        Terran_Comsat_Station,
        Terran_Nuclear_Silo,
        Terran_Supply_Depot,
        Terran_Refinery,
        Terran_Barracks,
        Terran_Academy,
        Terran_Factory,
        Terran_Starport,
        Terran_Control_Tower,
        Terran_Science_Facility,
        Terran_Covert_Ops,
        Terran_Physics_Lab,
        Unused_Terran1,
        Terran_Machine_Shop,
        Unused_Terran2,
        Terran_Engineering_Bay,
        Terran_Armory,
        Terran_Missile_Turret,
        Terran_Bunker,
        Special_Crashed_Norad_II,
        Special_Ion_Cannon,
        Powerup_Uraj_Crystal,
        Powerup_Khalis_Crystal,
        Zerg_Infested_Command_Center,
        Zerg_Hatchery,
        Zerg_Lair,
        Zerg_Hive,
        Zerg_Nydus_Canal,
        Zerg_Hydralisk_Den,
        Zerg_Defiler_Mound,
        Zerg_Greater_Spire,
        Zerg_Queens_Nest,
        Zerg_Evolution_Chamber,
        Zerg_Ultralisk_Cavern,
        Zerg_Spire,
        Zerg_Spawning_Pool,
        Zerg_Creep_Colony,
        Zerg_Spore_Colony,
        Unused_Zerg1,
        Zerg_Sunken_Colony,
        Special_Overmind_With_Shell,
        Special_Overmind,
        Zerg_Extractor,
        Special_Mature_Chrysalis,
        Special_Cerebrate,
        Special_Cerebrate_Daggoth,
        Unused_Zerg2,
        Protoss_Nexus,
        Protoss_Robotics_Facility,
        Protoss_Pylon,
        Protoss_Assimilator,
        Unused_Protoss1,
        Protoss_Observatory,
        Protoss_Gateway,
        Unused_Protoss2,
        Protoss_Photon_Cannon,
        Protoss_Citadel_of_Adun,
        Protoss_Cybernetics_Core,
        Protoss_Templar_Archives,
        Protoss_Forge,
        Protoss_Stargate,
        Special_Stasis_Cell_Prison,
        Protoss_Fleet_Beacon,
        Protoss_Arbiter_Tribunal,
        Protoss_Robotics_Support_Bay,
        Protoss_Shield_Battery,
        Special_Khaydarin_Crystal_Form,
        Special_Protoss_Temple,
        Special_XelNaga_Temple,
        Resource_Mineral_Field,
        Resource_Mineral_Field_Type_2,
        Resource_Mineral_Field_Type_3,
        Unused_Cave,
        Unused_Cave_In,
        Unused_Cantina,
        Unused_Mining_Platform,
        Unused_Independant_Command_Center,
        Special_Independant_Starport,
        Unused_Independant_Jump_Gate,
        Unused_Ruins,
        Unused_Khaydarin_Crystal_Formation,
        Resource_Vespene_Geyser,
        Special_Warp_Gate,
        Special_Psi_Disrupter,
        Unused_Zerg_Marker,
        Unused_Terran_Marker,
        Unused_Protoss_Marker,
        Special_Zerg_Beacon,
        Special_Terran_Beacon,
        Special_Protoss_Beacon,
        Special_Zerg_Flag_Beacon,
        Special_Terran_Flag_Beacon,
        Special_Protoss_Flag_Beacon,
        Special_Power_Generator,
        Special_Overmind_Cocoon,
        Spell_Dark_Swarm,
        Special_Floor_Missile_Trap,
        Special_Floor_Hatch,
        Special_Upper_Level_Door,
        Special_Right_Upper_Level_Door,
        Special_Pit_Door,
        Special_Right_Pit_Door,
        Special_Floor_Gun_Trap,
        Special_Wall_Missile_Trap,
        Special_Wall_Flame_Trap,
        Special_Right_Wall_Missile_Trap,
        Special_Right_Wall_Flame_Trap,
        Special_Start_Location,
        Powerup_Flag,
        Powerup_Young_Chrysalis,
        Powerup_Psi_Emitter,
        Powerup_Data_Disk,
        Powerup_Khaydarin_Crystal,
        Powerup_Mineral_Cluster_Type_1,
        Powerup_Mineral_Cluster_Type_2,
        Powerup_Protoss_Gas_Orb_Type_1,
        Powerup_Protoss_Gas_Orb_Type_2,
        Powerup_Zerg_Gas_Sac_Type_1,
        Powerup_Zerg_Gas_Sac_Type_2,
        Powerup_Terran_Gas_Tank_Type_1,
        Powerup_Terran_Gas_Tank_Type_2,

        None,
        AllUnits,
        Men,
        Buildings,
        Factories,
        Unknown,
        MAX
      };


    };
  }
  /** The UnitType class is used to get information about a particular type of unit, such as the build time
   * of a Lurker, or the mineral price of an Ultralisk. TODO Add the unittype table from the wiki*/
  class UnitType : public Type<UnitType, UnitTypes::Enum::Unknown>
  {
  public:
    UnitType(int id = UnitTypes::Enum::None);

    /** Returns the race that the unit belongs to. For example UnitTypes::Terran_SCV.getRace() will return
     * Races::Terran. */
    Race getRace() const;

    /** Returns what builds this unit type. The second number will usually be 1 unless the unit type is
     * Protoss_Archon or Protoss_Dark_Archon. Units that cannot be created, such as critters and mineral
     * fields, will return a pair where the unit type is UnitTypes::None, and the second component is 0.
     *
     * Example: UnitTypes::Terran_Marine.whatBuilds() will return an std::pair, where the first component
     * is UnitTypes::Terran_Barracks. */
    const std::pair< UnitType, int > whatBuilds() const;

    /** Returns the units the player is required to have before it can train or build the given unit type.
     *
     * Example: UnitTypes::Terran_Battlecruiser.requiredUnits() will return a map of three keys:
     * UnitTypes::Terran_Starport, UnitTypes::Terran_Control_Tower, and UnitTypes::Terran_Physics_Lab. */
    const std::map< UnitType, int >& requiredUnits() const;

    /** Included in the API for completeness, since the only units that actually needs tech to be trained
     * are the Zerg_Lurker and Zerg_Lurker_Egg. The tech type needed is TechTypes::Lurker_Aspect. */
    TechType requiredTech() const;

    /** Returns the tech used to cloak the unit, or TechTypes::None if the unit cannot cloak or is
        permanently cloaked */
    TechType cloakingTech() const;

    /** Returns the set of tech types this unit can use, provided the tech types have been researched and
     * the unit has enough energy. */
    const ConstVectorset<TechType>& abilities() const;

    /** Returns the set of upgrade types that can affect this unit. */
    const ConstVectorset<UpgradeType>& upgrades() const;

    /** Returns the upgrade that increase's the unit's armor, or UpgradeTypes::None if no upgrade
     * increase's this unit's armor. For example UnitTypes::Terran_Marine.armorUpgrade() will return a
     * pointer to UpgradeTypes::Terran_Infantry_Armor. */
    UpgradeType armorUpgrade() const;

    /** Returns the maximum amount of hit points the unit type can have. */
    int maxHitPoints() const;

    /** Returns the maximum amount of shields the unit type can have. */
    int maxShields() const;

    /** Returns the maximum amount of energy the unit type can have. */
    int maxEnergy() const;

    /** Returns the amount of armor the non-upgraded unit type has. */
    int armor() const;

    /** Returns the mineral price of the unit.
     *
     * Example: UnitTypes::Siege_Tank_Tank_Mode.mineralPrice() returns 150. */
    int mineralPrice() const;

    /** UnitTypes::Siege_Tank_Tank_Mode.gasPrice() returns 100. */
    int gasPrice() const;

    /** Returns the number of frames needed to make this unit type. */
    int buildTime() const;

    /** Returns the amount of supply used by this unit. Supply counts returned by BWAPI are double what you
     *  would expect to see from playing the game. This is because zerglings take up 0.5 in-game supply. */
    int supplyRequired() const;

    /** Returns the amount of supply produced by this unit (i.e. for a Protoss_Pylon). Supply counts
     * returned by BWAPI are double what you would expect to see from playing the game. This is because
     * zerglings take up 0.5 in-game supply. */
    int supplyProvided() const;

    /** Returns the amount of space this unit type takes up inside a bunker or transport unit. */
    int spaceRequired() const;

    /** Returns the amount of space this unit type provides. */
    int spaceProvided() const;

    /** Returns the score which is used to determine the total scores in the after-game stats screen. */
    int buildScore() const;

    /** Returns the score which is used to determine the total scores in the after-game stats screen. */
    int destroyScore() const;

    /** Returns the size of the unit - either Small, Medium, Large, or Independent. */
    UnitSizeType size() const;

    /** Returns the tile width of the unit. Useful for determining the size of buildings. For example
     * UnitTypes::Terran_Supply_Depot.tileWidth() will return 3. */
    int tileWidth() const;

    /** Returns the tile height of the unit. Useful for determining the size of buildings. For example
     * UnitTypes::Terran_Supply_Depot.tileHeight() will return 2. */
    int tileHeight() const;

    TilePosition tileSize() const;

    /** Distance from the center of the unit to the left edge of the unit, measured in pixels. */
    int dimensionLeft() const;

    /** Distance from the center of the unit to the top edge of the unit, measured in pixels. */
    int dimensionUp() const;

    /** Distance from the center of the unit to the right edge of the unit, measured in pixels. */
    int dimensionRight() const;

    /** Distance from the center of the unit to the bottom edge of the unit, measured in pixels. */
    int dimensionDown() const;

    /// A macro for retrieving the width of the unit type, which is calculated using
    /// dimensionLeft + dimensionRight + 1.
    int width() const;

    /// A macro for retrieving the height of the unit type, which is calculated using
    /// dimensionUp + dimensionDown + 1.
    int height() const;

    /** Returns the range at which the unit will start targeting enemy units, measured in pixels. */
    int seekRange() const;

    /** Returns how far the un-upgraded unit type can see into the fog of war, measured in pixels. */
    int sightRange() const;

    /** Returns the unit's ground weapon. */
    WeaponType groundWeapon() const;

    // TODO: add doc
    int maxGroundHits() const;

    /** Returns the unit's air weapon. */
    WeaponType airWeapon() const;

    // TODO: add doc
    int maxAirHits() const;

    /** Returns the unit's non-upgraded top speed in pixels per frame. For Terran buildings that can lift
     * off and the Zerg Infested Command Center, this returns how fast the building moves when it is
     * lifted. */
    double topSpeed() const;

    /** Returns how fast the unit can accelerate to its top speed. What units this quantity is measured in
     * is currently unknown. */
    int acceleration() const;

    /** Related to how fast the unit can halt. What units this quantity is measured in is currently
     * unknown. */
    int haltDistance() const;

    /** Related to how fast the unit can turn. What units this quantity is measured in is currently
     * unknown. */
    int turnRadius() const;

    /** Returns true if the unit can train other units. For example, UnitTypes::Terran_Barracks.canProduce()
     * will return true, while UnitTypes::Terran_Marine?.canProduce() will return false. This is also true
     * for two non-building units: Protoss Carrier (can produce interceptors) and Protoss Reaver
     * (can produce scarabs). */
    bool canProduce() const;

    /** Returns true if the unit can attack (either ground or air). Returns false for units that can only
     * inflict damage via special abilities (such as Protoss High Templar). */
    bool canAttack() const;

    /** Returns true if the unit can move. Note that buildings will return false, even Terran buildings
     * which can move once lifted. */
    bool canMove() const;

    /** Returns true for flying/air units. */
    bool isFlyer() const;

    /** Returns true for units that regenerate health (i.e. zerg units). */
    bool regeneratesHP() const;

    /** Returns true if the unit type is capable of casting spells / using technology. */
    bool isSpellcaster() const;

    /** Returns true for the two units that are permanently cloaked - Protoss Observer and Protoss Dark
     * Templar. */
    bool hasPermanentCloak() const;

    /** Returns true for units that cannot be destroyed (i.e. Terran Nuclear Missile, Mineral Field,
     * Vespene Geyser, etc) */
    bool isInvincible() const;

    /** Returns true if the unit is organic, such as a Terran Marine. */
    bool isOrganic() const;

    /** Returns true if the unit is mechanical such as a Terran Vulture. */
    bool isMechanical() const;

    /** Returns true for the four robotic Protoss units - Probe, Shuttle, Reaver, and Observer. */
    bool isRobotic() const;

    /** Returns true for the seven units that can detect cloaked units - Terran Science Vessel, Spell
     * Scanner Sweep, Zerg Overlord, Protoss Observer, Terran Missile Turret, Zerg Spore Colony, and Protoss
     * Photon Cannon. */
    bool isDetector() const;

    /** Returns true for the five units that hold resources - Mineral Field, Vespene Geyser,
     * Terran Refinery, Zerg Extractor, and Protoss Assimilator. */
    bool isResourceContainer() const;

    /** Returns true for the five units that can accept resources - Terran Command Center, Protoss Nexus,
     * Zerg Hatchery, Zerg Lair, and Zerg Hive. */
    bool isResourceDepot() const;

    /** Returns true for Terran Refinery, Zerg Extractor, and Protoss Assimilator. */
    bool isRefinery() const;

    /** Returns true for Protoss Probe, Terran SCV, and Zerg Drone. */
    bool isWorker() const;

    /** Returns true for buildings that must be near a pylon to be constructed. */
    bool requiresPsi() const;

    /** Returns true for buildings that can only be built on zerg creep. */
    bool requiresCreep() const;

    /** Returns true for Zergling and Scourge. */
    bool isTwoUnitsInOneEgg() const;

    /** Returns true for Zerg Lurker and units that can burrow when burrow tech is researched. */
    bool isBurrowable() const;

    /** Returns true for units that can be cloaked - Terran Ghost and Terran Wraith. Does not include units
     * which have permanent cloak (Protoss Observer and Protoss Dark Templar). */
    bool isCloakable() const;

    /** Returns true if the unit is a building (also true for mineral field and vespene geyser). */
    bool isBuilding() const;

    /** Returns true if the unit is an add-on, such as a Terran Comsat Station. */
    bool isAddon() const;

    /** Returns true for Terran buildings that can lift off (i.e. Barracks). */
    bool isFlyingBuilding() const;

    /** Returns true if the unit is neutral, such as a critter or mineral field. */
    bool isNeutral() const;

    /** Returns true if the unit is a Hero unit. */
    bool isHero() const;

    /** Returns true if the unit is a Powerup unit. */
    bool isPowerup() const;

    /** Returns true if the unit is a regular Beacon. */
    bool isBeacon() const;

    /** Returns true if the unit is a flag Beacon. */
    bool isFlagBeacon() const;

    /** Returns true if the unit is a special building. */
    bool isSpecialBuilding() const;

    /** Returns true if the unit is a spell unit. */
    bool isSpell() const;

    /** Returns true if the unit produces larva. */
    bool producesLarva() const;

    /** Returns true if the unit is one of the three mineral field types. */
    bool isMineralField() const;

    /// Returns true if this unit type is a neutral critter.
    ///
    /// @code
    ///   BWAPI::Position myBasePosition( BWAPI::Broodwar->self()->getStartLocation() );
    ///   BWAPI::UnitSet unitsAroundTheBase = BWAPI::Broodwar->getUnitsInRadius(myBasePosition, 1024, GetPlayer != BWAPI::Broodwar->self());
    ///   for ( auto i = unitsAroundTheBase.begin(); i != unitsAroundTheBase.end(); ++i )
    ///   {
    ///     if ( i->getType().isCritter() && !i->isInvincible() )
    ///     {
    ///       BWAPI::Unit *myQueen = i->getClosestUnit(GetType == BWAPI::UnitTypes::Zerg_Queen && GetPlayer == BWAPI::Broodwar->self());
    ///       if ( myQueen )
    ///         myQueen->useTech(BWAPI::TechTypes::Spawn_Broodlings, *i);
    ///     }
    ///   }
    /// @endcode
    bool isCritter() const;

    /// Returns true if the unit is capable of constructing an add-on. An add-on is an extension
    /// or attachment for @Terran structures, specifically the @Command_Center, @Factory,
    /// @Starport, and @Science_Facility.
    ///
    /// @see isAddon
    bool canBuildAddon() const;
  };

  /// Namespace containing unit types
  namespace UnitTypes
  {
    /// Retrieves the maximum unit width from the set of all units.
    int maxUnitWidth();
    
    /// Retrieves the maximum unit height from the set of all units.
    int maxUnitHeight();

    /** Returns the set of all the UnitTypes. */
    const UnitType::const_set& allUnitTypes();

    /** Returns the set of all the MacroTypes. */
    const UnitType::const_set& allMacroTypes();

#ifdef BWAPI_DECL
#undef BWAPI_DECL
#endif
#define BWAPI_DECL(x) /** x */ extern const UnitType x
    BWAPI_DECL(Terran_Marine);
    BWAPI_DECL(Terran_Ghost);
    BWAPI_DECL(Terran_Vulture);
    BWAPI_DECL(Terran_Goliath);
    // goliath turret 4
    BWAPI_DECL(Terran_Siege_Tank_Tank_Mode);
    // siege tank turret 6
    BWAPI_DECL(Terran_SCV);
    BWAPI_DECL(Terran_Wraith);
    BWAPI_DECL(Terran_Science_Vessel);
    BWAPI_DECL(Hero_Gui_Montag);
    BWAPI_DECL(Terran_Dropship);
    BWAPI_DECL(Terran_Battlecruiser);
    BWAPI_DECL(Terran_Vulture_Spider_Mine);
    BWAPI_DECL(Terran_Nuclear_Missile);
    BWAPI_DECL(Terran_Civilian);
    BWAPI_DECL(Hero_Sarah_Kerrigan);
    BWAPI_DECL(Hero_Alan_Schezar);
    // alan turret 18
    BWAPI_DECL(Hero_Jim_Raynor_Vulture);
    BWAPI_DECL(Hero_Jim_Raynor_Marine);
    BWAPI_DECL(Hero_Tom_Kazansky);
    BWAPI_DECL(Hero_Magellan);
    BWAPI_DECL(Hero_Edmund_Duke_Tank_Mode);
    // edmund duke turret 24
    BWAPI_DECL(Hero_Edmund_Duke_Siege_Mode);
    // edmund duke turret siege mode 26
    BWAPI_DECL(Hero_Arcturus_Mengsk);
    BWAPI_DECL(Hero_Hyperion);
    BWAPI_DECL(Hero_Norad_II);
    BWAPI_DECL(Terran_Siege_Tank_Siege_Mode);
    // siege tank siege mode turret 31
    BWAPI_DECL(Terran_Firebat);
    BWAPI_DECL(Spell_Scanner_Sweep);
    BWAPI_DECL(Terran_Medic);
    BWAPI_DECL(Zerg_Larva);
    BWAPI_DECL(Zerg_Egg);
    BWAPI_DECL(Zerg_Zergling);
    BWAPI_DECL(Zerg_Hydralisk);
    BWAPI_DECL(Zerg_Ultralisk);
    BWAPI_DECL(Zerg_Broodling);
    BWAPI_DECL(Zerg_Drone);
    BWAPI_DECL(Zerg_Overlord);
    BWAPI_DECL(Zerg_Mutalisk);
    BWAPI_DECL(Zerg_Guardian);
    BWAPI_DECL(Zerg_Queen);
    BWAPI_DECL(Zerg_Defiler);
    BWAPI_DECL(Zerg_Scourge);
    BWAPI_DECL(Hero_Torrasque);
    BWAPI_DECL(Hero_Matriarch);
    BWAPI_DECL(Zerg_Infested_Terran);
    BWAPI_DECL(Hero_Infested_Kerrigan);
    BWAPI_DECL(Hero_Unclean_One);
    BWAPI_DECL(Hero_Hunter_Killer);
    BWAPI_DECL(Hero_Devouring_One);
    BWAPI_DECL(Hero_Kukulza_Mutalisk);
    BWAPI_DECL(Hero_Kukulza_Guardian);
    BWAPI_DECL(Hero_Yggdrasill);
    BWAPI_DECL(Terran_Valkyrie);
    BWAPI_DECL(Zerg_Cocoon);
    BWAPI_DECL(Protoss_Corsair);
    BWAPI_DECL(Protoss_Dark_Templar);
    BWAPI_DECL(Zerg_Devourer);
    BWAPI_DECL(Protoss_Dark_Archon);
    BWAPI_DECL(Protoss_Probe);
    BWAPI_DECL(Protoss_Zealot);
    BWAPI_DECL(Protoss_Dragoon);
    BWAPI_DECL(Protoss_High_Templar);
    BWAPI_DECL(Protoss_Archon);
    BWAPI_DECL(Protoss_Shuttle);
    BWAPI_DECL(Protoss_Scout);
    BWAPI_DECL(Protoss_Arbiter);
    BWAPI_DECL(Protoss_Carrier);
    BWAPI_DECL(Protoss_Interceptor);
    BWAPI_DECL(Hero_Dark_Templar);
    BWAPI_DECL(Hero_Zeratul);
    BWAPI_DECL(Hero_Tassadar_Zeratul_Archon);
    BWAPI_DECL(Hero_Fenix_Zealot);
    BWAPI_DECL(Hero_Fenix_Dragoon);
    BWAPI_DECL(Hero_Tassadar);
    BWAPI_DECL(Hero_Mojo);
    BWAPI_DECL(Hero_Warbringer);
    BWAPI_DECL(Hero_Gantrithor);
    BWAPI_DECL(Protoss_Reaver);
    BWAPI_DECL(Protoss_Observer);
    BWAPI_DECL(Protoss_Scarab);
    BWAPI_DECL(Hero_Danimoth);
    BWAPI_DECL(Hero_Aldaris);
    BWAPI_DECL(Hero_Artanis);
    BWAPI_DECL(Critter_Rhynadon);
    BWAPI_DECL(Critter_Bengalaas);
    BWAPI_DECL(Special_Cargo_Ship);
    BWAPI_DECL(Special_Mercenary_Gunship);
    BWAPI_DECL(Critter_Scantid);
    BWAPI_DECL(Critter_Kakaru);
    BWAPI_DECL(Critter_Ragnasaur);
    BWAPI_DECL(Critter_Ursadon);
    BWAPI_DECL(Zerg_Lurker_Egg);
    BWAPI_DECL(Hero_Raszagal);
    BWAPI_DECL(Hero_Samir_Duran);
    BWAPI_DECL(Hero_Alexei_Stukov);
    BWAPI_DECL(Special_Map_Revealer);
    BWAPI_DECL(Hero_Gerard_DuGalle);
    BWAPI_DECL(Zerg_Lurker);
    BWAPI_DECL(Hero_Infested_Duran);
    BWAPI_DECL(Spell_Disruption_Web);
    BWAPI_DECL(Terran_Command_Center);
    BWAPI_DECL(Terran_Comsat_Station);
    BWAPI_DECL(Terran_Nuclear_Silo);
    BWAPI_DECL(Terran_Supply_Depot);
    BWAPI_DECL(Terran_Refinery);
    BWAPI_DECL(Terran_Barracks);
    BWAPI_DECL(Terran_Academy);
    BWAPI_DECL(Terran_Factory);
    BWAPI_DECL(Terran_Starport);
    BWAPI_DECL(Terran_Control_Tower);
    BWAPI_DECL(Terran_Science_Facility);
    BWAPI_DECL(Terran_Covert_Ops);
    BWAPI_DECL(Terran_Physics_Lab);
    // starbase 119
    BWAPI_DECL(Terran_Machine_Shop);
    // repair bay 121
    BWAPI_DECL(Terran_Engineering_Bay);
    BWAPI_DECL(Terran_Armory);
    BWAPI_DECL(Terran_Missile_Turret);
    BWAPI_DECL(Terran_Bunker);
    BWAPI_DECL(Special_Crashed_Norad_II);
    BWAPI_DECL(Special_Ion_Cannon);
    BWAPI_DECL(Powerup_Uraj_Crystal);
    BWAPI_DECL(Powerup_Khalis_Crystal);
    BWAPI_DECL(Zerg_Infested_Command_Center);
    BWAPI_DECL(Zerg_Hatchery);
    BWAPI_DECL(Zerg_Lair);
    BWAPI_DECL(Zerg_Hive);
    BWAPI_DECL(Zerg_Nydus_Canal);
    BWAPI_DECL(Zerg_Hydralisk_Den);
    BWAPI_DECL(Zerg_Defiler_Mound);
    BWAPI_DECL(Zerg_Greater_Spire);
    BWAPI_DECL(Zerg_Queens_Nest);
    BWAPI_DECL(Zerg_Evolution_Chamber);
    BWAPI_DECL(Zerg_Ultralisk_Cavern);
    BWAPI_DECL(Zerg_Spire);
    BWAPI_DECL(Zerg_Spawning_Pool);
    BWAPI_DECL(Zerg_Creep_Colony);
    BWAPI_DECL(Zerg_Spore_Colony);
    // unused zerg 1 145
    BWAPI_DECL(Zerg_Sunken_Colony);
    BWAPI_DECL(Special_Overmind_With_Shell);
    BWAPI_DECL(Special_Overmind);
    BWAPI_DECL(Zerg_Extractor);
    BWAPI_DECL(Special_Mature_Chrysalis);
    BWAPI_DECL(Special_Cerebrate);
    BWAPI_DECL(Special_Cerebrate_Daggoth);
    // unused zerg 2 153
    BWAPI_DECL(Protoss_Nexus);
    BWAPI_DECL(Protoss_Robotics_Facility);
    BWAPI_DECL(Protoss_Pylon);
    BWAPI_DECL(Protoss_Assimilator);
    // unused protoss 1 158
    BWAPI_DECL(Protoss_Observatory);
    BWAPI_DECL(Protoss_Gateway);
    // unused protoss 2 161
    BWAPI_DECL(Protoss_Photon_Cannon);
    BWAPI_DECL(Protoss_Citadel_of_Adun);
    BWAPI_DECL(Protoss_Cybernetics_Core);
    BWAPI_DECL(Protoss_Templar_Archives);
    BWAPI_DECL(Protoss_Forge);
    BWAPI_DECL(Protoss_Stargate);
    BWAPI_DECL(Special_Stasis_Cell_Prison);
    BWAPI_DECL(Protoss_Fleet_Beacon);
    BWAPI_DECL(Protoss_Arbiter_Tribunal);
    BWAPI_DECL(Protoss_Robotics_Support_Bay);
    BWAPI_DECL(Protoss_Shield_Battery);
    BWAPI_DECL(Special_Khaydarin_Crystal_Form);
    BWAPI_DECL(Special_Protoss_Temple);
    BWAPI_DECL(Special_XelNaga_Temple);
    BWAPI_DECL(Resource_Mineral_Field);
    BWAPI_DECL(Resource_Mineral_Field_Type_2);
    BWAPI_DECL(Resource_Mineral_Field_Type_3);
    // cave 179
    // cave-in 180
    // cantina 181
    // mining platform 182
    // independant command center 183
    BWAPI_DECL(Special_Independant_Starport);
    // independant jump gate 185
    // ruins 186
    // unused khaydarin crystal formation 187
    BWAPI_DECL(Resource_Vespene_Geyser);
    BWAPI_DECL(Special_Warp_Gate);
    BWAPI_DECL(Special_Psi_Disrupter);
    // zerg marker 191
    // terran marker 192
    // protoss marker 193
    BWAPI_DECL(Special_Zerg_Beacon);
    BWAPI_DECL(Special_Terran_Beacon);
    BWAPI_DECL(Special_Protoss_Beacon);
    BWAPI_DECL(Special_Zerg_Flag_Beacon);
    BWAPI_DECL(Special_Terran_Flag_Beacon);
    BWAPI_DECL(Special_Protoss_Flag_Beacon);
    BWAPI_DECL(Special_Power_Generator);
    BWAPI_DECL(Special_Overmind_Cocoon);
    BWAPI_DECL(Spell_Dark_Swarm);
    BWAPI_DECL(Special_Floor_Missile_Trap);
    BWAPI_DECL(Special_Floor_Hatch);
    BWAPI_DECL(Special_Upper_Level_Door);
    BWAPI_DECL(Special_Right_Upper_Level_Door);
    BWAPI_DECL(Special_Pit_Door);
    BWAPI_DECL(Special_Right_Pit_Door);
    BWAPI_DECL(Special_Floor_Gun_Trap);
    BWAPI_DECL(Special_Wall_Missile_Trap);
    BWAPI_DECL(Special_Wall_Flame_Trap);
    BWAPI_DECL(Special_Right_Wall_Missile_Trap);
    BWAPI_DECL(Special_Right_Wall_Flame_Trap);
    BWAPI_DECL(Special_Start_Location);
    BWAPI_DECL(Powerup_Flag);
    BWAPI_DECL(Powerup_Young_Chrysalis);
    BWAPI_DECL(Powerup_Psi_Emitter);
    BWAPI_DECL(Powerup_Data_Disk);
    BWAPI_DECL(Powerup_Khaydarin_Crystal);
    BWAPI_DECL(Powerup_Mineral_Cluster_Type_1);
    BWAPI_DECL(Powerup_Mineral_Cluster_Type_2);
    BWAPI_DECL(Powerup_Protoss_Gas_Orb_Type_1);
    BWAPI_DECL(Powerup_Protoss_Gas_Orb_Type_2);
    BWAPI_DECL(Powerup_Zerg_Gas_Sac_Type_1);
    BWAPI_DECL(Powerup_Zerg_Gas_Sac_Type_2);
    BWAPI_DECL(Powerup_Terran_Gas_Tank_Type_1);
    BWAPI_DECL(Powerup_Terran_Gas_Tank_Type_2);

    BWAPI_DECL(None);
    BWAPI_DECL(AllUnits);
    BWAPI_DECL(Men);
    BWAPI_DECL(Buildings);
    BWAPI_DECL(Factories);
    BWAPI_DECL(Unknown);
#undef BWAPI_DECL
  }
}
