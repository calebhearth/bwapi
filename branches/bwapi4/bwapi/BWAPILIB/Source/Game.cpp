#include <BWAPI/Game.h>
#include <BWAPI/Position.h>

namespace BWAPI
{
  //------------------------------------------ ACTIONS -----------------------------------------------
  void Game::setScreenPosition(BWAPI::Position p)
  {
    this->setScreenPosition(p.x, p.y);
  }
  void Game::pingMinimap(BWAPI::Position p)
  {
    this->pingMinimap(p.x, p.y);
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

  //------------------------------------------ REGIONS -----------------------------------------------
  BWAPI::Region *Game::getRegionAt(BWAPI::Position position) const
  {
    return this->getRegionAt(position.x, position.y);
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

