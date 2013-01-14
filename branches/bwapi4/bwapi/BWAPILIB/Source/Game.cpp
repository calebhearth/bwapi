#include <BWAPI/Game.h>
#include <BWAPI/Position.h>
#include <BWAPI/CoordinateType.h>
#include <BWAPI/Color.h>
#include <BWAPI/Unitset.h>
#include <BWAPI/Unit.h>
#include <BWAPI/Filters.h>
#include <BWAPI/Player.h>

#include <cstdarg>

namespace BWAPI
{
  GameWrapper Broodwar;
  Game *BroodwarPtr;
  //-------------------------------------- BUILD LOCATION --------------------------------------------
#define MAX_RANGE 64
  class PlacementReserve
  {
  public:
    void reset()
    {
      memset(data,0,sizeof(data));
    };

    // Checks if the given x/y value is valid for the Placement position
    static bool isValidPos(int x, int y)
    {
      return x >= 0 && x < MAX_RANGE && y >= 0 && y < MAX_RANGE;
    };
    static bool isValidPos(TilePosition p)
    {
      return isValidPos(p.x, p.y);
    };

    // Sets the value in the placement reserve array
    void setValue(int x, int y, unsigned char value)
    {
      if ( isValidPos(x,y) )
        data[y][x] = value;
    };
    void setValue(TilePosition p, unsigned char value)
    {
      this->setValue(p.x,p.y,value);
    };

    // Gets the value from the placement reserve array, 0 if position is invalid
    unsigned char getValue(int x, int y) const
    { 
      if ( isValidPos(x,y) )
        return data[y][x];
      return 0;
    };
    unsigned char getValue(TilePosition p) const
    { 
      return this->getValue(p.x,p.y);
    };

    template <typename T>
    bool iterate(const T& proc, int startX = 0, int startY = 0, int endX = MAX_RANGE, int endY = MAX_RANGE)
    {
      for ( int y = startX; y < endX; ++y )
        for ( int x = startY; x < endY; ++x )
        {
          if ( !proc(this, x, y) )
            return false;
        }
      return true;
    };

    bool hasValidSpace()
    {
      return !iterate( [](PlacementReserve *pr, int x, int y)->bool{ return pr->getValue(x,y) == 0; } );
    };

    void backup()
    {
      memcpy(save, data, sizeof(data));
    };
    void restore()
    {
      memcpy(data, save, sizeof(data));
    };
    void restoreIfInvalid()
    {
      if ( !hasValidSpace() )
        restore();
    };
  private:
    unsigned char data[MAX_RANGE][MAX_RANGE];
    unsigned char save[MAX_RANGE][MAX_RANGE];
  };

  void AssignBuildableLocations(PlacementReserve &reserve, UnitType type, TilePosition desiredPosition, int maxRange)
  {
    // Get min/max distances
    int min = MAX_RANGE/2 - maxRange/2;
    int max = min + maxRange;

    TilePosition start = desiredPosition - TilePosition(MAX_RANGE,MAX_RANGE)/2;
    
    // Reserve space for the addon as well
    bool hasAddon = type.canBuildAddon();
    
    // Assign 1 to all buildable locations
    reserve.iterate( [&](PlacementReserve *pr, int x, int y)->bool
                      { 
                        if ( (!hasAddon || Broodwar->canBuildHere(start+TilePosition(x+4,y+1), UnitTypes::Terran_Missile_Turret) ) &&
                          Broodwar->canBuildHere(start+TilePosition(x,y), type) )
                        {
                          pr->setValue(x, y, 1);
                        }
                        return true;
                      }, min, min, max, max );
  }

