#pragma once
/*  clamp.h
    clamping a value to be between a specified min and max value
*/

template <typename _T>
static _T clamp(const _T &val, const _T &low, const _T &high)
{
  return val < low ? low : (val > high ? high : val);
}


