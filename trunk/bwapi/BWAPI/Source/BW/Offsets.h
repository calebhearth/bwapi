#pragma once
#pragma pack(1)

#include <Util/Bitmask.h>
#include <Util/Types.h>
#include "../StaticAssert.h"

#include "UnitPrototypeFlags.h"
#include "MiniTileFlags.h"
#include "GroupFlags.h"
#include "PlayerType.h"
#include "Race.h"

/**
 * Broodwar content access tools. The namespace contains:
 * -# Data structers corresponding to bw data structures
 * -# Offests to bw data/functions
 * -# Functions that communicate directly with broodwar.
 */
namespace BW
{
  static const u8  PLAYER_COUNT          =  12;
  static const u8  PLAYABLE_PLAYER_COUNT =   8;
  static const u8  RACE_COUNT            =   3;
  static const u8  UNIT_TYPE_COUNT       = 228;
  static const u8  TECH_TYPE_COUNT       =  44;
  static const u8  UPGRADE_TYPE_COUNT    =  66;

  static const int TILE_SIZE          =  32;
 
  struct Unit;
  struct UnitArray;
  class TileType;
  class DoodatType;
  //-------------------------------------------- PLAYER RESOURCES --------------------------------------------
  /** Direct mapping of players resource amount in the bw memory */
  struct PlayerResources
  {
    /** Direct mapping of resource value of some player (gas or minerals) */
    struct PlayerResource
    {
      s32 player[PLAYER_COUNT];
    };
    PlayerResource minerals;
    PlayerResource gas;
  };
  static PlayerResources* BWXFN_PlayerResources = (PlayerResources*) 0x0057F0D0; 

  //----------------------------------------------- NAMES TYPE -----------------------------------------------
  /** Direct mapping of players info in bw memory */
  struct Players
  {
    /** Direct mapping of player info in bw memory */
    struct PlayerInfo
    {
      u32              id;
      u32              actions; // unused; FF FF FF FF if not a human player
      PlayerType::Enum type;
      Race::Enum       race;
      u8               force;
      char             name[25];
    };
    PlayerInfo   player[PLAYER_COUNT];
  };

  struct Positions
  {
    u16   x;
    u16   y;
  };

  struct ForceName
  {
    char           name[30];
  };

  struct DatLoad
  {
    u32            address;
    u32            length;
    u32            entries;
  };

  struct PlayerAlliance
  {
    struct Alliances
    {
      u8 player[PLAYER_COUNT];
    };
    Alliances alliance[PLAYER_COUNT];
  };

  static DatLoad* upgradesDat = (DatLoad*) 0x005136E0; // 1.15.3 AND 1.16
  static DatLoad* techdataDat = (DatLoad*) 0x005137D8;
  static DatLoad* weaponsDat = (DatLoad*) 0x00513868;
  static DatLoad* unitsDat = (DatLoad*) 0x00513C30;

  static Positions* startPositions = (Positions*) 0x058D700;
  static ForceName* ForceNames = (ForceName*) 0x0058D59C;
  static Players* BWXFN_Players = (Players*) 0x0057EEC0;
  static PlayerAlliance* BWDATA_Alliance = (PlayerAlliance*) 0x0058D614;

  //----------------------------------------------- UNIT STATS -----------------------------------------------
  /** Direct mapping of bw tables used for unti stats like count, completed, kills, deaths */
  struct Counts
  {
    struct UnitStats
    {
      struct PlayerStats
      {
        s32 player[PLAYER_COUNT];
      };
      PlayerStats unit[BW::UNIT_TYPE_COUNT];
    };
    UnitStats all;
    UnitStats completed;
    UnitStats killed;
    UnitStats dead;
  };
  
  static Counts* BWXFN_Counts = (Counts*) 0x00582304;
  static u8* BWOFF_gameType = (u8*) 0x00596800;

