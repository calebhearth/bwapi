#include <string>
#include <map>
#include <BWAPI/Order.h>
#include <Util/Foreach.h>

#include "Common.h"

#include "../../Debug.h"

namespace BWAPI
{
  std::string orderName[191];
  std::map<std::string, Order> orderMap;
  namespace Orders
  {
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
      orderName[Die]                    = "Die";
      orderName[Stop]                   = "Stop";
      orderName[Guard]                  = "Guard";
      orderName[PlayerGuard]            = "PlayerGuard";
      orderName[TurretGuard]            = "TurretGuard";
      orderName[BunkerGuard]            = "BunkerGuard";
      orderName[Move]                   = "Move";
      orderName[AttackUnit]             = "AttackUnit";
      orderName[AttackTile]             = "AttackTile";
      orderName[Hover]                  = "Hover";
      orderName[AttackMove]             = "AttackMove";
      orderName[InfestedCommandCenter]  = "InfestedCommandCenter";
      orderName[UnusedNothing]          = "UnusedNothing";
      orderName[UnusedPowerup]          = "UnusedPowerup";
      orderName[TowerGuard]             = "TowerGuard";
      orderName[VultureMine]            = "VultureMine";
      orderName[Nothing]                = "Nothing";
      orderName[CastInfestation]        = "CastInfestation";
      orderName[InfestingCommandCenter] = "InfestingCommandCenter";
      orderName[PlaceBuilding]          = "PlaceBuilding";
      orderName[BuildProtoss2]          = "BuildProtoss2";
      orderName[ConstructingBuilding]   = "ConstructingBuilding";
      orderName[Repair]                 = "Repair";
      orderName[PlaceAddon]             = "PlaceAddon";
      orderName[BuildAddon]             = "BuildAddon";
      orderName[Train]                  = "Train";
      orderName[RallyPointUnit]         = "RallyPointUnit";
      orderName[RallyPointTile]         = "RallyPointTile";
      orderName[ZergBirth]              = "ZergBirth";
      orderName[ZergUnitMorph]          = "ZergUnitMorph";
      orderName[ZergBuildingMorph]      = "ZergBuildingMorph";
      orderName[IncompleteBuilding]     = "IncompleteBuilding";
      orderName[BuildNydusExit]         = "BuildNydusExit";
      orderName[EnterNydusCanal]        = "EnterNydusCanal";
      orderName[Follow]                 = "Follow";
      orderName[Carrier]                = "Carrier";
      orderName[ReaverCarrierMove]      = "ReaverCarrierMove";
      orderName[CarrierIgnore2]         = "CarrierIgnore2";
      orderName[Reaver]                 = "Reaver";
      orderName[TrainFighter]           = "TrainFighter";
      orderName[InterceptorAttack]      = "InterceptorAttack";
      orderName[ScarabAttack]           = "ScarabAttack";
      orderName[RechargeShieldsUnit]    = "RechargeShieldsUnit";
      orderName[RechargeShieldsBattery] = "RechargeShieldsBattery";
      orderName[ShieldBattery]          = "ShieldBattery";
      orderName[InterceptorReturn]      = "InterceptorReturn";
      orderName[BuildingLand]           = "BuildingLand";
      orderName[BuildingLiftOff]        = "BuildingLiftOff";
      orderName[DroneLiftOff]           = "DroneLiftOff";
      orderName[LiftingOff]             = "LiftingOff";
      orderName[ResearchTech]           = "ResearchTech";
      orderName[Upgrade]                = "Upgrade";
      orderName[Larva]                  = "Larva";
      orderName[SpawningLarva]          = "SpawningLarva";
      orderName[Harvest1]               = "Harvest1";
      orderName[Harvest2]               = "Harvest2";
      orderName[MoveToGas]              = "MoveToGas";
      orderName[WaitForGas]             = "WaitForGas";
      orderName[HarvestGas]             = "HarvestGas";
      orderName[ReturnGas]              = "ReturnGas";
      orderName[MoveToMinerals]         = "MoveToMinerals";
      orderName[WaitForMinerals]        = "WaitForMinerals";
      orderName[MiningMinerals]         = "MiningMinerals";
      orderName[Harvest3]               = "Harvest3";
      orderName[Harvest4]               = "Harvest4";
      orderName[ReturnMinerals]         = "ReturnMinerals";
      orderName[Interrupted]            = "Interrupted";
      orderName[EnterTransport]         = "EnterTransport";
      orderName[PickupIdle]             = "PickupIdle";
      orderName[PickupTransport]        = "PickupTransport";
      orderName[PickupBunker]           = "PickupBunker";
      orderName[Pickup4]                = "Pickup4";
      orderName[PowerupIdle]            = "PowerupIdle";
      orderName[Sieging]                = "Sieging";
      orderName[Unsieging]              = "Unsieging";
      orderName[InitCreepGrowth]        = "InitCreepGrowth";
      orderName[SpreadCreep]            = "SpreadCreep";
      orderName[StoppingCreepGrowth]    = "StoppingCreepGrowth";
      orderName[GuardianAspect]         = "GuardianAspect";
      orderName[ArchonWarp]             = "ArchonWarp";
      orderName[CompletingArchonSummon] = "CompletingArchonsummon";
      orderName[HoldPosition]           = "HoldPosition";
      orderName[Cloak]                  = "Cloak";
      orderName[Decloak]                = "Decloak";
      orderName[Unload]                 = "Unload";
      orderName[MoveUnload]             = "MoveUnload";
      orderName[FireYamatoGun]          = "FireYamatoGun";
      orderName[CastLockdown]           = "CastLockdown";
      orderName[Burrowing]              = "Burrowing";
      orderName[Burrowed]               = "Burrowed";
      orderName[Unburrowing]            = "Unburrowing";
      orderName[CastDarkSwarm]          = "CastDarkSwarm";
      orderName[CastParasite]           = "CastParasite";
      orderName[CastSpawnBroodlings]    = "CastSpawnBroodlings";
      orderName[CastEMPShockwave]       = "CastEMPShockwave";
      orderName[NukeWait]               = "NukeWait";
      orderName[NukeTrain]              = "NukeTrain";
      orderName[NukeLaunch]             = "NukeLaunch";
      orderName[NukePaint]              = "NukePaint";
      orderName[NukeUnit]               = "NukeUnit";
      orderName[CastNuclearStrike]      = "CastNuclearStrike";
      orderName[NukeTrack]              = "NukeTrack";
      orderName[CloakNearbyUnits]       = "CloakNearbyUnits";
      orderName[PlaceMine]              = "PlaceMine";
      orderName[RightClickAction]       = "RightClickAction";
      orderName[CastRecall]             = "CastRecall";
      orderName[Teleport]     = "TeleporttoLocation";
      orderName[CastScannerSweep]       = "CastScannerSweep";
      orderName[Scanner]                = "Scanner";
      orderName[CastDefensiveMatrix]    = "CastDefensiveMatrix";
      orderName[CastPsionicStorm]       = "CastPsionicStorm";
      orderName[CastIrradiate]          = "CastIrradiate";
      orderName[CastPlague]             = "CastPlague";
      orderName[CastConsume]            = "CastConsume";
      orderName[CastEnsnare]            = "CastEnsnare";
      orderName[CastStasisField]        = "CastStasisField";
      orderName[CastHallucination]      = "CastHallucination";
      orderName[Hallucination2]         = "Hallucination2";
      orderName[ResetCollision]         = "ResetCollision";
      orderName[Patrol]                 = "Patrol";
      orderName[CTFCOPInit]             = "CTFCOPInit";
      orderName[CTFCOPStarted]          = "CTFCOPStarted";
      orderName[CTFCOP2]                = "CTFCOP2";
      orderName[ComputerAI]             = "ComputerAI";
      orderName[AtkMoveEP]              = "AtkMoveEP";
      orderName[HarassMove]             = "HarassMove";
      orderName[AIPatrol]               = "AIPatrol";
      orderName[GuardPost]              = "GuardPost";
      orderName[RescuePassive]          = "RescuePassive";
      orderName[Neutral]                = "Neutral";
      orderName[ComputerReturn]         = "ComputerReturn";
      orderName[SelfDestructing]         = "SelfDestrucing";
      orderName[Critter]                = "Critter";
      orderName[HiddenGun]              = "HiddenGun";
      orderName[OpenDoor]               = "OpenDoor";
      orderName[CloseDoor]              = "CloseDoor";
      orderName[HideTrap]               = "HideTrap";
      orderName[RevealTrap]             = "RevealTrap";
      orderName[EnableDoodad]           = "Enabledoodad";
      orderName[DisableDoodad]          = "Disabledoodad";
      orderName[WarpIn]                 = "Warpin";
      orderName[Medic]                  = "Medic";
      orderName[MedicHeal1]             = "MedicHeal1";
      orderName[HealMove]               = "HealMove";
      orderName[MedicHeal2]             = "MedicHeal2";
      orderName[CastRestoration]        = "CastRestoration";
      orderName[CastDisruptionWeb]      = "CastDisruptionWeb";
      orderName[CastMindControl]        = "CastMindControl";
      orderName[DarkArchonMeld]         = "DarkArchonMeld";
      orderName[CastFeedback]           = "CastFeedback";
      orderName[CastOpticalFlare]       = "CastOpticalFlare";
      orderName[CastMaelstrom]          = "CastMaelstrom";
      orderName[JunkYardDog]            = "JunkYardDog";
      orderName[Fatal]                  = "Fatal";
      orderName[None]                   = "None";
      orderName[Unknown]                = "Unknown";

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
    return orderName[this->getID()];
  }
  const char *Order::c_str() const
  {
    return orderName[this->getID()].c_str();
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
