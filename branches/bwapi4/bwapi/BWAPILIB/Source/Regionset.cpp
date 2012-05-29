#include <BWAPI/Vectorset.h>
#include <BWAPI/Regionset.h>

#include <utility>

namespace BWAPI
{
  Regionset::Regionset(size_t initialSize) : Vectorset(initialSize) { };
  Regionset::Regionset(const Regionset &other) : Vectorset(other) { };
  Regionset::Regionset(Regionset &&other) : Vectorset( std::forward<Regionset>(other) ) { };


}