  void RemoveDisconnected(PlacementReserve &reserve, UnitType type, TilePosition desiredPosition, int maxRange)
  {
    // Get min/max distances
    int min = MAX_RANGE/2 - maxRange/2;
    int max = min + maxRange;

    TilePosition start = desiredPosition - TilePosition(MAX_RANGE,MAX_RANGE)/2;

    // Assign 0 to all locations that aren't connected
    reserve.iterate( [&](PlacementReserve *pr, int x, int y)->bool
                      { 
                        if ( !Broodwar->hasPath(Position(desiredPosition), Position(start + TilePosition(x,y)) ) )
                          pr->setValue(x, y, 0);
                        return true;
                      }, min, min, max, max );
  }

  void ReserveGroundHeight(PlacementReserve &reserve, UnitType type, TilePosition desiredPosition, int maxRange)
  {
    // Get min/max distances
    int min = MAX_RANGE/2 - maxRange/2;
    int max = min + maxRange;

    TilePosition start = desiredPosition - TilePosition(MAX_RANGE,MAX_RANGE)/2;

    // Exclude locations with a different ground height, but restore a backup in case there are no more build locations
    reserve.backup();
    int targetHeight = Broodwar->getGroundHeight(desiredPosition);
    reserve.iterate( [&](PlacementReserve *pr, int x, int y)->bool
                      { 
                        if ( Broodwar->getGroundHeight( start + TilePosition(x,y) ) != targetHeight )
                          pr->setValue(x, y, 0);
                        return true;
                      }, min, min, max, max );

    // Restore original if there is nothing left
    reserve.restoreIfInvalid();
  }

  void ReserveExistingAddonPlacement(PlacementReserve &reserve, UnitType type, TilePosition desiredPosition, int maxRange)
  {
    TilePosition start = desiredPosition - TilePosition(MAX_RANGE,MAX_RANGE)/2;

    //Exclude addon placement locations
    reserve.backup();
    Unitset myUnits = Broodwar->self()->getUnits();
    myUnits.erase_if( !(Exists && CanBuildAddon) );
    for ( auto it = myUnits.begin(); it != myUnits.end(); ++it )
    {
      TilePosition addonPos = (it->getTilePosition() + TilePosition(4,1)) - start;
      reserve.iterate( [&](PlacementReserve *pr, int x, int y)->bool
                        { 
                          pr->setValue(x, y, 0);
                          return true;
                        }, addonPos.x, addonPos.y, addonPos.x+2, addonPos.y+2 );
    }

    // Restore if this gave us no build locations
    reserve.restoreIfInvalid();
  }

  void ReserveStructureWithPadding(PlacementReserve &reserve, TilePosition currentPosition, TilePosition sizeExtra, int padding, UnitType type, TilePosition desiredPosition, int maxRange)
  {
    TilePosition paddingSize = sizeExtra + TilePosition(padding,padding)*2;
  
    TilePosition topLeft = currentPosition - type.tileSize() - paddingSize/2 + TilePosition(1,1);
    TilePosition topLeftRelative = topLeft - desiredPosition + TilePosition(MAX_RANGE,MAX_RANGE)/2;
    TilePosition maxSize = type.tileSize() + paddingSize - TilePosition(1,1);

    reserve.iterate( [&](PlacementReserve *pr, int x, int y)->bool
                      { 
                        pr->setValue(x, y, 0);
                        return true;
                      }, topLeftRelative.x, topLeftRelative.y, maxSize.x, maxSize.y );
  }

  void ReserveStructure(PlacementReserve &reserve, Unit *pUnit, int padding, UnitType type, TilePosition desiredPosition, int maxRange)
  {
    ReserveStructureWithPadding(reserve, TilePosition(pUnit->getPosition()), pUnit->getType().tileSize(), padding, type, desiredPosition, maxRange);
  }

