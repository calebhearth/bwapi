#pragma once
namespace BWAPI
{
  namespace Latency
  {
    enum Enum : unsigned __int8
    {
      SinglePlayer    = 2,
      LanLow          = 5,
      LanMedium       = 7,
      LanHigh         = 9,
      BattlenetLow    = 14,
      BattlenetMedium = 19,
      BattlenetHigh   = 24
    };
  }
}