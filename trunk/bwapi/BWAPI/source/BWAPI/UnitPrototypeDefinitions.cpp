#include "UnitPrototypeDefinitions.h"
#include "RaceTypes.h"
#include "AbilityFlags.h"

#include "..//BW//UnitTypes.h"

namespace BWAPI
{
  namespace Prototypes
  {
    /*                c++ ability name                       ID Name               mana   minerals gas */
    AbilityPrototype* Move            = new AbilityPrototype( 0, "Move",                0,         0,  0, TargetType::TARGET_LOCATION | 
                                                                                                          TargetType::TARGET_OPPONENTS_UNITS | 
                                                                                                          TargetType::TARGET_MY_UNITS |
                                                                                                          TargetType::TARGET_BIOLOGICAL | 
                                                                                                          TargetType::TARGET_MECHANICAL | 
                                                                                                          TargetType::TARGET_ROBOTIC);
    AbilityPrototype* Attack          = new AbilityPrototype( 1, "Attack",              0,         0,  0, TargetType::TARGET_OPPONENTS_UNITS | 
                                                                                                          TargetType::TARGET_MY_UNITS |
                                                                                                          TargetType::TARGET_BIOLOGICAL | 
                                                                                                          TargetType::TARGET_MECHANICAL | 
                                                                                                          TargetType::TARGET_ROBOTIC);
    AbilityPrototype* AttackMove      = new AbilityPrototype( 2, "Attack move",         0,        0,   0, TargetType::TARGET_LOCATION);
    AbilityPrototype* Ensare          = new AbilityPrototype( 3, "Ensare",             75,        0,   0, TargetType::TARGET_LOCATION |
                                                                                                          TargetType::TARGET_OPPONENTS_UNITS |
                                                                                                          TargetType::TARGET_MY_UNITS |
                                                                                                          TargetType::TARGET_BIOLOGICAL | 
                                                                                                          TargetType::TARGET_MECHANICAL | 
                                                                                                          TargetType::TARGET_ROBOTIC);
    AbilityPrototype* Broodling       = new AbilityPrototype( 4, "Broodling",         150,        0,   0, TargetType::TARGET_OPPONENTS_UNITS | 
                                                                                                          TargetType::TARGET_MY_UNITS | 
                                                                                                          TargetType::TARGET_BIOLOGICAL |
                                                                                                          TargetType::TARGET_MECHANICAL); 
    AbilityPrototype* WarpArchon      = new AbilityPrototype( 5, "Warp archon",         0,        0,   0, TargetType::TARGET_MY_UNITS | 
                                                                                                          TargetType::TARGET_BIOLOGICAL |
                                                                                                          TargetType::TARGET_TEMPLAR);
    AbilityPrototype* WarpDarkArchon  = new AbilityPrototype( 6, "Warp archon",         0,        0,   0, TargetType::TARGET_MY_UNITS | 
                                                                                                          TargetType::TARGET_BIOLOGICAL |
                                                                                                          TargetType::TARGET_TEMPLAR);
    AbilityPrototype* SiegeTank       = new AbilityPrototype( 7, "Siege tank",          0,        0,   0, TargetType::TARGET_SELF);
    AbilityPrototype* UnsiegeTank     = new AbilityPrototype( 8, "Unsiege tank",        0,        0,   0, TargetType::TARGET_SELF);
    AbilityPrototype* Gather          = new AbilityPrototype( 9, "Gather",              0,        0,   0, TargetType::TARGET_RESOURCE |
                                                                                                          TargetType::TARGET_OPPONENTS_UNITS);
    AbilityPrototype* MorphToLurker   = new AbilityPrototype(10, "Morph to lurker",     0,       50, 100, TargetType::TARGET_SELF);
    AbilityPrototype* MorphToGuardian = new AbilityPrototype(11, "Morph to guardian",   0,       50, 100, TargetType::TARGET_SELF);
    AbilityPrototype* Stims           = new AbilityPrototype(11, "Use stimpacks",       0,        0,   0, TargetType::TARGET_SELF);
    