  void ReserveAllStructures(PlacementReserve &reserve, UnitType type, TilePosition desiredPosition, int maxRange)
  {
    if ( type.isAddon() )
      return;
    reserve.backup();

    // Reserve space around owned resource depots and resource containers
    Unitset myUnits = Broodwar->self()->getUnits();
    myUnits.erase_if( !(Exists && (IsCompleted || (ProducesLarva && IsMorphing)) && IsBuilding && (IsResourceDepot || IsRefinery)) );
    for ( auto it = myUnits.begin(); it != myUnits.end(); ++it )
      ReserveStructure(reserve, *it, 2, type, desiredPosition, maxRange);
    
    // Reserve space around neutral resources
    if ( type != UnitTypes::Terran_Bunker )
    {
      Unitset resources = Broodwar->getNeutralUnits();
      resources.erase_if( !(Exists && IsResourceContainer) );
      for ( auto it = resources.begin(); it != resources.end(); ++it )
        ReserveStructure(reserve, *it, 2, type, desiredPosition, maxRange);
    }
    reserve.restoreIfInvalid();
  }

  TilePosition gDirections[] = { 
    TilePosition( 1, 1),
    TilePosition( 0, 1),
    TilePosition(-1, 1),
    TilePosition( 1, 0),
    TilePosition(-1, 0),
    TilePosition( 1,-1),
    TilePosition( 0,-1),
    TilePosition(-1,-1)
  };
  void ReserveDefault(PlacementReserve &reserve, UnitType type, TilePosition desiredPosition, int maxRange)
  {
    reserve.backup();
    auto original = reserve;

    // Reserve some space around some specific units
    Unitset myUnits = Broodwar->self()->getUnits();
    for ( auto it = myUnits.begin(); it != myUnits.end(); ++it )
    {
      if ( !it->exists() )
        continue;

      switch ( it->getType() )
      {
      case UnitTypes::Enum::Terran_Factory:
      case UnitTypes::Enum::Terran_Missile_Turret:
      case UnitTypes::Enum::Protoss_Robotics_Facility:
      case UnitTypes::Enum::Protoss_Gateway:
      case UnitTypes::Enum::Protoss_Photon_Cannon:
        ReserveStructure(reserve, *it, 2, type, desiredPosition, maxRange);
        break;
      case UnitTypes::Enum::Terran_Barracks:
      case UnitTypes::Enum::Terran_Bunker:
      case UnitTypes::Enum::Zerg_Creep_Colony:
        ReserveStructure(reserve, *it, 1, type, desiredPosition, maxRange);
        break;
      }
    }
    
    switch ( type )
    {
      case UnitTypes::Enum::Terran_Barracks:
      case UnitTypes::Enum::Terran_Factory:
      case UnitTypes::Enum::Terran_Missile_Turret:
      case UnitTypes::Enum::Terran_Bunker:
      case UnitTypes::Enum::Protoss_Robotics_Facility:
      case UnitTypes::Enum::Protoss_Gateway:
      case UnitTypes::Enum::Protoss_Photon_Cannon:
        for ( int y = 0; y < 64; ++y )
        {
          for ( int x = 0; x < 64; ++x )
          {
            for ( int dir = 0; dir < 8; ++dir )
            {
              TilePosition p = TilePosition(x,y) + gDirections[dir];
              if ( !PlacementReserve::isValidPos(p) || original.getValue(p) == 0 )
                reserve.setValue(p, 0);
            }

          }
        }
        break;
    }
    reserve.restoreIfInvalid();
  }

