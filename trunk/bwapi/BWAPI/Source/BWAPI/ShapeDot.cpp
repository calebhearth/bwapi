#include "ShapeDot.h"
#include "DLLMain.h"
#include "Graphics.h"
#include "BWAPI/Game.h"

#include "../../Debug.h"

namespace BWAPI
{
  ShapeDot::ShapeDot(int ctype, int x, int y, int color)
  :ctype(ctype)
  ,x(x)
  ,y(y)
  ,color(color)
  {
  }

  void ShapeDot::draw()
  {
    drawDot(x,y,color,ctype);
  }
};
