#pragma once
#pragma pack(1)
#include "Sprite.h"

namespace BW { struct CSprite; };

namespace BW
{
  namespace Image
  {
    struct CImage
    {
      CImage  *prev;
      CImage  *next;
      u16     imageID;
      u8      peletteType;
      u8      direction;
      u16     flags;
      s8      horizontalOffset;
      s8      verticalOffset;
      u16     iscriptHeader;
      u16     iscriptOffset;
      u16     unknown_14;
      u8      anim;
      u8      wait;
      u16     frameSet;
      u16     frameIndex;
      u16     unknown_1C;
      u16     unknown_1E;
      u16     unknown_20;
      u16     unknown_22;
      u16     unknown_24;
      u16     unknown_26;
      u16     unknown_28;
      u16     unknown_2A;
      void    *GRPFile;
      void    *coloringData;
      void    *renderFunction1;
      void    *renderFunction2;
      CSprite *spriteOwner;
    };

    namespace Anims
    {
      enum Enum
      {
        Init          = 0x00,
        Death         = 0x01,
        GndAttkInit   = 0x02,
        AirAttkInit   = 0x03,
        Unused1       = 0x04,
        GndAttkRpt    = 0x05,
        AirAttkRpt    = 0x06,
        CastSpell     = 0x07,
        GndAttkToIdle = 0x08,
        AirAttkToIdle = 0x09,
        Unused2       = 0x0A,
        Walking       = 0x0B,
        WalkingToIdle = 0x0C,
        SpecialState1 = 0x0D,
        SpecialState2 = 0x0E,
        AlmostBuilt   = 0x0F,
        Built         = 0x10,
        Landing       = 0x11,
        LiftOff       = 0x12,
        IsWorking     = 0x13,
        WorkingToIdle = 0x14,
        WarpIn        = 0x15,
        Unused3       = 0x16,
        StarEditInit  = 0x17,
        Disable       = 0x18,
        Burrow        = 0x19,
        UnBurrow      = 0x1A,
        Enable        = 0x1B
      };
    };

  };
};
#pragma pack()
