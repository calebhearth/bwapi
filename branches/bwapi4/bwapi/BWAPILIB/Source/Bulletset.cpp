#include <BWAPI/Vectorset.h>
#include <BWAPI/Bulletset.h>

namespace BWAPI
{
  Bulletset::Bulletset(size_t initialSize, size_t maxSizeHint) : Vectorset(initialSize, maxSizeHint) { };
  Bulletset::Bulletset(const Bulletset &other) : Vectorset(other) { };


}