  /** Higher 12 bits for tile group, lower 4 bits for variant of tile in the tile group. */
  typedef u16 TileID;
  static TileID**       BWXFN_MapTileArray = (TileID**) 0x005993A4;
  static TileType**     BWXFN_TileSet = (TileType**) 0x006D5EA0;  /**< Index  0-1023 */
  static DoodatType**   BWXFN_DoodatSet = (DoodatType**) 0x006D5EA0; /**< Index 1024 + */
  static u16*           BWXFN_MapSizeX = (u16*) 0x0057F1B4;
  static u16*           BWXFN_MapSizeY = ((u16*) BWXFN_MapSizeX) + 1;
  static Unit**         BWXFN_UnitNodeTable_FirstElement = (Unit**)0x00628410;
  static UnitArray*     BWXFN_UnitNodeTable = (UnitArray*) 0x0059CB38; // starts properly at 0x0059CC88
  const  u32            UNIT_ARRAY_MAX_LENGTH = 1701; // should be 1700
  
  static void (_stdcall* selectUnits)(int count, BW::Unit ** unitsToSelect) = (void (_stdcall*) (int, BW::Unit * *)) 0x004C0750; 
  static void (_stdcall* selectUnitsHelperSTD)(int, BW::Unit **, bool, bool) = (void (_stdcall*) (int, BW::Unit * *, bool, bool)) 0x0049AEE0;
  static u32            BWFXN_IssueCommand = 0x00485AC0; 
  static u32*           BWFXN_InGame = (u32*) 0x006556C0;
  static u8*            BWFXN_InReplay = (u8*) 0x006D0EF4;
  static u32            BWFXN_CountDownTimer = 0x0058D6D4;

  static u32            BWXFN_PrintText = 0x0048D0B0;
  static u32            BWFXN_PrintPublic = 0x0048D0B0;

  static u32            BWXFN_SendPublicCall = 0x004F3107; 
  static u32            BWXFN_SendPublicCallBack = BWXFN_SendPublicCall + 5;
  static u32            BWXFN_SendPublicCallTarget = 0x004C2310; // @todo: Read call location from BWXFN_SendPublicCall

  static Unit*          BWXFN_CurrentPlayerSelectionGroup = (Unit*) 0x005971E8;

  static u32            BWXFN_GameStart = 0x004C9941;
  static u32            BWXFN_GameStartBack = BW::BWXFN_GameStart + 5;
  static u32            BWXFN_GameStartTarget = 0x00417C10; //  @todo: Read call location from BWXFN_GameStart

  static u32            BWXFN_GameEnd = 0x004EE7D3;
  static u32            BWXFN_GameEndBack = BWXFN_GameEnd + 5;
  static u32            BWXFN_GameEndTarget = 0x00416D80; //  @todo: Read call location from BWXFN_GameEnd

  static u32            BWXFN_NextFrameHelperFunction = 0x004D977D; 
  static u32            BWXFN_NextFrameHelperFunctionBack = BWXFN_NextFrameHelperFunction + 5;
  static u32            BWXFN_NextFrameHelperFunctionTarget = 0x004D13B0; //  @todo: Read call location from BWXFN_NextFrameHelperFunction

  static u32            BWXFN_Refresh = 0x0041DDE0;

  static u32            BWXFN_CancelTrainByClickInTheQueue = 0x004573D9; 
  static u32            BWXFN_CancelTrainByClickInTheQueueBack = BWXFN_CancelTrainByClickInTheQueue + 5;
  static u32            BWXFN_CancelTrainByClickInTheQueueTarget = BWFXN_IssueCommand;

  static u32            BWXFN_CancelTrainByEscape = 0x00423494; 
  static u32            BWXFN_CancelTrainByEscapeBack = BWXFN_CancelTrainByEscape + 5;
  static u32            BWXFN_CancelTrainByEscapeTarget = BWFXN_IssueCommand;

