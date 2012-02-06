#include <BWAPI/Vectorset.h>
#include <BWAPI/Unitset.h>

namespace BWAPI
{
	Unitset::Unitset(size_t initialSize, size_t maxSizeHint) : Vectorset(initialSize, maxSizeHint) { };
	Unitset::Unitset(const Unitset &other) : Vectorset(other) { };


}

