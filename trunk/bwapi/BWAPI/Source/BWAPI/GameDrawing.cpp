#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used stuff from Windows headers
#include "GameImpl.h"

#include <windows.h>

#include "../../Debug.h"
#include <cassert>
#include "Graphics.h"

namespace BWAPI
{
  //-------------------------------------------------- IN SCREEN ---------------------------------------------
  bool GameImpl::inScreen(int ctype, int x, int y)
  {
    if ( !data->hasGUI )
      return false;
    int screen_x1 = x;
    int screen_y1 = y;
    switch ( ctype )
    {
    case BWAPI::CoordinateType::Map: // if we're using map coordinates, subtract the position of the screen to convert the coordinates into screen coordinates
      screen_x1 -= *(BW::BWDATA_ScreenX);
      screen_y1 -= *(BW::BWDATA_ScreenY);
      break;
    case BWAPI::CoordinateType::Mouse: // if we're using mouse coordinates, add the position of the mouse to convert the coordinates into screen coordinates
      screen_x1 += BW::BWDATA_Mouse->x;
      screen_y1 += BW::BWDATA_Mouse->y;
      break;
    }
    if (screen_x1 < 0 || 
        screen_y1 < 0 ||
        screen_x1 > BW::BWDATA_GameScreenBuffer->wid || 
        screen_y1 > BW::BWDATA_GameScreenBuffer->ht)
      return false;
    return true;
  }

  bool GameImpl::inScreen(int ctype, int x1, int y1, int x2, int y2)
  {
    if ( !data->hasGUI )
      return false;
    int screen_x1 = x1;
    int screen_y1 = y1;
    int screen_x2 = x2;
    int screen_y2 = y2;
    switch ( ctype )
    {
    case BWAPI::CoordinateType::Map: // if we're using map coordinates, subtract the position of the screen to convert the coordinates into screen coordinates
      {
        int maxW = Broodwar->mapWidth()*32;
        int maxH = Broodwar->mapHeight()*32;
        if ( (x1 < 0 && x2 < 0)         ||
             (y1 < 0 && y2 < 0)         ||
             (x1 >= maxW && x2 >= maxW) ||
             (y1 >= maxH && y2 >= maxH) )
          return false;

        screen_x1 -= *(BW::BWDATA_ScreenX);
        screen_y1 -= *(BW::BWDATA_ScreenY);
        screen_x2 -= *(BW::BWDATA_ScreenX);
        screen_y2 -= *(BW::BWDATA_ScreenY);
        break;
      }
    case BWAPI::CoordinateType::Mouse: // if we're using mouse coordinates, add the position of the mouse to convert the coordinates into screen coordinates
      screen_x1 += BW::BWDATA_Mouse->x;
      screen_y1 += BW::BWDATA_Mouse->y;
      screen_x2 += BW::BWDATA_Mouse->x;
      screen_y2 += BW::BWDATA_Mouse->y;
      break;
    }
    rect scrLimit = { 0, 0, BW::BWDATA_GameScreenBuffer->wid, BW::BWDATA_GameScreenBuffer->ht };
    if ((screen_x1 < 0 && screen_x2 < 0) ||
        (screen_y1 < 0 && screen_y2 < 0) ||
        (screen_x1 > scrLimit.right  && screen_x2 > scrLimit.right) ||
        (screen_y1 > scrLimit.bottom && screen_y2 > scrLimit.bottom))
      return false;
    return true;
  }