  static u32            BWXFN_RemoveUnit = 0x004EC354; 
  static u32            BWXFN_RemoveUnitBack = BWXFN_RemoveUnit + 5;
  static u32            BWXFN_RemoveUnitTarget = 0x00479920; // @todo: Read call location from BWXFN_RemoveUnitTarget

  static u32            BWXFN_Redraw = 0x004D97F3; 
  static u32            BWXFN_RedrawBack = BWXFN_Redraw + 5;
  static u32            BWXFN_RedrawTarget = 0x0041C9F0; //  @todo: Read call location from BWXFN_Redraw

  static u32            BWXFN_NextLogicFrame = 0x004D960E; 
  static u32            BWXFN_NextLogicFrameBack = BWXFN_NextLogicFrame + 5;
  static u32            BWXFN_NextLogicFrameTarget = 0x00488670; //  @todo: Read call location from BWXFN_NextLogicFrame

  static u32            BWFXN_Mouse  = 0x006CDDA4; 
  static int*           BWXFN_MouseX = (int*) BWFXN_Mouse;
  static int*           BWXFN_MouseY = (int*) (BWFXN_Mouse + 4);
  static int*           BWXFN_ScreenX = (int*) 0x00628428; //  @todo: Find better screen x/y location (as a pair, one address)
  static int*           BWXFN_ScreenY = (int*) 0x00628450; 
  static char*          BWXFN_CurrentMapFileName = (char*) 0x0057FD1C;
  static u16*           BWXFN_TechLabelIndex = (u16*) techdataDat[7].address;
  static u16*           BWXFN_UpgradeLabelIndex = (u16*) upgradesDat[8].address;
  static u8*            BWXFN_UpgradeMax = (u8*) upgradesDat[10].address;
  static u16*           BWXFN_UpgradeMineralCostBase = (u16*) upgradesDat[0].address;
  static u16*           BWXFN_UpgradeMineralCostFactor = (u16*) upgradesDat[1].address;
  static u16*           BWXFN_UpgradeGasCostBase = (u16*) upgradesDat[2].address;
  static u16*           BWXFN_UpgradeGasCostFactor = (u16*) upgradesDat[3].address;
  static u16*           BWXFN_UpgradeTimeCostBase = (u16*) upgradesDat[4].address;
  static u16*           BWXFN_UpgradeTimeCostFactor = (u16*) upgradesDat[5].address;
  static Race::Enum*    BWXFN_UpgradeRace = (Race::Enum*) upgradesDat[9].address;
  static u32            BWXFN_StringTableOff = 0x006D1218;
  static u16**          BWXFN_StringTableIndex = (u16**) BWXFN_StringTableOff;
  static char**         BWXFN_StringTable = (char**) BWXFN_StringTableOff;
  static u32**          BWXFN_MapFogOfWar = (u32**) 0x006D1240;

  struct UpgradeProgress
  {
    Util::BitMask<u64> player[PLAYER_COUNT];
  };
  static UpgradeProgress* BWXFN_UpgradeProgress = (UpgradeProgress*) 0x0058F3C0;
  static u32 BWXFN_ResearchProgress = 0x0058F210; 

  //------------------------------------------------ SUPPLIES ------------------------------------------------
  struct Supplies
  {
    /** Direct mapping of available/Used/Max supplies for one race and all players */
    struct SuppliesPerRace
    {
      /** Direct mapping of some supply vale for all players*/
      struct SupplyValues
      {
        s32 player[PLAYER_COUNT];
      };
      SupplyValues available;
      SupplyValues used;
      SupplyValues max;
    };
    SuppliesPerRace race[RACE_COUNT];
  };
  static Supplies* BWXFN_Supplies    = (Supplies*) 0x00582124;
  
  
  //------------------------------------------ UNIT MINERAL PRICES -------------------------------------------
  /** Direct mapping of unit types mineral prices. */
  struct MineralPrices_type
  {
    u16 mineralPrice[UNIT_TYPE_COUNT];
  };
  static MineralPrices_type* BWXFN_MineralPrices = (MineralPrices_type*) unitsDat[40].address;

