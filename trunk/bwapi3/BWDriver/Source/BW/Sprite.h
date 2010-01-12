#pragma once
#include "Position.h"
namespace BW
{
  struct CSprite
  {
    CSprite* prev;
    CSprite* next;
    u16 spriteID;
    u8 color;
    u8 unknown1;
    u8 visibilityFlags;
    u8 elevationLevel;
    u8 unknown2;
    u8 selectionTimer;
    u32 index;
    BW::Position position;
    u32 CImage_underlay;
    u32 CImage_mainGraphic;
    u32 CImage_overlay;
  };
};

