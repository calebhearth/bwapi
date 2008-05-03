#pragma once

#include "Types.h"
#include "StaticAssert.h"

#include "Sprite.h"
#include "Order.h"
#include "OrderID.h"
#include "UnitTypes.h"
#include "MovementFlags.h"

const u32 BW_UNIT_SIZE_IN_BYTES = 336;
const u32 BW_UNIT_ARRAY_MAX_LENGTH = 1700;

namespace BW
{
  #pragma pack(1)
  struct Unit
  {
    /*0x000*/ Unit*                 previousUnit;
    /*0x004*/ Unit*                 nextUnit;
    /*0x008*/ u8                    healthPointsFraction;  
    /*0x009*/ u16                   healthPoints;
    /*0x00B*/ _UNKNOWN _1[1];
    /*0x00C*/ Sprite*               sprite;
    /*0x010*/ Position              moveTo;
    /*0x014*/ Unit*                 targetUnit;
    /*0x018*/ _UNKNOWN _2[8];
    /*0x020*/ MovementFlags::Enum   movementFlags;
    /*0x021*/ _UNKNOWN _3[7];
    /*0x028*/ Position              currentPos;
    /*0x02C*/ _UNKNOWN _4[32];
    /*0x04C*/ u8                    playerID;
    /*0x04D*/ OrderID::Enum         orderID;
    /*0x04E*/ _UNKNOWN _5[18];
    /*0x060*/ u8                    shieldPointsFraction;
    /*0x061*/ u16                   shieldPoints;
    /*0x063*/ _UNKNOWN _6[1];
    /*0x064*/ UnitType::Enum        unitID;
    /*0x066*/ _UNKNOWN _7[2];
    /*0x068*/ BW_Unit*              previousUnitOfPlayer;
    /*0x06C*/ BW_Unit*              nextUnitOfPlayer;
    /*0x070*/ BW_Unit*              subUnit;
    /*0x074*/ Order*                orderQueueHead;
    /*0x078*/ Order*                orderQueueTail;
    /*0x07C*/ _UNKNOWN _8[4];
    /*0x080*/ BW_Unit*              connectedUnit;
    /*0x080*/ _UNKNOWN _9[204];

