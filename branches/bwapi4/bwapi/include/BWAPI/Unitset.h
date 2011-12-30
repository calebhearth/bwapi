#pragma once
#include <BWAPI/Vectorset.h>
#include <BWAPI/Unit.h>

namespace BWAPI
{
  class Unitset : public Vectorset<BWAPI::Unit*>
  {
  public:
    Unitset(size_t initialSize = 16, size_t maxSizeHint = 1700);
    Unitset(const Unitset &other);
  };
}

