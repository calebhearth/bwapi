#include <BWAPI.h>
#include <GameImpl.h>
#include <TemplatesImpl.h>

namespace BWAPI
{
  //--------------------------------------------- GET MOUSE STATE --------------------------------------------
  bool GameImpl::getMouseState(MouseButton button) const
  {
    return getMouseState((int)button);
  }
  //---------------------------------------------- GET KEY STATE ---------------------------------------------
  bool GameImpl::getKeyState(Key key) const
  {
    return getKeyState((int)key);
  }
  //----------------------------------------------- GET FORCES -----------------------------------------------
  const Forceset& GameImpl::getForces() const
  {
    return forces;
  }
  //----------------------------------------------- GET PLAYERS ----------------------------------------------
  const Playerset& GameImpl::getPlayers() const
  {
    return playerSet;
  }
  //------------------------------------------------- GET UNITS ----------------------------------------------
  const Unitset& GameImpl::getAllUnits() const
  {
    return accessibleUnits;
  }
  //------------------------------------------------- GET MINERALS -------------------------------------------
  const Unitset& GameImpl::getMinerals() const
  {
    return minerals;
  }
  //------------------------------------------------- GET GEYSERS --------------------------------------------
  const Unitset& GameImpl::getGeysers() const
  {
    return geysers;
  }
  //------------------------------------------------- GET NEUTRAL UNITS --------------------------------------
  const Unitset& GameImpl::getNeutralUnits() const
  {
    return neutralUnits;
  }
  //------------------------------------------------- GET STATIC MINERALS ------------------------------------
  const Unitset& GameImpl::getStaticMinerals() const
  {
    return staticMinerals;
  }
  //------------------------------------------------- GET STATIC GEYSERS -------------------------------------
  const Unitset& GameImpl::getStaticGeysers() const
  {
    return staticGeysers;
  }
  //------------------------------------------------- GET STATIC NEUTRAL UNITS -------------------------------
  const Unitset& GameImpl::getStaticNeutralUnits() const
  {
    return staticNeutralUnits;
  }
  //------------------------------------------------ GET BULLETS ---------------------------------------------
  const Bulletset& GameImpl::getBullets() const
  {
    return bullets;
  }
  //------------------------------------------------ GET NUKE DOTS -------------------------------------------
  const std::set< Position >& GameImpl::getNukeDots() const
  {
    return nukeDots;
  }
  //------------------------------------------------ GET EVENTS ----------------------------------------------
  const std::list< Event >& GameImpl::getEvents() const
  {
    return events;
  }
  //----------------------------------------------- GET LAST ERROR -------------------------------------------
  Error GameImpl::getLastError() const
  {
    /* returns the last error encountered in BWAPI */
    return lastError;
  }
  //--------------------------------------------- SET LAST ERROR ---------------------------------------------
  bool GameImpl::setLastError(BWAPI::Error e)
  {
    /* implies that an error has occured */
    lastError = e;
    return e == Errors::None;
  }
  //--------------------------------------------- HAS POWER --------------------------------------------------
  bool GameImpl::hasPower(int tileX, int tileY, UnitType unitType) const
  {
    if ( unitType >= 0 && unitType < UnitTypes::None )
      return hasPowerPrecise( tileX*32 + unitType.tileWidth()*16, tileY*32 + unitType.tileHeight()*16, unitType);
    return hasPowerPrecise( tileX*32, tileY*32, UnitTypes::None);
  }
  bool GameImpl::hasPower(TilePosition position, UnitType unitType) const
  {
    return hasPower(position.x, position.y, unitType);
  }
  bool GameImpl::hasPower(int tileX, int tileY, int tileWidth, int tileHeight, UnitType unitType) const
  {
    return hasPowerPrecise( tileX*32 + tileWidth*16, tileY*32 + tileHeight*16, unitType);
  }
  bool GameImpl::hasPower(TilePosition position, int tileWidth, int tileHeight, UnitType unitType) const
  {
    return hasPower(position.x, position.y, tileWidth, tileHeight, unitType);
  }
  bool GameImpl::hasPowerPrecise(Position position, UnitType unitType) const
  {
    return hasPowerPrecise(position.x, position.y, unitType);
  }
  //--------------------------------------------- CAN BUILD HERE ---------------------------------------------
  bool GameImpl::canBuildHere(TilePosition position, UnitType type, const Unit* builder, bool checkExplored)
  {
    return Templates::canBuildHere(builder,position,type,checkExplored);
  }
  //--------------------------------------------- CAN MAKE ---------------------------------------------------
  bool GameImpl::canMake(UnitType type, const Unit* builder)
  {
    return Templates::canMake(builder,type);
  }
  //--------------------------------------------- CAN RESEARCH -----------------------------------------------
  bool GameImpl::canResearch(TechType type, const Unit* unit)
  {
    return Templates::canResearch(unit,type);
  }
  //--------------------------------------------- CAN UPGRADE ------------------------------------------------
  bool GameImpl::canUpgrade(UpgradeType type, const Unit* unit)
  {
    return Templates::canUpgrade(unit,type);
  }
  //--------------------------------------------- GET START LOCATIONS ----------------------------------------
  const std::set< TilePosition >& GameImpl::getStartLocations() const
  {
    return startLocations;
  }
  //----------------------------------------------- GET ALL REGIONS ------------------------------------------
  const Regionset &GameImpl::getAllRegions() const
  {
    return this->regionsList;
  }

  const GameData* GameImpl::getGameData() const
  {
    return data;
  }
};