  void ReservePlacement(PlacementReserve &reserve, UnitType type, TilePosition desiredPosition, int maxRange, bool creep)
  {
    // Reset the array
    reserve.reset();

    AssignBuildableLocations(reserve, type, desiredPosition, maxRange);
    RemoveDisconnected(reserve, type, desiredPosition, maxRange);
    
    // @TODO: Assign 0 to all locations that have a ground distance > maxRange

    if ( !reserve.hasValidSpace() )
      return;

    ReserveGroundHeight(reserve, type, desiredPosition, maxRange);
    // @TODO: reserveBuildingOnPlacemap   // this one just checks if the space is buildable?? (in that case it's covered already)

    bool ignoreStandardPlacement = type.isResourceDepot();
    if ( !type.isResourceDepot() )
      ReserveAllStructures(reserve, type, desiredPosition, maxRange);
    
    ReserveExistingAddonPlacement(reserve, type, desiredPosition, maxRange);

    // Unit-specific reservations
    switch ( type )
    {
    case UnitTypes::Enum::Protoss_Pylon:  // @TODO
      //reservePylonPlacement();
      break;
    case UnitTypes::Enum::Terran_Bunker:  // @TODO
      //if ( !GetBunkerPlacement() )
      {
        //reserveTurretPlacement();
      }
      break;
    case UnitTypes::Enum::Terran_Missile_Turret:  // @TODO
    case UnitTypes::Enum::Protoss_Photon_Cannon:
      //reserveTurretPlacement();
      break;
    case UnitTypes::Enum::Zerg_Creep_Colony:  // @TODO
      //if ( creep || !GetBunkerPlacement() )
      {
        //reserveTurretPlacement();
      }
      break;
    default:
      if ( !ignoreStandardPlacement )
        ReserveDefault(reserve, type, desiredPosition, maxRange);
      break;
    }
  }

