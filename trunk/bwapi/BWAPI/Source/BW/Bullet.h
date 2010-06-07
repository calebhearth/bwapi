#pragma once

#include <Util/Bitmask.h>
#include <Util/Types.h>
#include "../StaticAssert.h"
#include <BW/Position.h>

#include <BW/Offsets.h>

namespace BW
{
#pragma pack(1)
  struct Bullet
  {
    /*0x00*/ BW::Bullet*  nextBullet;
    /*0x04*/ BW::Bullet*  previousBullet;
    /*0x08*/ u32          unknown_0x08; //usually holds 1
    /*0x0C*/ u32          unknown_0x0C; //address, not in unit table
    /*0x10*/ u32          unknown_0x10; //address? animation related? changes a during the same bullet
    /*0x14*/ u32          unknown_0x14; //usually holds 0
    /*0x18*/ BW::Position targetPosition;
    /*0x1C*/ BW::Position targetPosition2; //usually same as targetPosition
    /*0x20*/ u8           unknown_0x20; //usually holds 0x09 for probes, 0x0b for dragoons
    /*0x21*/ u8           direction1; //not sure how this and direction2 differ from currentDirection and velocityDirection
    /*0x22*/ u8           unknown_0x22; //usually holds 0x00 for probes, 0x80 for dragoons
    /*0x23*/ u8           direction2;
    /*0x24*/ u16          type;
    /*0x26*/ u16          unknown_0x26; //0x0200 for probes, 0x0100 for dragoons
    /*0x28*/ BW::Position position;
    /*0x2C*/ s32          pos4_x; // (pos4_x >> 4) == pos_x
    /*0x30*/ s32          pos4_y;
    /*0x34*/ s32          unknown_0x34; //might be the maximum value of unknown_0x38 or unknown_0x3C
    /*0x38*/ s32          unknown_0x38; //increasing counter or timer
    /*0x3C*/ s32          unknown_0x3C; //increasing counter or timer, often the same value as unknown_0x38
    /*0x40*/ s32          current_speedX;
    /*0x44*/ s32          current_speedY;
    /*0x48*/ u8           unknown_0x48; //0x9b for vulture & wraiths; 0x0b for valkyries, goliath air attack, & missile turret; 0x55 for battle cruiser, 0x00 for marines, etc
    /*0x49*/ u8           unknown_0x49; //0x02 for vulture & wraiths; 0x01 for valkyries, goliath air attack, & missile turret; 0x21 for battle cruiser, 0x00 for marines, etc
    /*0x4A*/ u8           currentDirection;
    /*0x4B*/ u8           velocityDirection; //seems to always be the same as currentDirection
    /*0x4C*/ s32          unknown_0x4C; //200, 400, or 500 - possibly some sort of state flags
    /*0x50*/ s32          unknown_0x50; //usually holds e4
    /*0x54*/ s32          unknown_0x54; //usually holds 0
    /*0x58*/ s32          unknown_0x58; //same as unknown_0x10 for dragoons
    /*0x5C*/ BW::Unit*    targetUnit;
    /*0x60*/ s32          time_left; // use time_left>>8 for frames
    /*0x64*/ BW::Unit*    sourceUnit;
    /*0x68*/ u32          unknown_0x68; //usually holds 0x00000000
    /*0x6C*/ u32          unknown_0x6C; //some sort of cyclic burst counter (used by valkyries, goliath air attack)
  };
#pragma pack()
  /** Direct maping of the BulletNodeTable in bw memory. */
  struct BulletArray
  {
    Bullet bullet[BULLET_ARRAY_MAX_LENGTH];
  };
};