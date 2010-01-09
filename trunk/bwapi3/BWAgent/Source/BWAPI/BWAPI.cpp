#include "BWAPI.h"
#include "BridgeClient.h"

#include <Util\Version.h>
#include <Util\Types.h>
#include <Util\HandleFactory.h>

#include <Bridge\SharedStuff.h>
#include <Bridge\EventEntry.h>
#include <Bridge\CommandEntry.h>

#include <BWAPIDatabase\UnitTypes.h>
#include <BWAPIDatabase\TechTypes.h>
#include <BWAPIDatabase\UpgradeTypes.h>
#include <BWAPIDatabase\WeaponTypes.h>
#include <BWAPIDatabase\DamageTypes.h>
#include <BWAPIDatabase\ExplosionTypes.h>
#include <BWAPIDatabase\Races.h>
#include <BWAPIDatabase\UnitSizeTypes.h>
#include <BWAPIDatabase\PlayerTypes.h>

namespace BWAPI
{
//private:
//public:
  //----------------------------------- GET VERSION -----------------------------------------------
  BWAPI_FUNCTION int getVersion()
  {
    return SVN_REV;
  }
  //----------------------------------- CONNECT ---------------------------------------------------
  BWAPI_FUNCTION bool connect() throw()
  {
    return BridgeClient::connect();
  }
  //----------------------------------- WAIT FOR EVENT --------------------------------------------
  BWAPI_FUNCTION CallTypeId waitForEvent()
  {
    while(true)
    {
      BridgeClient::waitForEvent();

      // react upon bridge state
      BridgeClient::RpcState rpcState = BridgeClient::getCurrentRpc();

      switch(rpcState)
      {
      case BridgeClient::OnInitMatch:
        {
          // init database
          UnitTypes::init();
          TechTypes::init();
          UpgradeTypes::init();
          WeaponTypes::init();
          DamageTypes::init();
          ExplosionTypes::init();
          Races::init();
          UnitSizeTypes::init();

        }return CallTypeIds::OnMatchStart;
      case BridgeClient::OnFrame:
        {
        }return CallTypeIds::OnFrame;
      }
    }
  }
  //----------------------------------- DRAW TEXT -------------------------------------------------
  BWAPI_FUNCTION bool printText(const char* text)
  {
    return BridgeClient::pushSendText(false, text);
  }
  //----------------------------------- DRAW TEXT -------------------------------------------------
  BWAPI_FUNCTION bool sendText(const char* text)
  {
    return BridgeClient::pushSendText(true, text);
  }
  //----------------------------------- DRAW TEXT -------------------------------------------------
  BWAPI_FUNCTION void drawText(Position pos, const char* text)
  {
    BridgeClient::pushDrawText(pos - BridgeClient::gameData->screenPosition, text);
  }
  BWAPI_FUNCTION void drawTextScreen(Position pos, const char* text)
  {
    BridgeClient::pushDrawText(pos, text);
  }
  //----------------------------------- DRAW RECTANGLE --------------------------------------------
  BWAPI_FUNCTION void drawRectangle(Position pos, Position size, int color, int solid)
  {
    BridgeClient::pushDrawRectangle(pos - BridgeClient::gameData->screenPosition, size - BridgeClient::gameData->screenPosition, color, !!solid);
  }
  BWAPI_FUNCTION void drawRectangleScreen(Position pos, Position size, int color, int solid)
  {
    BridgeClient::pushDrawRectangle(pos, size, color, !!solid);
  }
  //----------------------------------- DRAW CIRCLE -----------------------------------------------
  BWAPI_FUNCTION void drawCircle(Position pos, int r, int color, bool solid)
  {
    BridgeClient::pushDrawCircle(pos - BridgeClient::gameData->screenPosition, r, color, !!solid);
  }
  BWAPI_FUNCTION void drawCircleScreen(Position pos, int r, int color, bool solid)
  {
    BridgeClient::pushDrawCircle(pos, r, color, !!solid);
  }
  //----------------------------------- DRAW ELLIPSE ----------------------------------------------
  BWAPI_FUNCTION void drawEllipse         (Position pos, int w, int h, int color, bool solid)
  {
    BridgeClient::pushDrawEllipse(pos - BridgeClient::gameData->screenPosition, w, h, color, !!solid);
  }
  BWAPI_FUNCTION void drawEllipseScreen   (Position pos, int w, int h, int color, bool solid)
  {
    BridgeClient::pushDrawEllipse(pos, w, h, color, !!solid);
  }
  //----------------------------------- DRAW LINE -------------------------------------------------
  BWAPI_FUNCTION void drawLine(Position pos, Position pos2, int color)
  {
    BridgeClient::pushDrawLine(pos - BridgeClient::gameData->screenPosition, pos2 - BridgeClient::gameData->screenPosition, color);
  }
  BWAPI_FUNCTION void drawLineScreen(Position pos, Position pos2, int color)
  {
    BridgeClient::pushDrawLine(pos, pos2, color);
  }
  //----------------------------------- DRAW DOT --------------------------------------------------
  BWAPI_FUNCTION void drawDot(Position pos, int color)
  {
    BridgeClient::pushDrawDot(pos - BridgeClient::gameData->screenPosition, color);
  }
  BWAPI_FUNCTION void drawDotScreen(Position pos, int color)
  {
    BridgeClient::pushDrawDot(pos, color);
  }
  //----------------------------------- GET STATIC DATA -------------------------------------------
  BWAPI_FUNCTION const StaticGameData* getStaticGameData()
  {
    if(!BridgeClient::isConnected())
      throw GeneralException("GetStaticGameData requires connection");
    StaticGameData *retval = BridgeClient::gameData;
    return retval;
  }
  //----------------------------------- GET DYNAMIC EVENT CONTAINERS ------------------------------
  BWAPI_FUNCTION const DynamicGameData* getDynamicGameData()
  {
    if(!BridgeClient::isConnected())
      throw GeneralException("GetDynamicGameData requires connection");
    DynamicGameData *retval = &BridgeClient::dynamicData;
    return retval;
  }
  //----------------------------------- GET UNIT --------------------------------------------------
  BWAPI_FUNCTION const UnitState* getUnit(UnitId unitId)
  {
    return &BridgeClient::gameData->units.at(unitId);
  }
  //----------------------------------- GET UNIT TYPE ---------------------------------------------
  BWAPI_FUNCTION const Player* getPlayer(PlayerId playerId)
  {
    return &BridgeClient::gameData->players.at(playerId);
  }
  //----------------------------------- GET UNIT TYPE ---------------------------------------------
  BWAPI_FUNCTION const UnitType* getUnitType(UnitTypeId id)
  {
    return &UnitTypes::unitTypeData[id];
  }
  //----------------------------------- GET TECH TYPE ---------------------------------------------
  BWAPI_FUNCTION const TechType* getTechType(TechTypeId id)
  {
    return &TechTypes::techTypeData[id];
  }
  //----------------------------------- GET UPGRADE TYPE ------------------------------------------
  BWAPI_FUNCTION const UpgradeType* getUpgradeType(UpgradeTypeId id)
  {
    return &upgradeTypeData[id];
  }
  //----------------------------------- GET WEAPON TYPE -------------------------------------------
  BWAPI_FUNCTION const WeaponType* getWeaponType(WeaponTypeId id)
  {
    return &WeaponTypes::weaponTypeData[id];
  }
  //----------------------------------- GET DAMAGE TYPE -------------------------------------------
  BWAPI_FUNCTION const DamageType* getDamageType(DamageTypeId id)
  {
    return &DamageTypes::damageTypeData[id];
  }
  //----------------------------------- GET EXPLOSION TYPE ----------------------------------------
  BWAPI_FUNCTION const ExplosionType* getExplosionType(ExplosionTypeId id)
  {
    return &ExplosionTypes::explosionTypeData[id];
  }
  //----------------------------------- GET RACE --------------------------------------------------
  BWAPI_FUNCTION const Race* getRace(RaceId id)
  {
      return &Races::raceData[id];
  }
  //----------------------------------- GET UNIT SIZE TYPE ----------------------------------------
  BWAPI_FUNCTION const UnitSizeType* getUnitSizeType(UnitSizeTypeId id)
  {
    return &UnitSizeTypes::unitSizeTypeData[id];
  }
  //----------------------------------- GET PLAYER TYPE -------------------------------------------
  BWAPI_FUNCTION const PlayerType* getPlayerType(PlayerTypeId id)
  {
    return &PlayerTypes::playerTypeData[id];
  }
  //----------------------------------- -----------------------------------------------------------
  //----------------------------------- INSERT COMMAND --------------------------------------------
  template<typename T>
    void pushCommand(const T& cmd)
    {
      Bridge::SharedStuff::CommandStack::Index i = BridgeClient::sharedStuff.commands.insert(Util::MemoryFrame::from(cmd));
    }
  //----------------------------------- COMMANDS --------------------------------------------------
  BWAPI_FUNCTION void startGame()
  {
    pushCommand(Bridge::CommandEntry::StartGame());
  }
  BWAPI_FUNCTION void pauseGame()
  {
    pushCommand(Bridge::CommandEntry::PauseGame());
  }
  BWAPI_FUNCTION void leaveGame()
  {
    pushCommand(Bridge::CommandEntry::LeaveGame());
  }
  BWAPI_FUNCTION void resumeGame()
  {
    pushCommand(Bridge::CommandEntry::ResumeGame());
  }
  BWAPI_FUNCTION void restartGame()
  {
    pushCommand(Bridge::CommandEntry::RestartGame());
  }
  BWAPI_FUNCTION void setLocalSpeed(int speed)
  {
    Bridge::CommandEntry::SetLocalSpeed cmd;
    cmd.speed = speed;
    pushCommand(cmd);
  }
  BWAPI_FUNCTION void pingMinimap(Position pos)
  {
    Bridge::CommandEntry::PingMinimap cmd;
    cmd.pos = pos;
    pushCommand(cmd);
  }
  BWAPI_FUNCTION void enableFlag(Flag flag)
  {
    Bridge::CommandEntry::EnableFlag cmd;
    cmd.flag = flag;
    pushCommand(cmd);
  }
  //----------------------------------- INSERT ORDER ----------------------------------------------
  UnitCommand& insertOrder()
  {
    Bridge::CommandEntry::UnitOrder entry;
    Bridge::SharedStuff::CommandStack::Index i = BridgeClient::sharedStuff.commands.insert(Util::MemoryFrame::from(entry));

    static UnitCommand safeSpot;
    if(!i.isValid())
      return safeSpot;

    return BridgeClient::sharedStuff.commands.get(i).getAs<Bridge::CommandEntry::UnitOrder>().unitCommand;
  }
  //----------------------------------- ATTACK POSITION ORDER -------------------------------------
  BWAPI_FUNCTION void orderAttackPosition(int unitId, int x, int y)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::AttackPosition;
    order.unitIndex = unitId;
    order.x = x;
    order.y = y;
  }
  //----------------------------------- ATTACK UNIT ORDER -----------------------------------------
  BWAPI_FUNCTION void orderAttackUnit(int unitId, int targetUnitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::AttackUnit;
    order.unitIndex = unitId;
    order.targetIndex = targetUnitId;
  }
  //----------------------------------- RIGHT CLICK POSITION ORDER --------------------------------
  BWAPI_FUNCTION void orderRightClickPosition(int unitId, int x, int y)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::RightClickPosition;
    order.unitIndex = unitId;
    order.x = x;
    order.y = y;
  }
  //----------------------------------- RIGHT CLICK UNIT ORDER ------------------------------------
  BWAPI_FUNCTION void orderRightClickUnit(int unitId, int targetUnitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::RightClickUnit;
    order.unitIndex = unitId;
    order.targetIndex = targetUnitId;
  }
  //----------------------------------- TRAIN ORDER -----------------------------------------------
  BWAPI_FUNCTION void orderTrain(int unitId, UnitTypeId what)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Train;
    order.unitIndex = unitId;
    order.unitType = what;
  }
  //----------------------------------- BUILD ORDER -----------------------------------------------
  BWAPI_FUNCTION void orderBuild(int unitId, UnitTypeId what)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Build;
    order.unitIndex = unitId;
    order.unitType = what;
  }
  //----------------------------------- BUILD ADDON ORDER -----------------------------------------
  BWAPI_FUNCTION void orderBuildAddon(int unitId, UnitTypeId what)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::BuildAddon;
    order.unitIndex = unitId;
    order.unitType = what;
  }
  //----------------------------------- RESEARCH ORDER --------------------------------------------
  BWAPI_FUNCTION void orderResearch(int unitId, TechTypeId what)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Research;
    order.unitIndex = unitId;
    order.tech = what;
  }
  //----------------------------------- UPGRADE ORDER ---------------------------------------------
  BWAPI_FUNCTION void orderUpgrade(int unitId, UpgradeTypeId what)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Upgrade;
    order.unitIndex = unitId;
    order.upgrade = what;
  }
  //----------------------------------- STOP ORDER ------------------------------------------------
  BWAPI_FUNCTION void orderStop(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Stop;
    order.unitIndex = unitId;
  }
  //----------------------------------- HOLD POSITION ORDER ---------------------------------------
  BWAPI_FUNCTION void orderHoldPosition(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::HoldPosition;
    order.unitIndex = unitId;
  }
  //----------------------------------- PATROL ORDER ----------------------------------------------
  BWAPI_FUNCTION void orderPatrol(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Patrol;
    order.unitIndex = unitId;
  }
  //----------------------------------- FOLLOW ORDER ----------------------------------------------
  BWAPI_FUNCTION void orderFollow(int unitId, int targetUnitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Follow;
    order.unitIndex = unitId;
  }
  //----------------------------------- SET RALLY POSITION ORDER ----------------------------------
  BWAPI_FUNCTION void orderSetRallyPosition(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::SetRallyPosition;
    order.unitIndex = unitId;
  }
  //----------------------------------- SET RALLY UNIT ORDER --------------------------------------
  BWAPI_FUNCTION void orderSetRallyUnit(int unitId, int targetUnitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::SetRallyUnit;
    order.unitIndex = unitId;
  }
  //----------------------------------- REPAIR ORDER ----------------------------------------------
  BWAPI_FUNCTION void orderRepair(int unitId, int targetUnitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Repair;
    order.unitIndex = unitId;
    order.targetIndex = targetUnitId;
  }
  //----------------------------------- RETURN CARGO ORDER ----------------------------------------
  BWAPI_FUNCTION void orderReturnCargo(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::ReturnCargo;
    order.unitIndex = unitId;
  }
  //----------------------------------- MORPH ORDER -----------------------------------------------
  BWAPI_FUNCTION void orderMorph(int unitId, UnitTypeId what)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Morph;
    order.unitIndex = unitId;
    order.unitType = what;
  }
  //----------------------------------- BURROW ORDER ----------------------------------------------
  BWAPI_FUNCTION void orderBurrow(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Burrow;
    order.unitIndex = unitId;
  }
  //----------------------------------- UNBURROW ORDER --------------------------------------------
  BWAPI_FUNCTION void orderUnburrow(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Unburrow;
    order.unitIndex = unitId;
  }
  //----------------------------------- SIEGE ORDER -----------------------------------------------
  BWAPI_FUNCTION void orderSiege(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Siege;
    order.unitIndex = unitId;
  }
  //----------------------------------- UNSIEGE ORDER ---------------------------------------------
  BWAPI_FUNCTION void orderUnsiege(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Unsiege;
    order.unitIndex = unitId;
  }
  //----------------------------------- CLOAK ORDER -----------------------------------------------
  BWAPI_FUNCTION void orderCloak(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Cloak;
    order.unitIndex = unitId;
  }
  //----------------------------------- DECLOAK ORDER ---------------------------------------------
  BWAPI_FUNCTION void orderDecloak(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Decloak;
    order.unitIndex = unitId;
  }
  //----------------------------------- LIFT ORDER ------------------------------------------------
  BWAPI_FUNCTION void orderLift(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Lift;
    order.unitIndex = unitId;
  }
  //----------------------------------- LAND ORDER ------------------------------------------------
  BWAPI_FUNCTION void orderLand(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Land;
    order.unitIndex = unitId;
  }
  //----------------------------------- LOAD ORDER ------------------------------------------------
  BWAPI_FUNCTION void orderLoad(int unitId, int targetUnitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Load;
    order.unitIndex = unitId;
    order.targetIndex = targetUnitId;
  }
  //----------------------------------- UNLOAD ORDER ----------------------------------------------
  BWAPI_FUNCTION void orderUnload(int unitId, int targetUnitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::Unload;
    order.unitIndex = unitId;
    order.targetIndex = targetUnitId;
  }
  //----------------------------------- UNLOAD ALL ORDER ------------------------------------------
  BWAPI_FUNCTION void orderUnloadAll(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::UnloadAll;
    order.unitIndex = unitId;
  }
  //----------------------------------- UNLOAD ALL POSITION ORDER ---------------------------------
  BWAPI_FUNCTION void orderUnloadAllPosition(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::UnloadAllPosition;
    order.unitIndex = unitId;
  }
  //----------------------------------- CANCEL CONSTRUCTION ORDER ---------------------------------
  BWAPI_FUNCTION void orderCancelConstruction(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::CancelConstruction;
    order.unitIndex = unitId;
  }
  //----------------------------------- HALT CONSTRUCTION ORDER -----------------------------------
  BWAPI_FUNCTION void orderHaltConstruction(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::HaltConstruction;
    order.unitIndex = unitId;
  }
  //----------------------------------- CANCEL MORPH ORDER ----------------------------------------
  BWAPI_FUNCTION void orderCancelMorph(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::CancelMorph;
    order.unitIndex = unitId;
  }
  //----------------------------------- CANCEL TRAIN ORDER ----------------------------------------
  BWAPI_FUNCTION void orderCancelTrain(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::CancelTrain;
    order.unitIndex = unitId;
  }
  //----------------------------------- CANCEL TRAIN SLOT ORDER -----------------------------------
  BWAPI_FUNCTION void orderCancelTrainSlot(int unitId, int slotId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::CancelTrainSlot;
    order.unitIndex = unitId;
    order.extra = slotId;
  }
  //----------------------------------- CANCEL ADDON ORDER ----------------------------------------
  BWAPI_FUNCTION void orderCancelAddon(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::CancelAddon;
    order.unitIndex = unitId;
  }
  //----------------------------------- CANCEL RESEARCH ORDER -------------------------------------
  BWAPI_FUNCTION void orderCancelResearch(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::CancelResearch;
    order.unitIndex = unitId;
  }
  //----------------------------------- CANCEL UPGRADE ORDER --------------------------------------
  BWAPI_FUNCTION void orderCancelUpgrade(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::CancelUpgrade;
    order.unitIndex = unitId;
  }
  //----------------------------------- USE TECH ORDER --------------------------------------------
  BWAPI_FUNCTION void orderUseTech(int unitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::UseTech;
    order.unitIndex = unitId;

  }
  //----------------------------------- USE TECH POSITION ORDER -----------------------------------
  BWAPI_FUNCTION void orderUseTechPosition(int unitId, TechTypeId what)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::UseTechPosition;
    order.unitIndex = unitId;
  }
  //----------------------------------- USE TECH UNIT ORDER ---------------------------------------
  BWAPI_FUNCTION void orderUseTechUnit(int unitId, TechTypeId what, int targetUnitId)
  {
    UnitCommand &order = insertOrder();
    order.commandId = UnitCommandTypeIds::UseTechUnit;
    order.unitIndex = unitId;
    order.targetIndex = targetUnitId;
  }
  //----------------------------------- -----------------------------------------------------------
}
