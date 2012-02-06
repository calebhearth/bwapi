#pragma once
#include <BWAPI/Vectorset.h>

namespace BWAPI
{
	class Region;
	class Regionset : public Vectorset<BWAPI::Region*>
	{
	public:
		Regionset(size_t initialSize = 16, size_t maxSizeHint = 1700);
		Regionset(const Regionset &other);
	};
}