  // ----- GET BUILD LOCATION
  // @TODO: If self() is nullptr, this will crash
  TilePosition Game::getBuildLocation(UnitType type, TilePosition desiredPosition, int maxRange, bool creep)
  {
    this->setLastError(); // Reset last error
    if ( maxRange < 0 ) maxRange = 0;
    if ( maxRange > MAX_RANGE ) maxRange = MAX_RANGE;

    // Make sure the type is compatible
    if ( !type.isBuilding() || !type.whatBuilds().first.isWorker() )
    {
      this->setLastError(Errors::Incompatible_UnitType);
      return TilePositions::Invalid;
    }

    // Do type-specific checks
    bool trimPlacement = true;
    Unit *pSpecialUnitTarget = nullptr;
    switch ( type )
    {
    case UnitTypes::Enum::Protoss_Pylon:
      pSpecialUnitTarget = this->getClosestUnit((Position)desiredPosition, IsOwned && IsUnpowered);
      if ( pSpecialUnitTarget )
      {
        desiredPosition = (TilePosition)pSpecialUnitTarget->getPosition();
        trimPlacement = false;
      }
      break;
    case UnitTypes::Enum::Terran_Command_Center:
    case UnitTypes::Enum::Protoss_Nexus:
    case UnitTypes::Enum::Zerg_Hatchery:
      trimPlacement = false;
      break;
    case UnitTypes::Enum::Zerg_Creep_Colony:
    case UnitTypes::Enum::Terran_Bunker:
      //if ( Get bunker placement region )
      //  trimPlacement = false;
      break;
    }
    
    PlacementReserve reserve;
    //if ( trimPlacement )
    //  trimPlacement();   // using some defs

    return TilePositions::None;
  }
  //------------------------------------------ ACTIONS -----------------------------------------------
  void Game::setScreenPosition(BWAPI::Position p)
  {
    this->setScreenPosition(p.x, p.y);
  }
  void Game::pingMinimap(BWAPI::Position p)
  {
    this->pingMinimap(p.x, p.y);
  }
  void Game::vSendText(const char *format, va_list arg)
  {
    this->vSendTextEx(false, format, arg);
  }
  void Game::sendText(const char *format, ...)
  {
    va_list ap;
    va_start(ap,format);
    this->vSendTextEx(false, format, ap);
    va_end(ap);
  }
  void Game::sendTextEx(bool toAllies, const char *format, ...)
  {
    va_list ap;
    va_start(ap,format);
    this->vSendTextEx(toAllies, format, ap);
    va_end(ap);
  };
  void Game::printf(const char *format, ...)
  {
    va_list ap;
    va_start(ap,format);
    this->vPrintf(format, ap);
    va_end(ap);
  };
  //--------------------------------------------- HAS POWER --------------------------------------------------
  bool Game::hasPower(int tileX, int tileY, UnitType unitType) const
  {
    if ( unitType.isValid() && unitType < UnitTypes::None )
      return hasPowerPrecise( tileX*32 + unitType.tileWidth()*16, tileY*32 + unitType.tileHeight()*16, unitType);
    return hasPowerPrecise( tileX*32, tileY*32, UnitTypes::None);
  }
  bool Game::hasPower(TilePosition position, UnitType unitType) const
  {
    return hasPower(position.x, position.y, unitType);
  }
  bool Game::hasPower(int tileX, int tileY, int tileWidth, int tileHeight, UnitType unitType) const
  {
    return hasPowerPrecise( tileX*32 + tileWidth*16, tileY*32 + tileHeight*16, unitType);
  }
  bool Game::hasPower(TilePosition position, int tileWidth, int tileHeight, UnitType unitType) const
  {
    return hasPower(position.x, position.y, tileWidth, tileHeight, unitType);
  }
  bool Game::hasPowerPrecise(Position position, UnitType unitType) const
  {
    return hasPowerPrecise(position.x, position.y, unitType);
  }
  //------------------------------------------ MAP DATA -----------------------------------------------
  bool Game::isWalkable(BWAPI::WalkPosition position) const
  {
    return this->isWalkable(position.x, position.y);
  }
  int Game::getGroundHeight(TilePosition position) const
  {
    return this->getGroundHeight(position.x, position.y);
  }
  bool Game::isBuildable(TilePosition position, bool includeBuildings) const
  {
    return this->isBuildable(position.x, position.y, includeBuildings);
  }
  bool Game::isVisible(TilePosition position) const
  {
    return this->isVisible(position.x, position.y);
  }
  bool Game::isExplored(TilePosition position) const
  {
    return this->isExplored(position.x, position.y);
  }
  bool Game::hasCreep(TilePosition position) const
  {
    return this->hasCreep(position.x, position.y);
  }
  Unitset Game::getUnitsOnTile(int tileX, int tileY, const UnitFilter &pred) const
  {
    return this->getUnitsOnTile(TilePosition(tileX,tileY), pred);
  }
  Unitset Game::getUnitsOnTile(BWAPI::TilePosition tile, const UnitFilter &pred) const
  {
    if ( !tile )  // if tileposition not valid
      return Unitset::none;

    Position p(tile); // convert to pixel position
    return this->getUnitsInRectangle(p.x, p.y, p.x + 32, p.y + 32, pred);
  }
  Unitset Game::getUnitsInRadius(Position center, int radius, const UnitFilter &pred) const
  {
    return this->getUnitsInRectangle(center.x - radius,
                                     center.y - radius,
                                     center.x + radius,
                                     center.y + radius,
                                     [&center,&radius,&pred](Unit *u){ return u->getDistance(center) <= radius && (!pred.isValid() || pred(u)); });
  }
  Unitset Game::getUnitsInRectangle(BWAPI::Position topLeft, BWAPI::Position bottomRight, const UnitFilter &pred) const
  {
    return this->getUnitsInRectangle(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y, pred);
  }
  Unit *Game::getClosestUnit(Position center, const UnitFilter &pred, int radius) const
  {
    return this->getClosestUnitInRectangle(center,
                                            [&](Unit *u){ return u->getDistance(center) <= radius && (!pred.isValid() || pred(u));},
                                            center.x - radius,
                                            center.y - radius,
                                            center.x + radius,
                                            center.y + radius);
  }
  //------------------------------------------ REGIONS -----------------------------------------------
  BWAPI::Region *Game::getRegionAt(BWAPI::Position position) const
  {
    return this->getRegionAt(position.x, position.y);
  }
  //------------------------------------------ DRAW TEXT ----------------------------------------------
  void Game::drawText(int ctype, int x, int y, const char *format, ...)
  {
    va_list ap;
    va_start(ap,format);
    this->vDrawText(ctype, x, y, format, ap);
    va_end(ap);
  }
  void Game::drawTextMap(int x, int y, const char *format, ...)
  {
    va_list ap;
    va_start(ap,format);
    this->vDrawText(BWAPI::CoordinateType::Map, x, y, format, ap);
    va_end(ap);
  }
  void Game::drawTextMouse(int x, int y, const char *format, ...)
  {
    va_list ap;
    va_start(ap,format);
    this->vDrawText(BWAPI::CoordinateType::Mouse, x, y, format, ap);
    va_end(ap);
  }
  void Game::drawTextScreen(int x, int y, const char *format, ...)
  {
    va_list ap;
    va_start(ap,format);
    this->vDrawText(BWAPI::CoordinateType::Screen, x, y, format, ap);
    va_end(ap);
  }
  void Game::drawTextMap(Position p, const char *format, ...)
  {
    va_list ap;
    va_start(ap,format);
    this->vDrawText(BWAPI::CoordinateType::Map, p.x, p.y, format, ap);
    va_end(ap);
  }
  void Game::drawTextMouse(Position p, const char *format, ...)
  {
    va_list ap;
    va_start(ap,format);
    this->vDrawText(BWAPI::CoordinateType::Mouse, p.x, p.y, format, ap);
    va_end(ap);
  }
  void Game::drawTextScreen(Position p, const char *format, ...)
  {
    va_list ap;
    va_start(ap,format);
    this->vDrawText(BWAPI::CoordinateType::Screen, p.x, p.y, format, ap);
    va_end(ap);
  }
  //------------------------------------------ DRAW BOX -----------------------------------------------
  void Game::drawBoxMap(int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    this->drawBox(CoordinateType::Map, left, top, right, bottom, color, isSolid);
  }
  void Game::drawBoxMouse(int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    this->drawBox(CoordinateType::Mouse, left, top, right, bottom, color, isSolid);
  }
  void Game::drawBoxScreen(int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    this->drawBox(CoordinateType::Screen, left, top, right, bottom, color, isSolid);
  }

