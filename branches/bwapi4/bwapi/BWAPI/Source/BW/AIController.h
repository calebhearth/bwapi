#pragma once
#include <Windows.h>

#include "Position.h"

namespace BW
{
  // forwards
  class Unit;

  class Captain
  {
  public:
    /* 0x00 */ WORD  wRegionId;
    /* 0x02 */ WORD  wUnk_02;
    /* 0x04 */ BYTE bPlayerId;
    /* 0x05 */ BYTE bCaptainType;
      /*    0
          1  - military (attack group)
          2  - military (attack group)
          3
          4  - implode/help_iftrouble (aiscript)
          5  - guard
          6  - campaign guard
          7
          8  - military (attack group, clear)
          9  - military (attack group)
      */
    /* 0x06 */ WORD wUnk_06;
    /* 0x08 */ BYTE bCaptainFlags;
      /*    
          0x02 - build bunkers
          0x10 - build turrets
      */
  };

  /* NOTES:  BYTE rgnData[2500];
          // 6 = neighbor of high-priority rgn contains nothing
          // 5 = contains own structures, or units if no completed structures owned
          // 4 = neighbor of high-priority enemy rgn contains nothing
          // 3 = neighbor of high-priority rgn containing enemies was acknowledged
          // 2 = neighbor of high-priority rgn contains enemies
          // 1 = contains visible enemy unit

  */
  class Town
  {
  public:

  };

  class CAIController
  {
  public:
    /* 0x00 */ CAIController *prev;
    /* 0x04 */ CAIController *next;
    
    /* 0x08 */ BYTE bType;
    /*      military/standard = 1,
          worker = 2,
          task = 3,

          unknown (additional structure) = 4
    */
    union
    {
      struct
      {
        /* 0x09 */ BYTE counter;
        /* 0x0A */ BYTE unk_0A;
        /* 0x0B */ BYTE unk_0B;
        /* 0x0C */ Unit *pUnit;
        /* 0x10 */ WORD unk_10;
        /* 0x12 */ Position desiredLocation;
      } type1;

      // Used for workers
      struct
      {
        /* 0x09 */ BYTE    unk_09;
        /* 0x0A */ BYTE    unk_0A;
        /* 0x0B */ BYTE    bWaitTimer;  // wait timer before using this unit
        /* 0x0C */ DWORD  dwLastUpdateTime;
        /* 0x10 */ DWORD  unk_10;
        /* 0x14 */ Town    *pTown; // ptr to town struct
      } worker;

      struct 
      {
        /* 0x09 */ BYTE unitQueue[5];
        /* 0x0E */ WORD unk_0E;
        /* 0x10 */ DWORD unk_10;
        /* 0x14 */ DWORD unkCurrentQueue;
        /* 0x18 */ DWORD unkQueue[5];
      } task;

      struct
      {
        /* 0x09 */ BYTE    unk_09;
        /* 0x0A */ BYTE    unk_0A;
        /* 0x0B */ BYTE    unk_0B;
        /* 0x0C */ Unit    *pUnit;
        /* 0x10 */ Captain  *captain;
      } type4;
    } u;
  };
}
