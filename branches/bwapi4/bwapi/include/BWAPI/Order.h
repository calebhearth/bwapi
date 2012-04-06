#pragma once
#include "Type.h"

#define BWAPI_ORDERDEF(x) static const Order x(Enum::x) /** ref x */

namespace BWAPI
{
  namespace Orders
  {
    namespace Enum
    {
      enum Enum
      {
        Die                    = 0,
        Stop                   = 1,
        Guard                  = 2,
        PlayerGuard            = 3,
        TurretGuard            = 4,
        BunkerGuard            = 5,
        Move                   = 6,
        ReaverStop,
        Attack1,
        Attack2,
        AttackUnit             = 10,
        AttackFixedRange,
        AttackTile             = 12,
        Hover                  = 13,
        AttackMove             = 14,
        InfestedCommandCenter  = 15,
        UnusedNothing          = 16,
        UnusedPowerup          = 17,
        TowerGuard             = 18,
        TowerAttack            = 19,
        VultureMine            = 20,
        StayInRange,
        TurretAttack,
        Nothing                = 23,
        Unused_24              = 24,
        DroneStartBuild,
        DroneBuild,
        CastInfestation        = 27,
        MoveToInfest,
        InfestingCommandCenter = 29,
        PlaceBuilding          = 30,
        PlaceProtossBuilding,
        CreateProtossBuilding,
        ConstructingBuilding   = 33,
        Repair                 = 34,
        MoveToRepair,
        PlaceAddon             = 36,
        BuildAddon             = 37,
        Train                  = 38,
        RallyPointUnit         = 39,
        RallyPointTile         = 40,
        ZergBirth              = 41,
        ZergUnitMorph          = 42,
        ZergBuildingMorph      = 43,
        IncompleteBuilding     = 44,
        IncompleteMorphing,
        BuildNydusExit         = 46,
        EnterNydusCanal        = 47,
        IncompleteWarping,
        Follow                 = 49,
        Carrier                = 50,
        ReaverCarrierMove      = 51,
        CarrierStop,
        CarrierAttack,
        CarrierMoveToAttack,
        CarrierIgnore2         = 55,
        CarrierFight,
        CarrierHoldPosition,
        Reaver                 = 58,
        ReaverAttack,
        ReaverMoveToAttack,
        ReaverFight,
        ReaverHoldPosition,
        TrainFighter           = 63,
        InterceptorAttack      = 64,
        ScarabAttack           = 65,
        RechargeShieldsUnit    = 66,
        RechargeShieldsBattery = 67,
        ShieldBattery          = 68,
        InterceptorReturn      = 69,
        DroneLand,
        BuildingLand           = 71,
        BuildingLiftOff        = 72,
        DroneLiftOff           = 73,
        LiftingOff             = 74,
        ResearchTech           = 75,
        Upgrade                = 76,
        Larva                  = 77,
        SpawningLarva          = 78,
        Harvest1               = 79,
        Harvest2               = 80,
        MoveToGas              = 81,
        WaitForGas             = 82,
        HarvestGas             = 83,
        ReturnGas              = 84,
        MoveToMinerals         = 85,
        WaitForMinerals        = 86,
        MiningMinerals         = 87,
        Harvest3               = 88,
        Harvest4               = 89,
        ReturnMinerals         = 90,
        Interrupted            = 91,
        EnterTransport         = 92,
        PickupIdle             = 93,
        PickupTransport        = 94,
        PickupBunker           = 95,
        Pickup4                = 96,
        PowerupIdle            = 97,
        Sieging                = 98,
        Unsieging              = 99,
        WatchTarget,
        InitCreepGrowth        = 101,
        SpreadCreep            = 102,
        StoppingCreepGrowth    = 103,
        GuardianAspect         = 104,
        ArchonWarp             = 105,
        CompletingArchonSummon = 106,
        HoldPosition           = 107,
        QueenHoldPosition,
        Cloak                  = 109,
        Decloak                = 110,
        Unload                 = 111,
        MoveUnload             = 112,
        FireYamatoGun          = 113,
        MoveToFireYamatoGun,
        CastLockdown           = 115,
        Burrowing              = 116,
        Burrowed               = 117,
        Unburrowing            = 118,
        CastDarkSwarm          = 119,
        CastParasite           = 120,
        CastSpawnBroodlings    = 121,
        CastEMPShockwave       = 122,
        NukeWait               = 123,
        NukeTrain              = 124,
        NukeLaunch             = 125,
        NukePaint              = 126,
        NukeUnit               = 127,
        CastNuclearStrike      = 128,
        NukeTrack              = 129,
        InitializeArbiter,
        CloakNearbyUnits       = 131,
        PlaceMine              = 132,
        RightClickAction       = 133,
        SuicideUnit,
        SuicideLocation,
        SuicideHoldPosition,
        CastRecall             = 137,
        Teleport               = 138,
        CastScannerSweep       = 139,
        Scanner                = 140,
        CastDefensiveMatrix    = 141,
        CastPsionicStorm       = 142,
        CastIrradiate          = 143,
        CastPlague             = 144,
        CastConsume            = 145,
        CastEnsnare            = 146,
        CastStasisField        = 147,
        CastHallucination      = 148,
        Hallucination2         = 149,
        ResetCollision         = 150,
        ResetHarvestCollision,
        Patrol                 = 152,
        CTFCOPInit             = 153,
        CTFCOPStarted          = 154,
        CTFCOP2                = 155,
        ComputerAI             = 156,
        AtkMoveEP              = 157,
        HarassMove             = 158,
        AIPatrol               = 159,
        GuardPost              = 160,
        RescuePassive          = 161,
        Neutral                = 162,
        ComputerReturn         = 163,
        InitializePsiProvider,
        SelfDestructing        = 165,
        Critter                = 166,
        HiddenGun              = 167,
        OpenDoor               = 168,
        CloseDoor              = 169,
        HideTrap               = 170,
        RevealTrap             = 171,
        EnableDoodad           = 172,
        DisableDoodad          = 173,
        WarpIn                 = 174,
        Medic                  = 175,
        MedicHeal              = 176,
        HealMove               = 177,
        MedicHoldPosition,
        MedicHealToIdle        = 179,
        CastRestoration        = 180,
        CastDisruptionWeb      = 181,
        CastMindControl        = 182,
        DarkArchonMeld         = 183,
        CastFeedback           = 184,
        CastOpticalFlare       = 185,
        CastMaelstrom          = 186,
        JunkYardDog            = 187,
        Fatal                  = 188,
        None                   = 189,
        Unknown                = 190,
        MAX
      };
    }
  }
  /** To get detailed information about what a unit is doing, you can use the Unit::getOrder method, which
   * will return an Order object. Note that a single command, like gather minerals, can consist of several
   * orders ( MoveToMinerals, HarvestMinerals2, MiningMinerals, ReturnMinerals, etc) which will indicate what
   * state the unit is in while executing the command. For information about how to issue commands to units,
   * go to Unit. */
  class Order : public Type<Order, Orders::Enum::Unknown>
  {
    public:
      Order(int id = Orders::Enum::None);

