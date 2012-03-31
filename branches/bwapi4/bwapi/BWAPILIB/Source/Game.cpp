#include <BWAPI/Game.h>
#include <BWAPI/Position.h>

namespace BWAPI
{
  void Game::setScreenPosition(BWAPI::Position p)
  {
    this->setScreenPosition(p.x, p.y);
  }

  void Game::pingMinimap(BWAPI::Position p)
  {
    this->pingMinimap(p.x, p.y);
  }

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

  BWAPI::Region *Game::getRegionAt(BWAPI::Position position) const
  {
    return this->getRegionAt(position.x, position.y);
  }
}

