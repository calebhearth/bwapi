#include <BWAPI/Vectorset.h>
#include <BWAPI/Regionset.h>

namespace BWAPI
{
  Regionset::Regionset(size_t initialSize, size_t maxSizeHint) : Vectorset(initialSize, maxSizeHint) { };
  Regionset::Regionset(const Regionset &other) : Vectorset(other) { };


}

