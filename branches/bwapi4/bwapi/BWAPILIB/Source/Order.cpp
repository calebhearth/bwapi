#include <string>
#include <map>
#include <BWAPI/Order.h>
#include <Util/Foreach.h>

#include "Common.h"

#include "../../Debug.h"

namespace BWAPI
{
  std::map<std::string, Order> orderMap;
  namespace Orders
  {
    static const std::string orderNames[Enum::MAX] = 
    { 
      "Die", "Stop", "Guard", "PlayerGuard", "TurretGuard", "BunkerGuard", "Move", "", "", "",
      "AttackUnit", "", "AttackTile", "Hover", "AttackMove", "InfestedCommandCenter", "", "", 
      "TowerGuard", "TowerAttack", "VultureMine", "", "", "Nothing", "", "", "", "CastInfestation",
      "", "InfestingCommandCenter", "PlaceBuilding", "", "BuildProtoss2", "ConstructingBuilding",
      "Repair", "", "PlaceAddon", "BuildAddon", "Train", "RallyPointUnit", "RallyPointTile", 
      "ZergBirth", "ZergUnitMorph", "ZergBuildingMorph", "IncompleteBuilding", "", "BuildNydusExit",
      "EnterNydusCanal", "", "Follow", "Carrier", "ReaverCarrierMove", "", "", "", "CarrierIgnore2",
      "", "", "Reaver", "", "", "", "", "TrainFighter", "InterceptorAttack", "ScarabAttack",
      "RechargeShieldsUnit", "RechargeShieldsBattery", "ShieldBattery", "InterceptorReturn", "",
      "BuildingLand", "BuildingLiftOff", "DroneLiftOff", "LiftingOff", "ResearchTech", "Upgrade",
      "Larva", "SpawningLarva", "Harvest1", "Harvest2", "MoveToGas", "WaitForGas", "HarvestGas",
      "ReturnGas", "MoveToMinerals", "WaitForMinerals", "MiningMinerals", "Harvest3", "Harvest4",
      "ReturnMinerals", "Interrupted", "EnterTransport", "PickupIdle", "PickupTransport", "PickupBunker",
      "Pickup4", "PowerupIdle", "Sieging", "Unsieging", "", "InitCreepGrowth", "SpreadCreep",
      "StoppingCreepGrowth", "GuardianAspect", "ArchonWarp", "CompletingArchonSummon", "HoldPosition",
      "", "Cloak", "Decloak", "Unload", "MoveUnload", "FireYamatoGun", "", "CastLockdown", "Burrowing",
      "Burrowed", "Unburrowing", "CastDarkSwarm", "CastParasite", "CastSpawnBroodlings", "CastEMPShockwave",
      "NukeWait", "NukeTrain", "NukeLaunch", "NukePaint", "NukeUnit", "CastNuclearStrike", "NukeTrack", "",
      "CloakNearbyUnits", "PlaceMine", "RightClickActioni", "", "", "", "CastRecall", "Teleport",
      "CastScannerSweep", "Scanner", "CastDefensiveMatrix", "CastPsionicStorm", "CastIrradiate",
      "CastPlague", "CastConsume", "CastEnsnare", "CastStasisField", "CastHallucination", "Hallucination2",
      "ResetCollision", "", "Patrol", "CTFCOPInit", "CTFCOPStarted", "CTFCOP2", "ComputerAI", "AtkMoveEP",
      "HarassMove", "AIPatrol", "GuardPost", "RescuePassive", "Neutral", "ComputerReturn", "",
      "SelfDestructing", "Critter", "HiddenGun", "OpenDoor", "CloseDoor", "HideTrap", "RevealTrap",
      "EnableDoodad", "DisableDoodad", "WarpIn", "Medic", "MedicHeal1", "HealMove", "", "MedicHeal2",
      "CastRestoratioin", "CastDisruptionWeb", "CastMindControl", "DarkArchonMeld", "CastFeedback",
      "CastOpticalFlare", "CastMaelstrom", "JunkYardDog", "Fatal", "None", "Unknown"
    };
    static const Order::set orderSet( Die | Stop | Guard | PlayerGuard | TurretGuard | BunkerGuard | Move | AttackUnit | 
                    AttackTile | Hover | AttackMove | InfestedCommandCenter | UnusedNothing | UnusedPowerup | 
                    TowerGuard | VultureMine | Nothing | CastInfestation | InfestingCommandCenter | 
                    PlaceBuilding | BuildProtoss2 | ConstructingBuilding | Repair | PlaceAddon | BuildAddon |
                    Train | RallyPointUnit | RallyPointTile | ZergBirth | ZergUnitMorph | ZergBuildingMorph |
                    IncompleteBuilding | BuildNydusExit | EnterNydusCanal | Follow | Carrier | ReaverCarrierMove |
                    CarrierIgnore2 | Reaver | TrainFighter | InterceptorAttack | ScarabAttack | RechargeShieldsUnit |
                    RechargeShieldsBattery | ShieldBattery | InterceptorReturn | BuildingLand | BuildingLiftOff |
                    DroneLiftOff | LiftingOff | ResearchTech | Upgrade | Larva | SpawningLarva | Harvest1 | Harvest2 |
                    MoveToGas | WaitForGas | HarvestGas | ReturnGas | MoveToMinerals | WaitForMinerals | MiningMinerals |
                    Harvest3 | Harvest4 | ReturnMinerals | Interrupted | EnterTransport | PickupIdle | PickupTransport |
                    PickupBunker | Pickup4 | PowerupIdle | Sieging | Unsieging | InitCreepGrowth | SpreadCreep | 
                    StoppingCreepGrowth | GuardianAspect | ArchonWarp | CompletingArchonSummon | HoldPosition |
                    Cloak | Decloak | Unload | MoveUnload | FireYamatoGun | CastLockdown | Burrowing | Burrowed |
                    Unburrowing | CastDarkSwarm | CastParasite | CastSpawnBroodlings | CastEMPShockwave | NukeWait |
                    NukeTrain | NukeLaunch | NukePaint | NukeUnit | CastNuclearStrike | NukeTrack | CloakNearbyUnits |
                    PlaceMine | RightClickAction | CastRecall | Teleport | CastScannerSweep | Scanner |
                    CastDefensiveMatrix | CastPsionicStorm | CastIrradiate | CastPlague | CastConsume | CastEnsnare |
                    CastStasisField | CastHallucination | Hallucination2 | ResetCollision | Patrol | CTFCOPInit |
                    CTFCOPStarted | CTFCOP2 | ComputerAI | AtkMoveEP | HarassMove | AIPatrol | GuardPost | RescuePassive |
                    Neutral | ComputerReturn | SelfDestructing | Critter | HiddenGun | OpenDoor | CloseDoor | HideTrap |
                    RevealTrap | EnableDoodad | DisableDoodad | WarpIn | Medic | MedicHeal1 | HealMove | MedicHeal2 |
                    CastRestoration | CastDisruptionWeb | CastMindControl | DarkArchonMeld | CastFeedback | CastOpticalFlare |
                    CastMaelstrom | JunkYardDog | Fatal | None | Unknown );
    void init()
    {
      foreach(Order i, orderSet)
      {
        std::string name(i.getName());
        fixName(&name);
        orderMap.insert(std::make_pair(name, i));
      }
    }
  }

  Order::Order(int id) : Type( id )
  {
  }
  const std::string &Order::getName() const
  {
    return Orders::orderNames[this->getID()];
  }
  const char *Order::c_str() const
  {
    return Orders::orderNames[this->getID()].c_str();
  }
  Order Orders::getOrder(std::string name)
  {
    fixName(&name);
    std::map<std::string, Order>::iterator i = orderMap.find(name);
    if (i == orderMap.end())
      return Orders::Unknown;
    return (*i).second;
  }
  const Order::set& Orders::allOrders()
  {
    return orderSet;
  }
}
