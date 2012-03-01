#pragma once
#include <BWAPI/Vectorset.h>

namespace BWAPI
{
  class Region;
  class Regionset : public Vectorset<BWAPI::Region*>
  {
  public:
    Regionset(size_t initialSize = 16);
    Regionset(const Regionset &other);
  };
}

