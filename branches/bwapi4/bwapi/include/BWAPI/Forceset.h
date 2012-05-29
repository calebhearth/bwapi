#pragma once
#include <BWAPI/Vectorset.h>

namespace BWAPI
{
  // Forward Declarations
  class Force;

  class Forceset : public Vectorset<BWAPI::Force*>
  {
  public:
    Forceset(size_t initialSize = 16);
    Forceset(const Forceset &other);
    Forceset(Forceset &&other);
  };
}

