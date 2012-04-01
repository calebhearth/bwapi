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

}