  //------------------------------------------ UNIT MINERAL PRICES -------------------------------------------
  /** Direct mapping of unit types gas prices. */
  struct GasPrices_type
  {
    u16 gasPrice[UNIT_TYPE_COUNT];
  };
  static GasPrices_type* BWXFN_GasPrices = (GasPrices_type*) unitsDat[41].address;
  //------------------------------------------ UNIT SUPPLY DEMANDS -------------------------------------------
  /** Direct mapping of unit supply demands. */
  struct SupplyDemands_type
  {
    u8 supplyDemand[UNIT_TYPE_COUNT];
  };
  static SupplyDemands_type* BWXFN_SupplyDemands = (SupplyDemands_type*) unitsDat[46].address;
  
  //------------------------------------------ UNIT SUPPLY PRODUCED ------------------------------------------
  /** Direct mapping of unit supply production. */
  struct SupplyProduced_type
  {
    u8 unitType[UNIT_TYPE_COUNT];
  };
  static SupplyProduced_type* BWXFN_SupplyProduced = (SupplyProduced_type*) unitsDat[45].address;

  //---------------------------------------------- UNIT MAX HP -----------------------------------------------
  /** Direct mapping of unit unit type (Max Health Points)/(Not Attackable)/(Requirable) specification. */
  struct MaxHealthPoints_NotAttackable_Repairable_type
  {
    /** mapping of the Max Health Points)/(Not Attackable)/(Requirable) for single unit type. */
    struct MaxHealthPoints_NotAttackable_Repairable_Internal_type
    {
      u16 maxHealthPoints;
      u8 notAttackable;
      u8 repairable;
    };
    MaxHealthPoints_NotAttackable_Repairable_Internal_type raw[UNIT_TYPE_COUNT];
  };
  static MaxHealthPoints_NotAttackable_Repairable_type* BWXFN_MaxHealthPoints_NotAttackable_Repairable = (MaxHealthPoints_NotAttackable_Repairable_type*) unitsDat[8].address;
  //-------------------------------------------- UNIT MAX SHIELDS --------------------------------------------
  /** Direct mapping of unit unit type Shield points */
  struct MaxShieldPoints_type
  {
    u16 maxShieldPoints[UNIT_TYPE_COUNT];
  };
  static MaxShieldPoints_type* BWXFN_MaxShieldPoints = (MaxShieldPoints_type*) unitsDat[7].address;
   //------------------------------------------- UNIT MAX SHIELDS --------------------------------------------
  /** Direct mapping of unit type armor */
  struct Armor_type
  {
    u8 armor[UNIT_TYPE_COUNT];
  };
  static Armor_type* BWXFN_Armor = (Armor_type*) unitsDat[27].address;

    //----------------------------------------- UNIT MAX BUILD TIME ------------------------------------------
  /** Direct mapping of unit build time*/
  struct BuildTime_type
  {
   u16 buildTime[UNIT_TYPE_COUNT];
  };
  static BuildTime_type* BWXFN_BuildTime = (BuildTime_type*) unitsDat[42].address;

  //-------------------------------------------- UNIT DIRECTIONS ---------------------------------------------
  /** Direct mapping of unit unit type armor */
  struct UnitsDimensions_type
  {
    /** Dimensions of unit, it's the distance from the 'center' of unit to each border */
    struct UnitDimensions
    {
      u16 left;
      u16 up;
      u16 right;
      u16 down;
    };
    UnitDimensions units[UNIT_TYPE_COUNT];
  };
  static UnitsDimensions_type* BWXFN_UnitDimensions = (UnitsDimensions_type*) unitsDat[38].address;
  const u8 NoWeapon = 130;
  //-------------------------------------------- GROUND WEAPONS  ---------------------------------------------
  /** Direct mapping of unit unit weapon type */
  struct UnitsGroundWeapon_type
  {
    u8 unit[UNIT_TYPE_COUNT];
  };
  static UnitsGroundWeapon_type* BWXFN_UnitGroundWeapon = (UnitsGroundWeapon_type*) unitsDat[17].address;
  