  //------------------------------------------ DRAW TRIANGLE -----------------------------------------------
  void Game::drawTriangleMap(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    this->drawTriangle(CoordinateType::Map, ax, ay, bx, by, cx, cy, color, isSolid);
  }
  void Game::drawTriangleMouse(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    this->drawTriangle(CoordinateType::Mouse, ax, ay, bx, by, cx, cy, color, isSolid);
  }
  void Game::drawTriangleScreen(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    this->drawTriangle(CoordinateType::Screen, ax, ay, bx, by, cx, cy, color, isSolid);
  }
  void Game::drawTriangleMap(Position a, Position b, Position c, Color color, bool isSolid)
  {
    this->drawTriangleMap(a.x, a.y, b.x, b.y, c.x, c.y, color, isSolid);
  }
  void Game::drawTriangleMouse(Position a, Position b, Position c, Color color, bool isSolid)
  {
    this->drawTriangleMouse(a.x, a.y, b.x, b.y, c.x, c.y, color, isSolid);
  }
  void Game::drawTriangleScreen(Position a, Position b, Position c, Color color, bool isSolid)
  {
    this->drawTriangleScreen(a.x, a.y, b.x, b.y, c.x, c.y, color, isSolid);
  }
  //------------------------------------------ DRAW CIRCLE -----------------------------------------------
  void Game::drawCircleMap(int x, int y, int radius, Color color, bool isSolid)
  {
    this->drawCircle(CoordinateType::Map, x, y, radius, color, isSolid);
  }
  void Game::drawCircleMouse(int x, int y, int radius, Color color, bool isSolid)
  {
    this->drawCircle(CoordinateType::Mouse, x, y, radius, color, isSolid);
  }
  void Game::drawCircleScreen(int x, int y, int radius, Color color, bool isSolid)
  {
    this->drawCircle(CoordinateType::Screen, x, y, radius, color, isSolid);
  }
  void Game::drawCircleMap(Position p, int radius, Color color, bool isSolid)
  {
    this->drawCircleMap(p.x, p.y, radius, color, isSolid);
  }
  void Game::drawCircleMouse(Position p, int radius, Color color, bool isSolid)
  {
    this->drawCircleMouse(p.x, p.y, radius, color, isSolid);
  }
  void Game::drawCircleScreen(Position p, int radius, Color color, bool isSolid)
  {
    this->drawCircleScreen(p.x, p.y, radius, color, isSolid);
  }
  //------------------------------------------ DRAW ELLIPSE -----------------------------------------------
  void Game::drawEllipseMap(int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    this->drawEllipse(CoordinateType::Map, x, y, xrad, yrad, color, isSolid);
  }
  void Game::drawEllipseMouse(int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    this->drawEllipse(CoordinateType::Mouse, x, y, xrad, yrad, color, isSolid);
  }
  void Game::drawEllipseScreen(int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    this->drawEllipse(CoordinateType::Screen, x, y, xrad, yrad, color, isSolid);
  }
  void Game::drawEllipseMap(Position p, int xrad, int yrad, Color color, bool isSolid)
  {
    this->drawEllipseMap(p.x, p.y, xrad, yrad, color, isSolid);
  }
  void Game::drawEllipseMouse(Position p, int xrad, int yrad, Color color, bool isSolid)
  {
    this->drawEllipseMouse(p.x, p.y, xrad, yrad, color, isSolid);
  }
  void Game::drawEllipseScreen(Position p, int xrad, int yrad, Color color, bool isSolid)
  {
    this->drawEllipseScreen(p.x, p.y, xrad, yrad, color, isSolid);
  }
  //------------------------------------------ DRAW DOT -----------------------------------------------
  void Game::drawDotMap(int x, int y, Color color)
  {
    this->drawDot(CoordinateType::Map, x, y, color);
  }
  void Game::drawDotMouse(int x, int y, Color color)
  {
    this->drawDot(CoordinateType::Mouse, x, y, color);
  }
  void Game::drawDotScreen(int x, int y, Color color)
  {
    this->drawDot(CoordinateType::Screen, x, y, color);
  }
  void Game::drawDotMap(Position p, Color color)
  {
    this->drawDotMap(p.x, p.y, color);
  }
  void Game::drawDotMouse(Position p, Color color)
  {
    this->drawDotMouse(p.x, p.y, color);
  }
  void Game::drawDotScreen(Position p, Color color)
  {
    this->drawDotScreen(p.x, p.y, color);
  }
  //------------------------------------------ DRAW LINE -----------------------------------------------
  void Game::drawLineMap(int x1, int y1, int x2, int y2, Color color)
  {
    this->drawLine(CoordinateType::Map, x1, y1, x2, y2, color);
  }
  void Game::drawLineMouse(int x1, int y1, int x2, int y2, Color color)
  {
    this->drawLine(CoordinateType::Mouse, x1, y1, x2, y2, color);
  }
  void Game::drawLineScreen(int x1, int y1, int x2, int y2, Color color)
  {
    this->drawLine(CoordinateType::Screen, x1, y1, x2, y2, color);
  }
  void Game::drawLineMap(Position a, Position b, Color color)
  {
    this->drawLineMap(a.x, a.y, b.x, b.y, color);
  }
  void Game::drawLineMouse(Position a, Position b, Color color)
  {
    this->drawLineMouse(a.x, a.y, b.x, b.y, color);
  }
  void Game::drawLineScreen(Position a, Position b, Color color)
  {
    this->drawLineScreen(a.x, a.y, b.x, b.y, color);
  }

}

