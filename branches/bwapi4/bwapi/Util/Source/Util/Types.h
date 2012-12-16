#pragma once

typedef unsigned char      u8;
typedef   signed char      s8;
typedef unsigned short     u16;
typedef   signed short     s16;
typedef unsigned int       u32;
typedef   signed int       s32;
typedef unsigned long long u64;
typedef   signed long long s64;
typedef unsigned int       uint;

typedef u8              _UNKNOWN;

typedef unsigned char   BYTE;
typedef unsigned short  WORD;
typedef unsigned long   DWORD;
typedef void*           PVOID;
typedef int             BOOL;
typedef void*           HANDLE;

typedef struct __rect
{
  s16 left;
  s16 top;
  s16 right;
  s16 bottom;
} rect;

#ifdef NULL
#undef NULL
#endif
#define NULL 0

#define ever (;;)

#include <cstdio>

#define MB_ICONINFO MB_ICONASTERISK

#define MemZero(x) memset(&x, 0, sizeof(x))
#define StrTerminate(x) (x[sizeof(x)-1] = '\0')
