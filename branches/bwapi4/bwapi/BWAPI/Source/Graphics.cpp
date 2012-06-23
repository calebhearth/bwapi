#include "Graphics.h"
#include "BW/Offsets.h"
#include "BW/Dialog.h"

#include <BWAPI/CoordinateType.h>
#include <BWAPI/Color.h>

struct iRect
{
  int left, top, right, bottom;
};

static inline void bwPlot(const int &x, const int &y, const int &color)
{
  if ( x < 0 ||
       y < 0 ||
       x >= BW::BWDATA_GameScreenBuffer->wid - 2 ||
       y >= BW::BWDATA_GameScreenBuffer->ht - 2)
    return;

  u8 *data = BW::BWDATA_GameScreenBuffer->data;
  data[y * BW::BWDATA_GameScreenBuffer->wid + x] = (u8)color;
}

static inline void convertCoordType(int &x, int &y, const int &ctype)
{
  switch ( ctype )
  {
  case BWAPI::CoordinateType::Map:
    x -= *(BW::BWDATA_ScreenX);
    y -= *(BW::BWDATA_ScreenY);
    break;
  case BWAPI::CoordinateType::Mouse:
    x += BW::BWDATA_Mouse->x;
    y += BW::BWDATA_Mouse->y;
    break;
  }
}

void bwDrawBox(int x, int y, int w, int h, int color, int ctype)
{
  convertCoordType(x, y, ctype);

  u16 scrWid = BW::BWDATA_GameScreenBuffer->wid, scrHgt = BW::BWDATA_GameScreenBuffer->ht;

  int right  = x + w;
  int bottom = y + h;
  if (right    <= 0   ||
      bottom   <= 0   ||
      x >= scrWid - 1 ||
      y >= scrHgt  - 1)
    return;

  if ( right > scrWid- 1 )
    w = (scrWid - 1) - x;
  if ( bottom > scrHgt - 1 )
    h = (scrHgt - 1) - y;
  if ( x < 0 )
  { 
    w += x;
    x =  0;
  }
  if ( y < 0 )
  {
    h += y;
    y =  0;
  }

  bottom = y + h;
  if ( w == 1 )
  {
    for ( int iy = y; iy < bottom; ++iy )
      BW::BWDATA_GameScreenBuffer->data[iy * scrWid + x] = (u8)color;
  }
  else
  {
    for ( int iy = y; iy < bottom; ++iy )
      memset(&BW::BWDATA_GameScreenBuffer->data[iy * scrWid + x], (u8)color, w);
  }
}

void bwDrawBoxEx(int _x, int _y, int _w, int _h, int borderColor, int borderThickness, int color, int ctype, int style, int intensity)
{
  iRect box = { _x, _y, _w, _h };
  convertCoordType(box.left, box.top, ctype);

  SIZE screen = { BW::BWDATA_GameScreenBuffer->wid, BW::BWDATA_GameScreenBuffer->ht };

  int right  = box.left + box.right;
  int bottom = box.top  + box.bottom;
  if (right    <= 0   ||
      bottom   <= 0   ||
      box.left >= screen.cx - 1 ||
      box.top  >= screen.cy - 1)
    return;

  RECT border = { borderThickness, borderThickness, borderThickness, borderThickness };
  if ( right > screen.cx - 1 )
  {
    box.right = (screen.cx - 1) - box.left;
    border.right -= right - (screen.cx - 1);
  }
  if ( bottom > screen.cy - 1 )
  {
    box.bottom = (screen.cy - 1) - box.top;
    border.bottom -= bottom - (screen.cy - 1);
  }
  if ( box.left < 0 )
  { 
    box.right += box.left; // subtracts because < 0
    border.left += box.left;
    box.left  =  0;
  }
  if ( box.top < 0 )
  {
    box.bottom  += box.top; // subtracts because < 0
    border.bottom += box.left;
    box.top     =  0;
  }
  u8 *data    = BW::BWDATA_GameScreenBuffer->data;
  right  = box.left + box.right;
  bottom = box.top  + box.bottom;
  
  if ( border.left < 0 )
    border.left = 0;
  if ( border.top < 0 )
    border.top = 0;
  if ( border.right < 0 )
    border.right = 0;
  if ( border.bottom < 0 )
    border.bottom = 0;

  // Draw Border
  // top
  if ( border.top > 0 )
    for ( int iy = box.top; iy < box.top + border.top; ++iy )
      memset(&data[iy * screen.cx + box.left], (u8)borderColor, box.right);
  // bottom
  if ( border.bottom > 0 )
    for ( int iy = bottom - border.bottom; iy < bottom; ++iy )
      memset(&data[iy * screen.cx + box.left], (u8)borderColor, box.right);
  // left
  if ( border.left > 0 )
    for ( int iy = box.top; iy < bottom; ++iy )
      memset(&data[iy * screen.cx + box.left], (u8)borderColor, border.left);
  // right
  if ( border.right > 0 )
    for ( int iy = box.top; iy < bottom; ++iy )
      memset(&data[iy * screen.cx + (right - border.right)], (u8)borderColor, border.right);

  BWAPI::Color c(color);
  int r = (c.red()*intensity)/100;
  int g = (c.green()*intensity)/100;
  int b = (c.blue()*intensity)/100;
  for ( int iy = box.top + border.top; iy < bottom - border.bottom; ++iy )
  {
    for ( int ix = box.left + border.left; ix < right - border.right; ++ix )
    {
      BWAPI::Color d(data[iy * screen.cx + ix]);
      data[iy * screen.cx + ix] = (u8)BWAPI::Color( d.red() | r, d.green() | g, d.blue() | b);
    }
  }
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

  // Line algorithm source: http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm#Simplification
  int dx = std::abs(x2 - x1),     dy = std::abs(y2 - y1);
  int sx = x1 < x2 ? 1 : -1,      sy = y1 < y2 ? 1 : -1;
  int err = dx - dy;

  while ( bwPlot(x1,y1,color), x1 != x2 || y1 != y2 )
  {
    int e2 = 2*err;
    if ( e2 > -dy )
    {
      err -= dy;
      x1 += sx;
    }
    if ( e2 < dx )
    {
      err += dx;
      y1 += sy;
    }
  }
}

void bwDrawText(int x, int y, const char* ptext, int ctype, char size)
{
  convertCoordType(x, y, ctype);

  if (x + BW::GetTextWidth(ptext, size)  < 0 || 
      y + BW::GetTextHeight(ptext, size) < 0 || 
      x >= BW::BWDATA_GameScreenBuffer->wid   || 
      y >= BW::BWDATA_GameScreenBuffer->ht)
    return;

  BW::BlitText(ptext, BW::BWDATA_GameScreenBuffer, x, y, size);
}
