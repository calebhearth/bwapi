#include "Graphics.h"
#include "BW/Offsets.h"
#include "BW/Dialog.h"
#include "BW/Font.h"
#include "BW/Bitmap.h"

#include <BWAPI/CoordinateType.h>
#include <BWAPI/Color.h>

struct iRect
{
  int left, top, right, bottom;
};

static inline void bwPlot(const int &x, const int &y, const int &color)
{
  BW::BWDATA::GameScreenBuffer->plot(x, y, (u8)color);
}

static inline void convertCoordType(int &x, int &y, const int &ctype)
{
  switch ( ctype )
  {
  case BWAPI::CoordinateType::Map:
    x -= *(BW::BWDATA::ScreenX);
    y -= *(BW::BWDATA::ScreenY);
    break;
  case BWAPI::CoordinateType::Mouse:
    x += BW::BWDATA::Mouse->x;
    y += BW::BWDATA::Mouse->y;
    break;
  }
}

void bwDrawBox(int x, int y, int w, int h, int color, int ctype)
{
  convertCoordType(x, y, ctype);

  for ( int i = y; i < y+h; ++i )
    BW::BWDATA::GameScreenBuffer->drawLine(x, i, x+w, i, (u8)color);
}

void bwDrawDot(int x, int y, int color, int ctype)
{
  // Convert coordinate type
  convertCoordType(x, y, ctype);

  // Plot the point
  bwPlot(x, y, color);
}

// Assume x1 != x2 and y1 != y2
void bwDrawLine(int x1, int y1, int x2, int y2, int color, int ctype)
{
  convertCoordType(x1,y1,ctype);
  convertCoordType(x2,y2,ctype);
  
  BW::BWDATA::GameScreenBuffer->drawLine(x1, y1, x2, y2, (u8)color);
}

void bwDrawText(int x, int y, const char* ptext, int ctype, char size)
{
  convertCoordType(x, y, ctype);
  BW::BWDATA::GameScreenBuffer->blitString(ptext, x, y, size);
}
