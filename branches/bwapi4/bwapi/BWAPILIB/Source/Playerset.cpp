#include <BWAPI/Vectorset.h>
#include <BWAPI/Playerset.h>

namespace BWAPI
{
	Playerset::Playerset(size_t initialSize, size_t maxSizeHint) : Vectorset(initialSize, maxSizeHint) { };
	Playerset::Playerset(const Playerset &other) : Vectorset(other) { };


}