    s32 MOVABLE_CAN_ATTACK = AbilityFlags::Move | AbilityFlags::AttackMove | AbilityFlags::Attack;
    /*                                                Unit name                 ID                                    Race                hp shield  sp  target flags*/
    /** Terran */
    UnitPrototype* SCV            = new UnitPrototype("Terran SCV"            , BW::UnitType::Terran_SCV,             RaceType::Terran,    0,   0, MOVABLE_CAN_ATTACK);
    UnitPrototype* Marine         = new UnitPrototype("Terran Marine"         , BW::UnitType::Terran_Marine,          RaceType::Terran,    0,   0, MOVABLE_CAN_ATTACK | AbilityFlags::Stims);
    UnitPrototype* CommandCenter  = new UnitPrototype("Terran Command Center" , BW::UnitType::Terran_CommandCenter,   RaceType::Terran,    0,   0, 0);
    UnitPrototype* SupplyDepot    = new UnitPrototype("Terran Supply depot"   , BW::UnitType::Terran_SupplyDepot,     RaceType::Terran,    0,   0, 0);
    UnitPrototype* Barracks       = new UnitPrototype("Terran Barracks"       , BW::UnitType::Terran_Barracks,        RaceType::Terran,    0,   0, 0);
    UnitPrototype* Goliath        = new UnitPrototype("Goliath"               , BW::UnitType::Terran_Goliath,         RaceType::Terran,    0,   0, MOVABLE_CAN_ATTACK);
    UnitPrototype* Ghost          = new UnitPrototype("Ghost"                 , BW::UnitType::Terran_Ghost,           RaceType::Terran,    0,   0, MOVABLE_CAN_ATTACK);
    UnitPrototype* Wraith         = new UnitPrototype("Wraith"                , BW::UnitType::Terran_Wraith,          RaceType::Terran,    0,   0, MOVABLE_CAN_ATTACK);
    UnitPrototype* BattleCruiser  = new UnitPrototype("Terran Battle Cruiser" , BW::UnitType::Terran_BattleCruiser,   RaceType::Terran,    0,   0, MOVABLE_CAN_ATTACK);
    UnitPrototype* Vulture        = new UnitPrototype("Terran Vulture"        , BW::UnitType::Terran_Vulture,         RaceType::Terran,    0,   0, MOVABLE_CAN_ATTACK);

    /** Protoss*/
    UnitPrototype* Probe          = new UnitPrototype("Protoss Probe"         , BW::UnitType::Protoss_Probe,          RaceType::Protoss,  20,   0, MOVABLE_CAN_ATTACK | AbilityFlags::Gather);
    UnitPrototype* Zealot         = new UnitPrototype("Protoss Zealot"        , BW::UnitType::Protoss_Zealot,         RaceType::Protoss,  80,   0, MOVABLE_CAN_ATTACK);
    UnitPrototype* Nexus          = new UnitPrototype("Protoss Nexus"         , BW::UnitType::Protoss_Nexus,          RaceType::Protoss, 750,   0, 0);
    UnitPrototype* Pylon          = new UnitPrototype("Protoss Pylon"         , BW::UnitType::Protoss_Pylon,          RaceType::Protoss, 300,   0, 0);
    UnitPrototype* Gateway        = new UnitPrototype("Protoss Gateway"       , BW::UnitType::Protoss_Gateway,        RaceType::Protoss, 500,   0, 0);
    /** Zerg */
    UnitPrototype* Drone          = new UnitPrototype("Drone"                 , BW::UnitType::Zerg_Drone,             RaceType::Zerg,      0,   0, MOVABLE_CAN_ATTACK);
    UnitPrototype* Larva          = new UnitPrototype("Larva"                 , BW::UnitType::Zerg_Larva,             RaceType::Zerg,      0,   0, 0);
    UnitPrototype* Overlord       = new UnitPrototype("Overlord"              , BW::UnitType::Zerg_Overlord,          RaceType::Zerg,      0,   0, AbilityFlags::Move);
    UnitPrototype* Zergling       = new UnitPrototype("Zergling"              , BW::UnitType::Zerg_Zergling,          RaceType::Zerg,      0,   0, MOVABLE_CAN_ATTACK);
    UnitPrototype* Queen          = new UnitPrototype("Zerg Queen"            , BW::UnitType::Zerg_Queen,             RaceType::Zerg,      0, 200, AbilityFlags::Move | AbilityFlags::Ensare | AbilityFlags::Broodling);
    UnitPrototype* Mutalisk       = new UnitPrototype("Zerg Mutalisk"         , BW::UnitType::Zerg_Mutalisk,          RaceType::Zerg,      0,   0, MOVABLE_CAN_ATTACK);
    UnitPrototype* Hatchery       = new UnitPrototype("Zerg Hatchery"         , BW::UnitType::Zerg_Hatchery,          RaceType::Zerg,      0,   0, 0);
    UnitPrototype* SpawningPool   = new UnitPrototype("Zerg Spawning pool"    , BW::UnitType::Zerg_SpawningPool,      RaceType::Zerg,      0,   0, 0);
    /** Neutral */
    UnitPrototype* Minerals       = new UnitPrototype("Minerals"              , BW::UnitType::Resource_MineralPatch1, RaceType::Other,     0,   0, 0);    
   }
}