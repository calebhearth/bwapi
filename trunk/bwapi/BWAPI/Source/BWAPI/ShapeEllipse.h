#pragma once
#include "Shape.h"
namespace BWAPI
{
  class ShapeEllipse : public Shape
  {
  public :
    ShapeEllipse(CoordinateType::Enum ctype, int x, int y, int xrad, int yrad, int color, bool isSolid=false);
    virtual void draw();
  private :
    CoordinateType::Enum ctype;
    int x;
    int y;
    int xrad;
    int yrad;
    int color;
    bool isSolid;
  };
}