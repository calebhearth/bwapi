#pragma once
#include <BWAPI/Vectorset.h>

namespace BWAPI
{
  class Bullet;
  class Bulletset : public Vectorset<BWAPI::Bullet*>
  {
  public:
    Bulletset(size_t initialSize = 16, size_t maxSizeHint = 1700);
    Bulletset(const Bulletset &other);
  };
}

