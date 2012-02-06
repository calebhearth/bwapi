#include <BWAPI/Vectorset.h>
#include <BWAPI/Forceset.h>

namespace BWAPI
{
	Forceset::Forceset(size_t initialSize, size_t maxSizeHint) : Vectorset(initialSize, maxSizeHint) { };
	Forceset::Forceset(const Forceset &other) : Vectorset(other) { };


}