  bool GameImpl::inScreen(int ctype, int x1, int y1, int x2, int y2, int x3, int y3)
  {
    if ( !data->hasGUI )
      return false;
    int screen_x1 = x1;
    int screen_y1 = y1;
    int screen_x2 = x2;
    int screen_y2 = y2;
    int screen_x3 = x3;
    int screen_y3 = y3;
    switch ( ctype )
    {
    case BWAPI::CoordinateType::Map: // if we're using map coordinates, subtract the position of the screen to convert the coordinates into screen coordinates
      {
        int maxW = Broodwar->mapWidth()*32;
        int maxH = Broodwar->mapHeight()*32;
        if ( (x1 < 0 && x2 < 0 && x3 < 0)             ||
             (y1 < 0 && y2 < 0 && y3 < 0)             ||
             (x1 >= maxW && x2 >= maxW && x3 >= maxW) ||
             (y1 >= maxH && y2 >= maxH && y3 >= maxH) )
          return false;
        screen_x1 -= *(BW::BWDATA_ScreenX);
        screen_y1 -= *(BW::BWDATA_ScreenY);
        screen_x2 -= *(BW::BWDATA_ScreenX);
        screen_y2 -= *(BW::BWDATA_ScreenY);
        screen_x3 -= *(BW::BWDATA_ScreenX);
        screen_y3 -= *(BW::BWDATA_ScreenY);
        break;
      }
    case BWAPI::CoordinateType::Mouse: // if we're using mouse coordinates, add the position of the mouse to convert the coordinates into screen coordinates
      screen_x1 += BW::BWDATA_Mouse->x;
      screen_y1 += BW::BWDATA_Mouse->y;
      screen_x2 += BW::BWDATA_Mouse->x;
      screen_y2 += BW::BWDATA_Mouse->y;
      screen_x3 += BW::BWDATA_Mouse->x;
      screen_y3 += BW::BWDATA_Mouse->y;
      break;
    }
    rect scrLimit = { 0, 0, BW::BWDATA_GameScreenBuffer->wid, BW::BWDATA_GameScreenBuffer->ht };
    if ((screen_x1 < 0 && screen_x2 < 0 && screen_x3 < 0) ||
        (screen_y1 < 0 && screen_y2 < 0 && screen_y3 < 0) ||
        (screen_x1 > scrLimit.right && screen_x2 > scrLimit.right && screen_x3 > scrLimit.right) ||
        (screen_y1 > scrLimit.bottom && screen_y2 > scrLimit.bottom && screen_y3 > scrLimit.bottom))
      return false;
    return true;
  }
  int GameImpl::addShape(const BWAPIC::Shape &s)
  {
    assert(data->shapeCount < GameData::MAX_SHAPES);
    data->shapes[data->shapeCount] = s;
    return data->shapeCount++;
  }
  int GameImpl::addString(const char* text)
  {
    assert(data->stringCount < GameData::MAX_STRINGS);
    strncpy(data->strings[data->stringCount], text, 256);
    return data->stringCount++;
  }
  int GameImpl::addText(BWAPIC::Shape &s, const char* text)
  {
    s.extra1 = addString(text);
    return addShape(s);
  }
  //-------------------------------------------------- DRAW TEXT ---------------------------------------------
  void GameImpl::setTextSize(int size)
  {
    if ( size < 0 )
      size = 0;
    if ( size > 3 )
      size = 3;
    if ( !this->tournamentCheck(Tournament::SetTextSize, &size) )
      return;
    textSize = size;
  }
  void GameImpl::drawText(int ctype, int x, int y, const char *format, ...)
  {
    if ( !data->hasGUI ) return;
    char *buffer;
    vstretchyprintf(buffer, format);
    BWAPIC::Shape s(BWAPIC::ShapeType::Text,ctype,x,y,0,0,0,textSize,0,false);
    addText(s,buffer);
    free(buffer);
  }
  void GameImpl::drawTextMap(int x, int y, const char *format, ...)
  {
    if ( !data->hasGUI ) return;
    char *buffer;
    vstretchyprintf(buffer, format);
    BWAPIC::Shape s(BWAPIC::ShapeType::Text,(int)BWAPI::CoordinateType::Map,x,y,0,0,0,textSize,0,false);
    addText(s,buffer);
    free(buffer);
  }
  void GameImpl::drawTextMouse(int x, int y, const char *format, ...)
  {
    if ( !data->hasGUI ) return;
    char *buffer;
    vstretchyprintf(buffer, format);
    BWAPIC::Shape s(BWAPIC::ShapeType::Text,(int)BWAPI::CoordinateType::Mouse,x,y,0,0,0,textSize,0,false);
    addText(s,buffer);
    free(buffer);
  }
  void GameImpl::drawTextScreen(int x, int y, const char *format, ...)
  {
    if ( !data->hasGUI ) return;
    char *buffer;
    vstretchyprintf(buffer, format);
    BWAPIC::Shape s(BWAPIC::ShapeType::Text,(int)BWAPI::CoordinateType::Screen,x,y,0,0,0,textSize,0,false);
    addText(s,buffer);
    free(buffer);
  }
  //--------------------------------------------------- DRAW BOX ---------------------------------------------
  void GameImpl::drawBox(int ctype, int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    /* Draws a box */
    if (!inScreen(ctype,left,top,right,bottom)) return;
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Box,ctype,left,top,right,bottom,0,0,color,isSolid));
  }
  void GameImpl::drawBoxMap(int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    /* Draws a box in relation to the map */
    if (!inScreen(BWAPI::CoordinateType::Map,left,top,right,bottom)) return;
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Box,(int)BWAPI::CoordinateType::Map,left,top,right,bottom,0,0,color,isSolid));
  }
  void GameImpl::drawBoxMouse(int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    /* Draws a box in relation to the mouse */
    if (!inScreen(BWAPI::CoordinateType::Mouse,left,top,right,bottom)) return;
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Box,(int)BWAPI::CoordinateType::Mouse,left,top,right,bottom,0,0,color,isSolid));
  }
  void GameImpl::drawBoxScreen(int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    /* Draws a box in relation to the screen */
    if (!inScreen(BWAPI::CoordinateType::Screen,left,top,right,bottom)) return;
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Box,(int)BWAPI::CoordinateType::Screen,left,top,right,bottom,0,0,color,isSolid));
  }
  //------------------------------------------------ DRAW TRIANGLE -------------------------------------------
  void GameImpl::drawTriangle(int ctype, int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    if (!inScreen(ctype,ax,ay,bx,by,cx,cy)) return;
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Triangle,ctype,ax,ay,bx,by,cx,cy,color,isSolid));
  }
  void GameImpl::drawTriangleMap(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Map,ax,ay,bx,by,cx,cy)) return;
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Triangle,(int)BWAPI::CoordinateType::Map,ax,ay,bx,by,cx,cy,color,isSolid));
  }
  void GameImpl::drawTriangleMouse(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Mouse,ax,ay,bx,by,cx,cy)) return;
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Triangle,(int)BWAPI::CoordinateType::Mouse,ax,ay,bx,by,cx,cy,color,isSolid));
  }
  void GameImpl::drawTriangleScreen(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Screen,ax,ay,bx,by,cx,cy)) return;
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Triangle,(int)BWAPI::CoordinateType::Screen,ax,ay,bx,by,cx,cy,color,isSolid));
  }
  //------------------------------------------------- DRAW CIRCLE --------------------------------------------
  void  GameImpl::drawCircle(int ctype, int x, int y, int radius, Color color, bool isSolid)
  {
    if (!inScreen(ctype,x-radius,y-radius,x+radius,y+radius)) return;
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Circle,ctype,x,y,0,0,radius,0,color,isSolid));
  }
  void GameImpl::drawCircleMap(int x, int y, int radius, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Map,x-radius,y-radius,x+radius,y+radius)) return;
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Circle,(int)BWAPI::CoordinateType::Map,x,y,0,0,radius,0,color,isSolid));
  }
  void GameImpl::drawCircleMouse(int x, int y, int radius, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Mouse,x-radius,y-radius,x+radius,y+radius)) return;
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Circle,(int)BWAPI::CoordinateType::Mouse,x,y,0,0,radius,0,color,isSolid));
  }
  void GameImpl::drawCircleScreen(int x, int y, int radius, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Screen,x-radius,y-radius,x+radius,y+radius)) return;
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Circle,(int)BWAPI::CoordinateType::Screen,x,y,0,0,radius,0,color,isSolid));
  }
  //------------------------------------------------- DRAW ELIPSE --------------------------------------------
  void GameImpl::drawEllipse(int ctype, int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    if (!inScreen(ctype,x-xrad,y-yrad,x+xrad,y+yrad)) return;
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Ellipse,ctype,x,y,0,0,xrad,yrad,color,isSolid));
  }
  void GameImpl::drawEllipseMap(int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Map,x-xrad,y-yrad,x+xrad,y+yrad)) return;
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Ellipse,(int)BWAPI::CoordinateType::Map,x,y,0,0,xrad,yrad,color,isSolid));
  }
  void GameImpl::drawEllipseMouse(int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Mouse,x-xrad,y-yrad,x+xrad,y+yrad)) return;
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Ellipse,(int)BWAPI::CoordinateType::Mouse,x,y,0,0,xrad,yrad,color,isSolid));
  }
  void GameImpl::drawEllipseScreen(int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Screen,x-xrad,y-yrad,x+xrad,y+yrad)) return;
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Ellipse,(int)BWAPI::CoordinateType::Screen,x,y,0,0,xrad,yrad,color,isSolid));
  }
  //--------------------------------------------------- DRAW DOT ---------------------------------------------
  void GameImpl::drawDot(int ctype, int x, int y, Color color)
  {
    if (!inScreen(ctype,x,y)) return;
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Dot,ctype,x,y,0,0,0,0,color,false));
  }
  void GameImpl::drawDotMap(int x, int y, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Map,x,y)) return;
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Dot,(int)BWAPI::CoordinateType::Map,x,y,0,0,0,0,color,false));
  }
  void GameImpl::drawDotMouse(int x, int y, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Mouse,x,y)) return;
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Dot,(int)BWAPI::CoordinateType::Mouse,x,y,0,0,0,0,color,false));
  }
  void GameImpl::drawDotScreen(int x, int y, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Screen,x,y)) return;
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Dot,(int)BWAPI::CoordinateType::Screen,x,y,0,0,0,0,color,false));
  }
  //-------------------------------------------------- DRAW LINE ---------------------------------------------
  void GameImpl::drawLine(int ctype, int x1, int y1, int x2, int y2, Color color)
  {
    if (!inScreen(ctype,x1,y1,x2,y2)) return;
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Line,ctype,x1,y1,x2,y2,0,0,color,false));
  }
  void GameImpl::drawLineMap(int x1, int y1, int x2, int y2, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Map,x1,y1,x2,y2)) return;
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Line,(int)BWAPI::CoordinateType::Map,x1,y1,x2,y2,0,0,color,false));
  }
  void GameImpl::drawLineMouse(int x1, int y1, int x2, int y2, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Mouse,x1,y1,x2,y2)) return;
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Line,(int)BWAPI::CoordinateType::Mouse,x1,y1,x2,y2,0,0,color,false));
  }
  void GameImpl::drawLineScreen(int x1, int y1, int x2, int y2, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Screen,x1,y1,x2,y2)) return;
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Line,(int)BWAPI::CoordinateType::Screen,x1,y1,x2,y2,0,0,color,false));
  }
  //------------------------------------------------ SCREEN BUFFER -------------------------------------------
  void *GameImpl::getScreenBuffer()
  {
    return BW::BWDATA_GameScreenBuffer->data;
  }
  //--------------------------------------------------- HAS GUI ----------------------------------------------
  bool GameImpl::isGUIEnabled()
  {
    return data->hasGUI;
  }
  //--------------------------------------------------- SET GUI ----------------------------------------------
  void GameImpl::setGUI(bool enabled)
  {
    if ( !this->tournamentCheck(Tournament::SetGUI, &enabled) )
      return;
    data->hasGUI = enabled;
    if ( enabled )
      setFrameSkip();
    else
      setFrameSkip(999999); // IT'S OVER NINE THOUSAND!!11one11one111eleven21
  }
  inline void swap(int& a0, int& a1)
  {
    int tmpx = a0;
    a0 = a1;
    a1 = tmpx;
  }
  // Fixed precision conversions
  // Uses * and / for compatibility with negative numbers
  #define fixed2Int( x ) ( ((x) + (1 << 15)) / (1 << 16) )
  #define int2Fixed( x ) ( (x) * (1 << 16) )
  #define positive( x ) ( (x) > 1 ? (x) : 1 )
  #define incx() x2++, dxt += d2xt, t += dxt
  #define incy() y2--, dyt += d2yt, t += dyt
  //--------------------------------------------- DRAW SHAPES ------------------------------------------------
  int GameImpl::drawShapes()
  {
    for ( int i = 0; i < data->shapeCount; i++ )
    {
      BWAPIC::ShapeType::Enum s = data->shapes[i].type;
      int x1 = data->shapes[i].x1;
      int y1 = data->shapes[i].y1;
      int x2, y2, x3, y3, ly, ry, w, h, lx, rx, dx1, dx2, dx3;
      int radius, f, ddF_x, ddF_y, xi, yi;
      int xrad, yrad, a2, b2, crit1, crit2, crit3, t, dxt, dyt, d2xt, d2yt, twoAsquare, twoBsquare, xchange, ychange, ellipseerror, stoppingX, stoppingY;
      int ctype = data->shapes[i].ctype;
      bool isSolid = data->shapes[i].isSolid;
      BWAPI::Color color = Color(data->shapes[i].color);
      switch ( s )
      {
        case BWAPIC::ShapeType::Text:
           bwDrawText(x1,y1,data->strings[data->shapes[i].extra1],ctype,(char)data->shapes[i].extra2);
           break;
        case BWAPIC::ShapeType::Box:
          x2 = data->shapes[i].x2;
          y2 = data->shapes[i].y2;
          w = abs(x2 - x1);
          h = abs(y2 - y1);
          if (isSolid)
          {
            bwDrawBox(x1, y1, w, h, color, ctype);
          }
          else
          {
            bwDrawBox(  x1,   y1, 1, h, color, ctype);
            bwDrawBox(  x1,   y1, w, 1, color, ctype);
            bwDrawBox(x2-1,   y1, 1, h, color, ctype);
            bwDrawBox(  x1, y2-1, w, 1, color, ctype);
          }
          break;
        case BWAPIC::ShapeType::Triangle:
          x2 = data->shapes[i].x2;
          y2 = data->shapes[i].y2;
          x3 = data->shapes[i].extra1;
          y3 = data->shapes[i].extra2;
          if(isSolid)
          {
            if(y1 > y2) { swap(x1, x2); swap(y1, y2); }
            if(y1 > y3) { swap(x1, x3); swap(y1, y3); }
            if(y2 > y3) { swap(x2, x3); swap(y2, y3); }

            if (y2 - y1 > 0) dx1 = int2Fixed(x2 - x1) / (y2 - y1); else dx1 = int2Fixed(x2 - x1);
            if (y3 - y1 > 0) dx2 = int2Fixed(x3 - x1) / (y3 - y1); else dx2 = 0;
            if (y3 - y2 > 0) dx3 = int2Fixed(x3 - x2) / (y3 - y2); else dx3 = 0;

            rx = lx = int2Fixed(x1);
            ry = ly = y1;
            if(dx1 > dx2)
            {
              for(; ly <= y2; ly++, ry++, lx += dx2, rx += dx1)
                bwDrawBox(fixed2Int(lx), ly, positive(fixed2Int(rx) - fixed2Int(lx)), 1, color, ctype);
              rx = int2Fixed(x2); ry = y2;
              for(; ly <= y3; ly++, ry++, lx += dx2, rx += dx3)
                bwDrawBox(fixed2Int(lx), ly, positive(fixed2Int(rx) - fixed2Int(lx)), 1, color, ctype);
            }
            else
            {
              for(; ly <= y2; ly++, ry++, lx += dx1, rx += dx2)
                bwDrawBox(fixed2Int(lx), ly, positive(fixed2Int(rx) - fixed2Int(lx)), 1, color, ctype);
              lx = int2Fixed(x2); ly = y2;
              for(; ly <= y3; ly++, ry++, lx += dx3, rx += dx2)
                bwDrawBox(fixed2Int(lx), ly, positive(fixed2Int(rx) - fixed2Int(lx)), 1, color, ctype);
            }
          }
          else
          {
            bwDrawLine(x1, y1, x2, y2, color, ctype);
            bwDrawLine(x1, y1, x3, y3, color, ctype);
            bwDrawLine(x2, y2, x3, y3, color, ctype);
          }
          break;
        case BWAPIC::ShapeType::Circle:
          radius = data->shapes[i].extra1;
          if (isSolid)
          {
            f = 1 - radius;
            ddF_x = 1;
            ddF_y = -2 * radius;
            xi = 0;
            yi = radius;

            bwDrawDot(x1, y1 + radius, color, ctype);
            bwDrawDot(x1, y1 - radius, color, ctype);
            bwDrawBox(x1 - radius, y1, radius*2, 1, color, ctype);

            while(xi < yi)
            {
              if(f >= 0)
              {
                yi--;
                ddF_y += 2;
                f += ddF_y;
              }
              xi++;
              ddF_x += 2;
              f += ddF_x;
              bwDrawBox(x1 - xi, y1 - yi, xi*2 + 1, 1, color, ctype);
              bwDrawBox(x1 - xi, y1 + yi, xi*2 + 1, 1, color, ctype);
              bwDrawBox(x1 - yi, y1 - xi, yi*2 + 1, 1, color, ctype);
              bwDrawBox(x1 - yi, y1 + xi, yi*2 + 1, 1, color, ctype);
            }
          }
          else
          {
            f = 1 - radius;
            ddF_x = 1;
            ddF_y = -2 * radius;
            xi = 0;
            yi = radius;

            bwDrawDot(x1, y1 + radius, color, ctype);
            bwDrawDot(x1, y1 - radius, color, ctype);
            bwDrawDot(x1 + radius, y1, color, ctype);
            bwDrawDot(x1 - radius, y1, color, ctype);

            while(xi < yi)
            {
              if(f >= 0)
              {
                yi--;
                ddF_y += 2;
                f += ddF_y;
              }
              xi++;
              ddF_x += 2;
              f += ddF_x;
              bwDrawDot(x1 + xi, y1 + yi, color, ctype);
              bwDrawDot(x1 - xi, y1 + yi, color, ctype);
              bwDrawDot(x1 + xi, y1 - yi, color, ctype);
              bwDrawDot(x1 - xi, y1 - yi, color, ctype);
              bwDrawDot(x1 + yi, y1 + xi, color, ctype);
              bwDrawDot(x1 - yi, y1 + xi, color, ctype);
              bwDrawDot(x1 + yi, y1 - xi, color, ctype);
              bwDrawDot(x1 - yi, y1 - xi, color, ctype);
            }
          }
          break;
        case BWAPIC::ShapeType::Ellipse:
          xrad = data->shapes[i].extra1;
          yrad = data->shapes[i].extra2;
          if (xrad != 0 && yrad != 0)
          {
            if (isSolid)
            {
              x2 = 0, y2 = yrad;
              w = 1;
              a2 = xrad * xrad;
              b2 = yrad * yrad;
              crit1 = -(a2 / 4 + xrad % 2 + b2);
              crit2 = -(b2 / 4 + yrad % 2 + a2);
              crit3 = -(b2 / 4 + yrad % 2);
              t = - a2 * y2;
              dxt = 2 * b2 * x2;
              dyt = -2 * a2 * y2;
              d2xt = 2 * b2;
              d2yt = 2 * a2;

              while (y2 >= 0 && x2 <= xrad)
              {
                if (t + b2*x2 <= crit1 ||
                    t + a2*y2 <= crit3)
                {
                  incx();
                  w += 2;
                }
                else if (t - a2*y2 > crit2)
                {
                  bwDrawBox(x1 - x2, y1 - y2, w, 1, color, ctype);
                  if (y2 != 0)
                    bwDrawBox(x1 - x2, y1 + y2, w, 1, color, ctype);
                  incy();
                }
                else
                {
                  bwDrawBox(x1 - x2, y1 - y2, w, 1, color, ctype);
                  if (y2 != 0)
                    bwDrawBox(x1 - x2, y1 + y2, w, 1, color, ctype);
                  incx();
                  incy();
                  w += 2;
                }
              }
              if (yrad == 0)
                bwDrawBox(x1 - xrad, y1, 2*xrad + 1, 1, color, ctype);
            }
            else
            {
              x2 = xrad;
              y2 = 0;
              twoAsquare = 2 * xrad * xrad;
              twoBsquare = 2 * yrad * yrad;
              xchange = yrad * yrad * (1 - 2 * xrad);
              ychange = xrad * xrad;
              ellipseerror = 0;
              stoppingX = twoBsquare * xrad;
              stoppingY = 0;

              while (stoppingX >= stoppingY)
              {
                bwDrawDot(x1 + x2, y1 + y2, color, ctype);
                bwDrawDot(x1 - x2, y1 + y2, color, ctype);
                bwDrawDot(x1 - x2, y1 - y2, color, ctype);
                bwDrawDot(x1 + x2, y1 - y2, color, ctype);

                y2++;
                stoppingY += twoAsquare;
                ellipseerror += ychange;
                ychange += twoAsquare;
                if ((2 * ellipseerror + xchange) > 0)
                {
                  x2--;
                  stoppingX -= twoBsquare;
                  ellipseerror += xchange;
                  xchange += twoBsquare;
                }
              }
              x2 = 0;
              y2 = yrad;
              xchange = yrad * yrad;
              ychange = xrad * xrad * (1 - 2 * yrad);
              ellipseerror = 0;
              stoppingX = 0;
              stoppingY = twoAsquare * yrad;
              while (stoppingX <= stoppingY)
              {

                bwDrawDot(x1 + x2, y1 + y2, color, ctype);
                bwDrawDot(x1 - x2, y1 + y2, color, ctype);
                bwDrawDot(x1 - x2, y1 - y2, color, ctype);
                bwDrawDot(x1 + x2, y1 - y2, color, ctype);

                x2++;
                stoppingX += twoBsquare;
                ellipseerror += xchange;
                xchange += twoBsquare;
                if ((2 * ellipseerror + ychange) > 0)
                {
                  y2--;
                  stoppingY -= twoAsquare;
                  ellipseerror += ychange;
                  ychange += twoAsquare;
                }
              }
            }
          }
          else if ( xrad == 0 && yrad == 0)
          {
            bwDrawDot(x1, y1, color, ctype);
          }
          else if ( xrad == 0)
          {
            bwDrawBox(x1, y1 - yrad, 1, yrad*2, color, ctype);
          }
          else //if ( yrad == 0)
          {
            bwDrawBox(x1 - xrad, y1, xrad*2, 1, color, ctype);
          }
          break;
        case BWAPIC::ShapeType::Dot:
          bwDrawDot(x1, y1, color, ctype);
          break;
        case BWAPIC::ShapeType::Line:
          x2 = data->shapes[i].x2;
          y2 = data->shapes[i].y2;
          if ( x1 == x2 && y1 == y2 )
            bwDrawDot(x1, y1, color, ctype);
          else if ( x1 == x2 )
            bwDrawBox(x1, y1 < y2 ? y1 : y2, 1, abs(y2 - y1), color, ctype);
          else if ( y1 == y2 )
            bwDrawBox(x1 < x2 ? x1 : x2, y1, abs(x2 - x1), 1, color, ctype);
          else
            bwDrawLine(x1, y1, x2, y2, color, ctype);
          break;
        default:
          break;
      }
    }
    return data->shapeCount;
  }
};