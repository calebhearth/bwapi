#include "BuildingPlacer.h"

#include <BWAPI/Unitset.h>

BuildingPlacer::BuildingPlacer()
{
  reserveMap.resize(BWAPI::Broodwar->mapWidth(), BWAPI::Broodwar->mapHeight());
  reserveMap.setTo(false);
  this->buildDistance=1;
}
bool BuildingPlacer::canBuildHere(BWAPI::TilePosition position, BWAPI::UnitType type) const
{
  //returns true if we can build this type of unit here. Takes into account reserved tiles.
  if (!BWAPI::Broodwar->canBuildHere(position, type))
    return false;
  for ( BWAPI::TilePosition::iterator i(position, position + type.tileSize()); i; ++i )
    if (reserveMap[i.x][i.y])
      return false;
  return true;
}
bool BuildingPlacer::canBuildHereWithSpace(BWAPI::TilePosition position, BWAPI::UnitType type) const
{
  return canBuildHereWithSpace(position,type,this->buildDistance);
}
bool BuildingPlacer::canBuildHereWithSpace(BWAPI::TilePosition position, BWAPI::UnitType type, int buildDist) const
{
  //returns true if we can build this type of unit here with the specified amount of space.
  //space value is stored in this->buildDistance.

  //if we can't build here, we of course can't build here with space
  if (!this->canBuildHere(position, type))
    return false;

  int width  = type.tileWidth();
  int height = type.tileHeight();

  //make sure we leave space for add-ons. These types of units can have addons:
  if ( type.canBuildAddon() )
    width += 2;

  BWAPI::TilePosition start(position - BWAPI::TilePosition(buildDist, buildDist));
  BWAPI::TilePosition end(position + BWAPI::TilePosition(buildDist + width, buildDist + height) );
  if ( !start || !end )
    return false;

  if ( !type.isRefinery() )
  {
    for ( BWAPI::TilePosition::iterator i(start, end); i; ++i )
    {
      if ( !buildable(i.x, i.y) || reserveMap[i.x][i.y] )
        return false;
    }
  }

  if ( position.x > 3 )
  {
    BWAPI::TilePosition newStart((start - BWAPI::TilePosition(2,0)).makeValid());
    BWAPI::TilePosition newEnd(start.x, end.y);

    for ( BWAPI::TilePosition::iterator p(newStart,newEnd); p; ++p )
    {
      BWAPI::Unitset units = BWAPI::Broodwar->getUnitsOnTile(p.x, p.y);
      for( BWAPI::Unitset::iterator i = units.begin(); i != units.end(); ++i )
      {
        if ( !i->isLifted() && i->getType().canBuildAddon() )
          return false;
      }
    }
  } //position.x > 3
  return true;
}
BWAPI::TilePosition BuildingPlacer::getBuildLocation(BWAPI::UnitType type) const
{
  //returns a valid build location if one exists, scans the map left to right
  for( BWAPI::TilePosition::iterator p(BWAPI::Broodwar->mapWidth(), BWAPI::Broodwar->mapHeight()); p; ++p )
  {
    if (this->canBuildHere(BWAPI::TilePosition(p.x, p.y), type))
      return p;
  }
  return BWAPI::TilePositions::None;
}

BWAPI::TilePosition BuildingPlacer::getBuildLocationNear(BWAPI::TilePosition position, BWAPI::UnitType type) const
{
  return getBuildLocationNear(position, type,this->buildDistance);
}

BWAPI::TilePosition BuildingPlacer::getBuildLocationNear(BWAPI::TilePosition position, BWAPI::UnitType type, int buildDist) const
{
  //returns a valid build location near the specified tile position.
  //searches outward in a spiral.
  BWAPI::TilePosition pos(position);

  int length = 1;
  int j      = 0;
  bool first = true;
  int dx     = 0;
  int dy     = 1;
  while (length < BWAPI::Broodwar->mapWidth()) //We'll ride the spiral to the end
  {
    //if we can build here, return this tile position
    if ( pos )
      if (this->canBuildHereWithSpace(pos, type, buildDist))
        return pos;

    //otherwise, move to another position
    pos += BWAPI::TilePosition(dx,dy);

    //count how many steps we take in this direction
    j++;
    if (j == length) //if we've reached the end, its time to turn
    {
      //reset step counter
      j = 0;

      //Spiral out. Keep going.
      if (!first)
        length++; //increment step counter if needed

      //first=true for every other turn so we spiral out at the right rate
      first =! first;

      //turn counter clockwise 90 degrees:
      if (dx == 0)
      {
        dx = dy;
        dy = 0;
      }
      else
      {
        dy = -dx;
        dx = 0;
      }
    }
    //Spiral out. Keep going.
  }
  return BWAPI::TilePositions::None;
}

bool BuildingPlacer::buildable(int x, int y) const
{
  //returns true if this tile is currently buildable, takes into account units on tile
  if (!BWAPI::Broodwar->isBuildable(x,y)) return false;
  BWAPI::Unitset units = BWAPI::Broodwar->getUnitsOnTile(x, y);
  for(BWAPI::Unitset::iterator i = units.begin(); i != units.end(); ++i)
    if (i->getType().isBuilding() && !i->isLifted())
      return false;
  return true;
}

void BuildingPlacer::reserveTiles(BWAPI::TilePosition position, int width, int height)
{
  BWAPI::TilePosition end(position + BWAPI::TilePosition(width,height));
  end.setMax(reserveMap.getWidth()-1, reserveMap.getHeight()-1);

  for ( BWAPI::TilePosition::iterator p(position, end); p; ++p )
    reserveMap[p.x][p.y] = true;
}

void BuildingPlacer::freeTiles(BWAPI::TilePosition position, int width, int height)
{
  for(int x = position.x; x < position.x + width && x < (int)reserveMap.getWidth(); ++x)
    for(int y = position.y; y < position.y + height && y < (int)reserveMap.getHeight(); ++y)
      reserveMap[x][y] = false;
}

void BuildingPlacer::setBuildDistance(int distance)
{
  this->buildDistance=distance;
}
int BuildingPlacer::getBuildDistance() const
{
  return this->buildDistance;
}
bool BuildingPlacer::isReserved(int x, int y) const
{
  if ( x < 0 || y < 0 || x >= (int)reserveMap.getWidth() || y >= (int)reserveMap.getHeight())
    return false;
  return reserveMap[x][y];
}