#include "Position.h"
#include "TilePosition.h"

#include <math.h>

#include "Offsets.h"

#include "../../Debug.h"
namespace BW
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  Position::Position()
      : x(0)
      , y(0)
  {
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  Position::Position(const BW::TilePosition& position)
      : x(position.x * TILE_SIZE)
      , y(position.y * TILE_SIZE)
  {
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  Position::Position(u16 x, u16 y)
      : x(x)
      , y(y)
  {
  }
  //---------------------------------------------- OPERATOR == -----------------------------------------------
  bool Position::operator == (const BW::Position& position) const
  {
    return this->x == position.x &&
           this->y == position.y;
  }
  //---------------------------------------------- OPERATOR != -----------------------------------------------
  bool Position::operator != (const BW::Position& position) const
  {
    return this->x != position.x ||
           this->y != position.y;
  }
  //---------------------------------------------- OPERATOR < ------------------------------------------------
  bool Position::operator  < (const BW::Position& position) const
  {
    return this->x   < position.x ||
           (this->x == position.x && this->y < position.y);
  }
  //----------------------------------------------------------------------------------------------------------
  u16 Position::getDistance(const Position& position) const
  {
    return (u16)sqrt((long double)((long double)this->x - position.x) * ((long double)this->x - position.x) +
                     (long double)((long double)this->y - position.y) * ((long double)this->y - position.y));
  }
  //----------------------------------------------------------------------------------------------------------
};