    //    BW_Unit* ConnectedUnit;
    //    u8 NumOfQueuedOrders;
    //    u8 Unkown_85_Order_Timer_Seprate;
    //    u8 _UNKNOWN7[2];
    //    u16 DisplayedUnitID;
    //    u8 _UNKNOWN8[4];
    //    u8 RankIncrease;
    //    u8 KillCount;
    //    u8 _UNKNOWN9[3];
    //    u8 UnknownState;
    //    u8 CurrentButtonSet;
    //    u16 Queue1;
    //    u16 Queue2;
    //    u16 Queue3;
    //    u16 Queue4;
    //    u16 Queue5;
    //    u16 Energy;
    //    u8 QueueSlot;
    //    u8 _UNKNOWN10[1];
    //    u8 SecondaryOrderID;
    //    u16 HPGainedDuringBuild;
    //    u16 RemainingBuildTime;
    //    union ChildUnion_type
    //    {
    //       struct UnitIsVultureBike_type
    //       {
    //          u8 SpiderMineCount;
    //          u8 Unused[3];
    //       } UnitIsVultureBike;
    //       BW_Unit* Unit_Unknown;
    //       // IF Carrier/Reaver - First Hangar Unit
    //       // IF Scarab/Interceptor - Parent
    //       // IF Building - Addon
    //       // IF Worker - Powerup Carried
    //    } ChildUnion;
    //    union ChildUnion2_type
    //    {
    //       struct UnitIsBuilding_type
    //       {
    //          u16 AddonBuildID;
    //          u16 UpgradeResearchTime;
    //       } UnitIsBuilding;
    //       BW_Unit* NextUnitInParentHangar;
    //       // IF Scarab/Interceptor - Next Unit in Parent Hangar
    //    } ChildUnion2;
    //    union ChildUnion3_type
    //    {
    //       struct UnitIsNotScarabInterceptor 
    //       {
    //          u8 ScarabCount_TechID;
    //          u8 InterceptorCount_UpgradeID;
    //          u8 LarvaSpawnTimer;
    //          u8 IsLanding;
    //       };
    //       BW_Unit* PrevUnitInParentHangar;
    //       // IF Scarab/Interceptor - Previous in Parent's Hangar
    //    } ChildUnion3;
    //    u8 MultiUse1;
    //    // -IF Interceptor/Scarab - InHanger?
    //    // -IF CreepUnit - timer between creep expansions
    //    // -IF Repairing - decrease mineral timer
    //    u8 UpgradeLevel;
    //    u8 IsCarryingSomething;
    //    u8 ResourceAmount;
    //    u8 DONOTCARE1[4];
    //    u32 Status;
    //    //       00000001(0x1) - Is Completed
    //    //       00000010(0x2) - Is on ground? or is it is unit...
    //    //       00000100(0x4) - Is in air
    //    //       00001000(0x8) - Checked for disabled, if it is 00001000, then the unit is disabled(/unpowered?)
    //    //       00010000(0x10) - Checked for burrowing purposes, if it is 00010000, then the unit is burrowed
    //    //       00100000(0x20) - Unit is entering a building
    //    //       01000000(0x40) - unit is entering a transport
    //    //       10000000(0x80) - 
    //    // 
    //    //       00000001(0x100) - Checked for invisible purposes, if it is 00000001, then the unit requires a detector?
    //    //       00000010(0x200) - checked for cloak?
    //    //       00000100(0x400) - deals with doodad states? if set, is disabled
    //    //       00001000(0x800) - Unit cloaking doesn't need energy decrease
    //    //       00010000(0x1000) - Unit is in unbreakable code section? Cannot receive orders
    //    //       00100000(0x2000) - Set by nobrkcodestart
    //    //       01000000(0x4000) - 
    //    //       10000000(0x8000) - cannot attack if set
    //    // 
    //    //       00000001(0x10000)
    //    //       00000010(0x20000) - Is a Building?
    //    // 
    //    //       00000100(0x4000000) - Invincible
    //    // 
    //    //       00010000(0x10000000) - Speed upgrade
    //    //       00100000(0x20000000) - cooldown upgrade
    //    u8 ResourceType;
    //    u8 WireframeRandomizer;
    //    u8 SecondaryOrderState;
    //    u8 Unknown_E3_Counter_Down;
    //    u8 _UNKNOWN11[8];
    //    BW_Unit* CurrentBuildUnit;
    //    u8 _UNKNOWN12[8];
    //    union RallyPsiProviderUnion_type
    //    {
    //       struct Rally_type
    //       {
    //          u16 RallyX;
    //          u16 RallyY;
    //          BW_Unit* RallyUnit;
    //       } Rally;
    //       struct PsiProvider_type
    //       {
    //          BW_Unit* PrevPsiProvider;
    //          BW_Unit* NextPsiProvider;
    //       };
    //    } RallyPsiProviderUnion;
    //    u32 Path_Unknown;
    //    u8 _UNKNOWN13[3];
    //    u8 IsBeingHealed;
    //    u16 X_Countours_1_Unknown;
    //    u16 Y_Countours_1_Unknown;
    //    u16 X_Countours_2_Unknown;
    //    u16 Y_Countours_2_Unknown;
    //    u16 RemoveTimer;
    //    // Hallucination, DSwarm, DWeb, Broodling
    //    u16 DefenseMatrixDamage;
    //    u8 DefenseMatrixTimer;
    //    u8 StimTimer;
    //    u8 EnsareTimer;
    //    u8 LockdownTimer;
    //    u8 IrradiateTimer;
    //    u8 StasisTimer;
    //    u8 PlaugeTimer;
    //    u8 IsUnderStrom;
    //    // Tell if a unit is under a psi storm
    //    BW_Unit* IrradiatedBy;
    //    u8 IrradiatePlayerID;
    //    u8 ParasiteFlags;
    //    // Each bit corresponds to a player who parasites this unit
    //    u8 CycleCounter;
    //    // Runs updates 2 times per sec (approx)
    //    u8 IsBlind;
    //    u8 MaelstromCounter;
    //    u8 _UNKNOWN14[1];
    //    u8 AcidSporeCount;
    //    u8 bAcidSpore1Time;
    //    u8 bAcidSpore2Time;
    //    u8 bAcidSpore3Time;
    //    u8 bAcidSpore4Time;
    //    u8 bAcidSpore5Time;
    //    u8 bAcidSpore6Time;
    //    u8 bAcidSpore7Time;
    //    u8 bAcidSpore8Time;
    //    u8 bAcidSpore9Time;
    //    u16 ThreeByThreeOffsetIndex;
    //    u8 _UNKNOWN15[6];
    //    u16 AirStrength; 
    //    // 1 if has no air weapon, but has a ground
    //    u16 GroundStrength;
    //    // 1 if has no ground weapon, but has an air
    //    u8 _UNKNOWN16[16];
    //    u8 Repulse_Unknown1;
    //    u8 Repulse_Unknown2;
    //    u8 DriftPosX; //mapsizex/1.5 max
    //    u8 DriftPosY; //mapsizex/1.5 max
  };
  #pragma pack()

  struct UnitArray
  {
    BW_Unit Unit[BW_UNIT_ARRAY_MAX_LENGTH];
  };

};

BOOST_STATIC_ASSERT(sizeof(BW::Unit) == BW_UNIT_SIZE_IN_BYTES);
BOOST_STATIC_ASSERT(sizeof(BW::UnitArray) == BW_UNIT_SIZE_IN_BYTES*BW_UNIT_ARRAY_MAX_LENGTH);
