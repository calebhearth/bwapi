#pragma once

#include <BWAPI/Position.h>
#include <Util/Types.h>

namespace BW
{
  /** Direct mapping of bw representation of map position. */
  _MAKE_POSITION_TEMPLATE(Position,u16,1)
  _MAKE_POSITION_TEMPLATE(TilePosition,u16,32)
};

