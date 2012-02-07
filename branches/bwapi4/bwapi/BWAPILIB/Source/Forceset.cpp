#include <BWAPI/Vectorset.h>
#include <BWAPI/Forceset.h>

namespace BWAPI
{
	Forceset::Forceset(size_t initialSize) : Vectorset(initialSize) { };
	Forceset::Forceset(const Forceset &other) : Vectorset(other) { };


}

