#include <BWAPI/Vectorset.h>
#include <BWAPI/Regionset.h>

namespace BWAPI
{
	Regionset::Regionset(size_t initialSize) : Vectorset(initialSize) { };
	Regionset::Regionset(const Regionset &other) : Vectorset(other) { };


}

