#include <BWAPI.h>
#include <GameImpl.h>
#include <TemplatesImpl.h>

namespace BWAPI
{
	//--------------------------------------------- GET MOUSE STATE --------------------------------------------
	bool GameImpl::getMouseState(MouseButton button)
	{
		return getMouseState((int)button);
	}
	//---------------------------------------------- GET KEY STATE ---------------------------------------------
	bool GameImpl::getKeyState(Key key)
	{
		return getKeyState((int)key);
	}
	//----------------------------------------------- GET FORCES -----------------------------------------------
	Forceset& GameImpl::getForces()
	{
		return forces;
	}
	//----------------------------------------------- GET PLAYERS ----------------------------------------------
	Playerset& GameImpl::getPlayers()
	{
		return playerSet;
	}
	//------------------------------------------------- GET UNITS ----------------------------------------------
	Unitset& GameImpl::getAllUnits()
	{
		return accessibleUnits;
	}
	//------------------------------------------------- GET MINERALS -------------------------------------------
	Unitset& GameImpl::getMinerals()
	{
		return minerals;
	}
	//------------------------------------------------- GET GEYSERS --------------------------------------------
	Unitset& GameImpl::getGeysers()
	{
		return geysers;
	}
	//------------------------------------------------- GET NEUTRAL UNITS --------------------------------------
	Unitset& GameImpl::getNeutralUnits()
	{
		return neutralUnits;
	}
	//------------------------------------------------- GET STATIC MINERALS ------------------------------------
	Unitset& GameImpl::getStaticMinerals()
	{
		return staticMinerals;
	}
	//------------------------------------------------- GET STATIC GEYSERS -------------------------------------
	Unitset& GameImpl::getStaticGeysers()
	{
		return staticGeysers;
	}
	//------------------------------------------------- GET STATIC NEUTRAL UNITS -------------------------------
	Unitset& GameImpl::getStaticNeutralUnits()
	{
		return staticNeutralUnits;
	}
	//------------------------------------------------ GET BULLETS ---------------------------------------------
	Bulletset& GameImpl::getBullets()
	{
		return bullets;
	}
	//------------------------------------------------ GET NUKE DOTS -------------------------------------------
	std::set< Position >& GameImpl::getNukeDots()
	{
		return nukeDots;
	}
	//------------------------------------------------ GET EVENTS ----------------------------------------------
	std::list< Event >& GameImpl::getEvents()
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
	//--------------------------------------------- IS BUILDABLE -----------------------------------------------
	bool GameImpl::isBuildable(TilePosition position, bool includeBuildings)
	{
		return isBuildable(position.x, position.y, includeBuildings);
	}
	//--------------------------------------------- IS VISIBLE -------------------------------------------------
	bool GameImpl::isVisible(TilePosition position)
	{
		return isVisible(position.x,position.y);
	}
	//--------------------------------------------- IS EXPLORED ------------------------------------------------
	bool GameImpl::isExplored(TilePosition position)
	{
		return isExplored(position.x,position.y);
	}
	//--------------------------------------------- HAS CREEP --------------------------------------------------
	bool GameImpl::hasCreep(TilePosition position)
	{
		return hasCreep(position.x, position.y);
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
	std::set< TilePosition >& GameImpl::getStartLocations()
	{
		return startLocations;
	}
	BWAPI::Region *GameImpl::getRegionAt(BWAPI::Position position) const
	{
		if ( !position )
		{
			Broodwar->setLastError(BWAPI::Errors::Invalid_Parameter);
			return NULL;
		}
		return getRegionAt(position.x, position.y);
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