      /** Returns the name of this order. */
      const std::string &getName() const;
      const char *c_str() const;
  };
  namespace Orders
  {
    /** Given the name of an order, getOrder() will return the corresponding order object. */
    Order getOrder(std::string name);

    /** Returns the set of all the Orders. */
    const Order::set& allOrders();

    void init();

    BWAPI_ORDERDEF(Die);
    BWAPI_ORDERDEF(Stop);
    BWAPI_ORDERDEF(Guard);
    BWAPI_ORDERDEF(PlayerGuard);
    BWAPI_ORDERDEF(TurretGuard);
    BWAPI_ORDERDEF(BunkerGuard);
    BWAPI_ORDERDEF(Move);
    BWAPI_ORDERDEF(AttackUnit);
    BWAPI_ORDERDEF(AttackTile);
    BWAPI_ORDERDEF(Hover);
    BWAPI_ORDERDEF(AttackMove);
    BWAPI_ORDERDEF(InfestedCommandCenter);
    BWAPI_ORDERDEF(UnusedNothing);
    BWAPI_ORDERDEF(UnusedPowerup);
    BWAPI_ORDERDEF(TowerGuard);
    BWAPI_ORDERDEF(VultureMine);
    BWAPI_ORDERDEF(Nothing);
    BWAPI_ORDERDEF(CastInfestation);
    BWAPI_ORDERDEF(InfestingCommandCenter);
    BWAPI_ORDERDEF(PlaceBuilding);
    BWAPI_ORDERDEF(CreateProtossBuilding);
    BWAPI_ORDERDEF(ConstructingBuilding);
    BWAPI_ORDERDEF(Repair);
    BWAPI_ORDERDEF(PlaceAddon);
    BWAPI_ORDERDEF(BuildAddon);
    BWAPI_ORDERDEF(Train);
    BWAPI_ORDERDEF(RallyPointUnit);
    BWAPI_ORDERDEF(RallyPointTile);
    BWAPI_ORDERDEF(ZergBirth);
    BWAPI_ORDERDEF(ZergUnitMorph);
    BWAPI_ORDERDEF(ZergBuildingMorph);
    BWAPI_ORDERDEF(IncompleteBuilding);
    BWAPI_ORDERDEF(BuildNydusExit);
    BWAPI_ORDERDEF(EnterNydusCanal);
    BWAPI_ORDERDEF(Follow);
    BWAPI_ORDERDEF(Carrier);
    BWAPI_ORDERDEF(ReaverCarrierMove);
    BWAPI_ORDERDEF(CarrierIgnore2);
    BWAPI_ORDERDEF(Reaver);
    BWAPI_ORDERDEF(TrainFighter);
    BWAPI_ORDERDEF(InterceptorAttack);
    BWAPI_ORDERDEF(ScarabAttack);
    BWAPI_ORDERDEF(RechargeShieldsUnit);
    BWAPI_ORDERDEF(RechargeShieldsBattery);
    BWAPI_ORDERDEF(ShieldBattery);
    BWAPI_ORDERDEF(InterceptorReturn);
    BWAPI_ORDERDEF(BuildingLand);
    BWAPI_ORDERDEF(BuildingLiftOff);
    BWAPI_ORDERDEF(DroneLiftOff);
    BWAPI_ORDERDEF(LiftingOff);
    BWAPI_ORDERDEF(ResearchTech);
    BWAPI_ORDERDEF(Upgrade);
    BWAPI_ORDERDEF(Larva);
    BWAPI_ORDERDEF(SpawningLarva);
    BWAPI_ORDERDEF(Harvest1);
    BWAPI_ORDERDEF(Harvest2);
    BWAPI_ORDERDEF(MoveToGas);
    BWAPI_ORDERDEF(WaitForGas);
    BWAPI_ORDERDEF(HarvestGas);
    BWAPI_ORDERDEF(ReturnGas);
    BWAPI_ORDERDEF(MoveToMinerals);
    BWAPI_ORDERDEF(WaitForMinerals);
    BWAPI_ORDERDEF(MiningMinerals);
    BWAPI_ORDERDEF(Harvest3);
    BWAPI_ORDERDEF(Harvest4);
    BWAPI_ORDERDEF(ReturnMinerals);
    BWAPI_ORDERDEF(Interrupted);
    BWAPI_ORDERDEF(EnterTransport);
    BWAPI_ORDERDEF(PickupIdle);
    BWAPI_ORDERDEF(PickupTransport);
    BWAPI_ORDERDEF(PickupBunker);
    BWAPI_ORDERDEF(Pickup4);
    BWAPI_ORDERDEF(PowerupIdle);
    BWAPI_ORDERDEF(Sieging);
    BWAPI_ORDERDEF(Unsieging);
    BWAPI_ORDERDEF(InitCreepGrowth);
    BWAPI_ORDERDEF(SpreadCreep);
    BWAPI_ORDERDEF(StoppingCreepGrowth);
    BWAPI_ORDERDEF(GuardianAspect);
    BWAPI_ORDERDEF(ArchonWarp);
    BWAPI_ORDERDEF(CompletingArchonSummon);
    BWAPI_ORDERDEF(HoldPosition);
    BWAPI_ORDERDEF(Cloak);
    BWAPI_ORDERDEF(Decloak);
    BWAPI_ORDERDEF(Unload);
    BWAPI_ORDERDEF(MoveUnload);
    BWAPI_ORDERDEF(FireYamatoGun);
    BWAPI_ORDERDEF(CastLockdown);
    BWAPI_ORDERDEF(Burrowing);
    BWAPI_ORDERDEF(Burrowed);
    BWAPI_ORDERDEF(Unburrowing);
    BWAPI_ORDERDEF(CastDarkSwarm);
    BWAPI_ORDERDEF(CastParasite);
    BWAPI_ORDERDEF(CastSpawnBroodlings);
    BWAPI_ORDERDEF(CastEMPShockwave);
    BWAPI_ORDERDEF(NukeWait);
    BWAPI_ORDERDEF(NukeTrain);
    BWAPI_ORDERDEF(NukeLaunch);
    BWAPI_ORDERDEF(NukePaint);
    BWAPI_ORDERDEF(NukeUnit);
    BWAPI_ORDERDEF(CastNuclearStrike);
    BWAPI_ORDERDEF(NukeTrack);
    BWAPI_ORDERDEF(CloakNearbyUnits);
    BWAPI_ORDERDEF(PlaceMine);
    BWAPI_ORDERDEF(RightClickAction);
    BWAPI_ORDERDEF(CastRecall);
    BWAPI_ORDERDEF(Teleport);
    BWAPI_ORDERDEF(CastScannerSweep);
    BWAPI_ORDERDEF(Scanner);
    BWAPI_ORDERDEF(CastDefensiveMatrix);
    BWAPI_ORDERDEF(CastPsionicStorm);
    BWAPI_ORDERDEF(CastIrradiate);
    BWAPI_ORDERDEF(CastPlague);
    BWAPI_ORDERDEF(CastConsume);
    BWAPI_ORDERDEF(CastEnsnare);
    BWAPI_ORDERDEF(CastStasisField);
    BWAPI_ORDERDEF(CastHallucination);
    BWAPI_ORDERDEF(Hallucination2);
    BWAPI_ORDERDEF(ResetCollision);
    BWAPI_ORDERDEF(Patrol);
    BWAPI_ORDERDEF(CTFCOPInit);
    BWAPI_ORDERDEF(CTFCOPStarted);
    BWAPI_ORDERDEF(CTFCOP2);
    BWAPI_ORDERDEF(ComputerAI);
    BWAPI_ORDERDEF(AtkMoveEP);
    BWAPI_ORDERDEF(HarassMove);
    BWAPI_ORDERDEF(AIPatrol);
    BWAPI_ORDERDEF(GuardPost);
    BWAPI_ORDERDEF(RescuePassive);
    BWAPI_ORDERDEF(Neutral);
    BWAPI_ORDERDEF(ComputerReturn);
    BWAPI_ORDERDEF(SelfDestructing);
    BWAPI_ORDERDEF(Critter);
    BWAPI_ORDERDEF(HiddenGun);
    BWAPI_ORDERDEF(OpenDoor);
    BWAPI_ORDERDEF(CloseDoor);
    BWAPI_ORDERDEF(HideTrap);
    BWAPI_ORDERDEF(RevealTrap);
    BWAPI_ORDERDEF(EnableDoodad);
    BWAPI_ORDERDEF(DisableDoodad);
    BWAPI_ORDERDEF(WarpIn);
    BWAPI_ORDERDEF(Medic);
    BWAPI_ORDERDEF(MedicHeal);
    BWAPI_ORDERDEF(HealMove);
    BWAPI_ORDERDEF(MedicHealToIdle);
    BWAPI_ORDERDEF(CastRestoration);
    BWAPI_ORDERDEF(CastDisruptionWeb);
    BWAPI_ORDERDEF(CastMindControl);
    BWAPI_ORDERDEF(DarkArchonMeld);
    BWAPI_ORDERDEF(CastFeedback);
    BWAPI_ORDERDEF(CastOpticalFlare);
    BWAPI_ORDERDEF(CastMaelstrom);
    BWAPI_ORDERDEF(JunkYardDog);
    BWAPI_ORDERDEF(Fatal);
    BWAPI_ORDERDEF(None);
    BWAPI_ORDERDEF(Unknown);
  };
}
