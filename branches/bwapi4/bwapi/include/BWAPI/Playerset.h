#pragma once
#include <BWAPI/Vectorset.h>

namespace BWAPI
{
  class Player;
  class Playerset : public Vectorset<BWAPI::Player*>
  {
  public:
    Playerset(size_t initialSize = 16, size_t maxSizeHint = 1700);
    Playerset(const Playerset &other);
  };
}