  //-------------------------------------------- GROUND WEAPONS  ---------------------------------------------
  /** Direct mapping of unit unit sight range */
  struct UnitsSightRange_type
  {
    u8 unit[UNIT_TYPE_COUNT];
  };
  static UnitsSightRange_type* BWXFN_UnitSightRange = (UnitsSightRange_type*) unitsDat[24].address;
  
  //-------------------------------------------- GROUND WEAPONS  ---------------------------------------------
  /** Direct mapping of unit unit sight range */
  struct UnitsSeekRange_type
  {
    u8 unit[UNIT_TYPE_COUNT];
  };
  static UnitsSeekRange_type* BWXFN_UnitSeekRange = (UnitsSeekRange_type*) unitsDat[23].address;
  
  const int weaponTypeCount = 130;
  //------------------------------------------ WEAPON DAMAGE FACTOR ------------------------------------------
  /** Direct mapping of weapon type damage factor*/
  struct WeaponsDamageFactor_type
  {
    u8 weapon[weaponTypeCount];
  };
  static WeaponsDamageFactor_type* BWXFN_WeaponDamageFactor = (WeaponsDamageFactor_type*) weaponsDat[17].address;

  //------------------------------------------- WEAPON DAMAGE TYPE -------------------------------------------
  /** Direct mapping of weapon type damage */
  struct WeaponsDamage_type
  {
    u16 weapon[weaponTypeCount];
  };
  static WeaponsDamage_type* BWXFN_WeaponDamage = (WeaponsDamage_type*) weaponsDat[14].address;
  
  //------------------------------------------- WEAPON RANGE TYPE --------------------------------------------
  /** Direct mapping of unit unit type armor */
  struct WeaponsRange_type
  {
    u32 weapon[weaponTypeCount];
  };
  static WeaponsRange_type* BWXFN_WeaponRange = (WeaponsRange_type*) weaponsDat[5].address;

  //------------------------------------------------- FLAGS --------------------------------------------------
  /** Direct mapping of unit flags data */
  struct PrototypeFlags_type
  {
    Util::BitMask<UnitPrototypeFlags::Enum> unit[UNIT_TYPE_COUNT];
  };
  static PrototypeFlags_type* BWXFN_UnitPrototypeFlags = (PrototypeFlags_type*) unitsDat[22].address;

  //---------------------------------------------- GROUP FLAGS -----------------------------------------------
  /** Direct mapping of unit flags data */
  struct PrototypeGroupFlags_type
  {
    Util::BitMask<GroupFlags::Enum> unit[UNIT_TYPE_COUNT];
  };
  static PrototypeGroupFlags_type* BWXFN_PrototypeGroupFlags = (PrototypeGroupFlags_type*) unitsDat[44].address;
  //------------------------------------------- TECH MINERAL COST --------------------------------------------
  struct TechCost
  {
    u16 tech[TECH_TYPE_COUNT];
  };
  static TechCost* BWXFN_TechMineralCost = (TechCost*) techdataDat[0].address;
  static TechCost* BWXFN_TechGasCost = (TechCost*) techdataDat[1].address;
  static TechCost* BWXFN_TechEnergyCost = (TechCost*) techdataDat[3].address;
  //------------------------------------------------ MAPPING -------------------------------------------------
  const u16 tileTypeCount = 65535;
  /** Direct mapping of minitile flags array */
  struct MiniTileMaps_type
  {
    struct MiniTileFlagArray
    {
      Util::BitMask<MiniTileFlags::Enum> miniTile[16];
    };
    MiniTileFlagArray tile[tileTypeCount];
  };

  static MiniTileMaps_type** BWXFN_MiniTileFlags = (MiniTileMaps_type**) 0x005993B0;
};
#pragma pack()
