#include <BWAPI/Vectorset.h>
#include <BWAPI/Bulletset.h>

namespace BWAPI
{
  Bulletset::Bulletset(size_t initialSize) : Vectorset(initialSize) { };
  Bulletset::Bulletset(const Bulletset &other) : Vectorset(other) { };
  Bulletset::Bulletset(const Bulletset &&other) : Vectorset(other) { };

}

