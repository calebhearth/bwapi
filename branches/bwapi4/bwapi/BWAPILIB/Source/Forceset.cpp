#include <BWAPI/Vectorset.h>
#include <BWAPI/Forceset.h>

#include <utility>

namespace BWAPI
{
  Forceset::Forceset(size_t initialSize) : Vectorset(initialSize) { };
  Forceset::Forceset(const Forceset &other) : Vectorset(other) { };
  Forceset::Forceset(Forceset &&other) : Vectorset( std::forward<Forceset>(other) ) { };


}

