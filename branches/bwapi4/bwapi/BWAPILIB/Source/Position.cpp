#include <BWAPI/Position.h>

#define _DEFINE_POSITION_TEMPLATE(_n,_t,_s) _n::_n() : Point<_t,_s>(0,0) { };                     \
                                            _n::_n(_t x, _t y) : Point<_t,_s>(x,y) { };

namespace BWAPI
{
  _DEFINE_POSITION_TEMPLATE(Position,int,1);
  _DEFINE_POSITION_TEMPLATE(TilePosition,int,32);
  _DEFINE_POSITION_TEMPLATE(WalkPosition,int,8);
}
