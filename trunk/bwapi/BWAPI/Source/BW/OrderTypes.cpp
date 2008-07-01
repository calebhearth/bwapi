#include "OrderTypes.h"
#include "..\\BWAPI\\Unit.h"
#include "Offsets.h"
namespace BW
{
  namespace Orders
  {

    //-------------------------------- RIGHT CLICK COONSTRUCTOR -----------------------
    RightClick::RightClick(BWAPI::Unit *target)
    :always0x14(0x14)
    ,target(target)
    ,always0xe4(0xe4)
    ,alwaysZero(0x0)
    {
    }
    //-------------------------------- RIGHT CLICK COONSTRUCTOR -----------------------
    RightClick::RightClick(const BW::Position& target)
    :always0x14(0x14)
    ,target(target)
    ,always0xe4(0xe4)
    ,alwaysZero(0x0)
    {
    }
    //-------------------------------- RIGHT CLICK COONSTRUCTOR -----------------------
    RightClick::RightClick(const PositionUnitTarget& target)
    :always0x14(0x14)
    ,target(target)
    ,always0xe4(0xe4)
    ,alwaysZero(0x0)
    {
    }
    //------------------------------- SELECT SINGLE CONSTRUCTOR -----------------------
    SelectSingle::SelectSingle(BWAPI::Unit *select)
    :always0x09(0x09)
    ,always0x01(0x01)
    ,target(select)
    {
    }
    //--------------------------------- TRAIN UNIT CONSTRUCTOR ------------------------
    TrainUnit::TrainUnit(BW::UnitType type)
    :always0x1f(0x1f)
    ,type(type)
    {
    }
    //--------------------------------- MAKE BULDING ----------------------------------
    MakeBuilding::MakeBuilding(BW::TilePosition position, BW::UnitType type)
    :always0x0c(0x0c)
    ,always0x1e(0x1e)
    ,position(position)
    ,type(type)
    {
    }
    //--------------------------------- INVENT TECH -----------------------------------
    Invent::Invent(BW::TechType tech)
    :always0x30(0x30)
    ,tech(tech)
    {
    }
    //----------------------------------- UPGRADE -------------------------------------
    Upgrade::Upgrade(BW::UpgradeType upgrade)
    :always0x32(0x32)
    ,upgrade(upgrade)
    {
    }    
    //--------------------------------- MAKE ADDON ------------------------------------
    MakeAddon::MakeAddon(BW::TilePosition position, BW::UnitType type)
    :always0x0c(0x0c)
    ,always0x24(0x24)
    ,position(position)
    ,type(type)
    {
    }    
    //---------------------------------- MOVE CONSTRUCTOR -----------------------------
    ChangeSlot::ChangeSlot(Slot slot, u8 slotID)
    :slot(slot)
    ,slotID(slotID)
    ,always0x44(0x44)
    {
    }
    //-------------------------------- CHANGE RACE CONSTRUCTOR ------------------------
    ChangeRace::ChangeRace(BW::Race::Enum race, u8 slotID)
    :race(race)
    ,slotID(slotID)
    ,always0x41(0x41)
    {
    }
    //------------------------------- START GAME CONSTRUCTOR --------------------------
    StartGame::StartGame()
    :always0x3c(0x3c)
    {
    }
    //---------------------------------------------------------------------------------        
  }
}