#pragma once
#include <string>
#include <set>
#include "Type.h"
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
		
				AttackUnit             = 10,
		
				AttackTile             = 12,
				Hover                  = 13,
				AttackMove             = 14,
				InfestedCommandCenter  = 15,
				UnusedNothing          = 16,
				UnusedPowerup          = 17,
				TowerGuard             = 18,
				TowerAttack            = 19,
				VultureMine            = 20,
		
				Nothing                = 23,
				Unused_24              = 24,
		
				CastInfestation        = 27,
		
				InfestingCommandCenter = 29,
				PlaceBuilding          = 30,
		
				BuildProtoss2          = 32,
				ConstructingBuilding   = 33,
				Repair                 = 34,
		
				PlaceAddon             = 36,
				BuildAddon             = 37,
				Train                  = 38,
				RallyPointUnit         = 39,
				RallyPointTile         = 40,
				ZergBirth              = 41,
				ZergUnitMorph          = 42,
				ZergBuildingMorph      = 43,
				IncompleteBuilding     = 44,
		
				BuildNydusExit         = 46,
				EnterNydusCanal        = 47,
		
				Follow                 = 49,
				Carrier                = 50,
				ReaverCarrierMove      = 51,
		
				CarrierIgnore2         = 55,
		
				Reaver                 = 58,
	
				TrainFighter           = 63,
				InterceptorAttack      = 64,
				ScarabAttack           = 65,
				RechargeShieldsUnit    = 66,
				RechargeShieldsBattery = 67,
				ShieldBattery          = 68,
				InterceptorReturn      = 69,
		
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
		
				InitCreepGrowth        = 101,
				SpreadCreep            = 102,
				StoppingCreepGrowth    = 103,
				GuardianAspect         = 104,
				ArchonWarp             = 105,
				CompletingArchonsummon = 106,
				HoldPosition           = 107,
		
				Cloak                  = 109,
				Decloak                = 110,
				Unload                 = 111,
				MoveUnload             = 112,
				FireYamatoGun          = 113,
		
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
		
				CloakNearbyUnits       = 131,
				PlaceMine              = 132,
				RightClickAction       = 133,
		
				CastRecall             = 137,
				TeleporttoLocation     = 138,
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
		
				SelfDestrucing         = 165,
				Critter                = 166,
				HiddenGun              = 167,
				OpenDoor               = 168,
				CloseDoor              = 169,
				HideTrap               = 170,
				RevealTrap             = 171,
				Enabledoodad           = 172,
				Disabledoodad          = 173,
				Warpin                 = 174,
				Medic                  = 175,
				MedicHeal1             = 176,
				HealMove               = 177,
		
				MedicHeal2             = 179,
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
		const std::set<Order>& allOrders();

		void init();
		extern const Order Die;
		extern const Order Stop;
		extern const Order Guard;
		extern const Order PlayerGuard;
		extern const Order TurretGuard;
		extern const Order BunkerGuard;
		extern const Order Move;
		extern const Order AttackUnit;
		extern const Order AttackTile;
		extern const Order Hover;
		extern const Order AttackMove;
		extern const Order InfestedCommandCenter;
		extern const Order UnusedNothing;
		extern const Order UnusedPowerup;
		extern const Order TowerGuard;
		extern const Order VultureMine;
		extern const Order Nothing;
		extern const Order Nothing3;
		extern const Order CastInfestation;
		extern const Order InfestingCommandCenter;
		extern const Order PlaceBuilding;
		extern const Order BuildProtoss2;
		extern const Order ConstructingBuilding;
		extern const Order Repair;
		extern const Order PlaceAddon;
		extern const Order BuildAddon;
		extern const Order Train;
		extern const Order RallyPointUnit;
		extern const Order RallyPointTile;
		extern const Order ZergBirth;
		extern const Order ZergUnitMorph;
		extern const Order ZergBuildingMorph;
		extern const Order IncompleteBuilding;
		extern const Order BuildNydusExit;
		extern const Order EnterNydusCanal;
		extern const Order Follow;
		extern const Order Carrier;
		extern const Order ReaverCarrierMove;
		extern const Order CarrierIgnore2;
		extern const Order Reaver;
		extern const Order TrainFighter;
		extern const Order InterceptorAttack;
		extern const Order ScarabAttack;
		extern const Order RechargeShieldsUnit;
		extern const Order RechargeShieldsBattery;
		extern const Order ShieldBattery;
		extern const Order InterceptorReturn;
		extern const Order BuildingLand;
		extern const Order BuildingLiftOff;
		extern const Order DroneLiftOff;
		extern const Order LiftingOff;
		extern const Order ResearchTech;
		extern const Order Upgrade;
		extern const Order Larva;
		extern const Order SpawningLarva;
		extern const Order Harvest1;
		extern const Order Harvest2;
		extern const Order MoveToGas;
		extern const Order WaitForGas;
		extern const Order HarvestGas;
		extern const Order ReturnGas;
		extern const Order MoveToMinerals;
		extern const Order WaitForMinerals;
		extern const Order MiningMinerals;
		extern const Order Harvest3;
		extern const Order Harvest4;
		extern const Order ReturnMinerals;
		extern const Order Interrupted;
		extern const Order EnterTransport;
		extern const Order PickupIdle;
		extern const Order PickupTransport;
		extern const Order PickupBunker;
		extern const Order Pickup4;
		extern const Order PowerupIdle;
		extern const Order Sieging;
		extern const Order Unsieging;
		extern const Order InitCreepGrowth;
		extern const Order SpreadCreep;
		extern const Order StoppingCreepGrowth;
		extern const Order GuardianAspect;
		extern const Order ArchonWarp;
		extern const Order CompletingArchonsummon;
		extern const Order HoldPosition;
		extern const Order Cloak;
		extern const Order Decloak;
		extern const Order Unload;
		extern const Order MoveUnload;
		extern const Order FireYamatoGun;
		extern const Order CastLockdown;
		extern const Order Burrowing;
		extern const Order Burrowed;
		extern const Order Unburrowing;
		extern const Order CastDarkSwarm;
		extern const Order CastParasite;
		extern const Order CastSpawnBroodlings;
		extern const Order CastEMPShockwave;
		extern const Order NukeWait;
		extern const Order NukeTrain;
		extern const Order NukeLaunch;
		extern const Order NukePaint;
		extern const Order NukeUnit;
		extern const Order CastNuclearStrike;
		extern const Order NukeTrack;
		extern const Order CloakNearbyUnits;
		extern const Order PlaceMine;
		extern const Order RightClickAction;
		extern const Order CastRecall;
		extern const Order TeleporttoLocation;
		extern const Order CastScannerSweep;
		extern const Order Scanner;
		extern const Order CastDefensiveMatrix;
		extern const Order CastPsionicStorm;
		extern const Order CastIrradiate;
		extern const Order CastPlague;
		extern const Order CastConsume;
		extern const Order CastEnsnare;
		extern const Order CastStasisField;
		extern const Order CastHallucination;
		extern const Order Hallucination2;
		extern const Order ResetCollision;
		extern const Order Patrol;
		extern const Order CTFCOPInit;
		extern const Order CTFCOPStarted;
		extern const Order CTFCOP2;
		extern const Order ComputerAI;
		extern const Order AtkMoveEP;
		extern const Order HarassMove;
		extern const Order AIPatrol;
		extern const Order GuardPost;
		extern const Order RescuePassive;
		extern const Order Neutral;
		extern const Order ComputerReturn;
		extern const Order SelfDestrucing;
		extern const Order Critter;
		extern const Order HiddenGun;
		extern const Order OpenDoor;
		extern const Order CloseDoor;
		extern const Order HideTrap;
		extern const Order RevealTrap;
		extern const Order Enabledoodad;
		extern const Order Disabledoodad;
		extern const Order Warpin;
		extern const Order Medic;
		extern const Order MedicHeal1;
		extern const Order HealMove;
		extern const Order MedicHeal2;
		extern const Order CastRestoration;
		extern const Order CastDisruptionWeb;
		extern const Order CastMindControl;
		extern const Order DarkArchonMeld;
		extern const Order CastFeedback;
		extern const Order CastOpticalFlare;
		extern const Order CastMaelstrom;
		extern const Order JunkYardDog;
		extern const Order Fatal;
		extern const Order None;
		extern const Order Unknown;
	};
}
