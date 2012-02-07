#include <BWAPI/Vectorset.h>
#include <BWAPI/Unitset.h>

namespace BWAPI
{
	Unitset::Unitset(size_t initialSize) : Vectorset(initialSize) { };
	Unitset::Unitset(const Unitset &other) : Vectorset(other) { };


}

