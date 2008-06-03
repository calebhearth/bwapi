#include "Position.h"

#include <math.h>

#include <tinyXml.h>
#include <Util/Exceptions.h>
#include <Util/Xml.h>

#include "Offsets.h"

namespace BW
{
  //------------------------------------- CONSTRUCTOR ------------------------------
  Position::Position()
  :x(0)
  ,y(0)
  {
  }
  //------------------------------------- CONSTRUCTOR ------------------------------
  Position::Position(const BW::TilePosition& position)
  :x(position.x*BW::TILE_SIZE)
  ,y(position.y*BW::TILE_SIZE)
  {
  }
  //------------------------------------- CONSTRUCTOR ------------------------------
  Position::Position(TiXmlElement* xmlElement)
  {
    this->x = Util::Xml::getRequiredU16Attribute(xmlElement, "x");
    this->y = Util::Xml::getRequiredU16Attribute(xmlElement, "y");
  }
  //------------------------------------- DESTRUCTOR -------------------------------
  Position::Position(u16 x, u16 y)
  :x(x)
  ,y(y)
  {
  }
  //------------------------------------- OPERATOR == ------------------------------
  bool Position::operator == (BW::Position& position) const
  {
    return this->x == position.x &&
           this->y == position.y;
  }
  //------------------------------------- OPERATOR != ------------------------------
  bool Position::operator != (BW::Position& position) const
  {
    return this->x != position.x ||
           this->y != position.y;
  }
  //--------------------------------------------------------------------------------
  u16 Position::getDistance(const Position &position) const
  {
    return (u16)sqrt((long double)((long double)this->x - position.x)*((long double)this->x - position.x) +
                     (long double)((long double)this->y - position.y)*((long double)this->y - position.y));
  }
  //--------------------------------------------------------------------------------
};
