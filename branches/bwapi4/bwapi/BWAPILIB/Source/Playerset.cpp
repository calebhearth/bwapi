#include <BWAPI/Vectorset.h>
#include <BWAPI/Playerset.h>

namespace BWAPI
{
	Playerset::Playerset(size_t initialSize) : Vectorset(initialSize) { };
	Playerset::Playerset(const Playerset &other) : Vectorset(other) { };


}

