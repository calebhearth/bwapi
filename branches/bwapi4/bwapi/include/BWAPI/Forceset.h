#pragma once
#include <BWAPI/Vectorset.h>

namespace BWAPI
{
	class Force;
	class Forceset : public Vectorset<BWAPI::Force*>
	{
	public:
		Forceset(size_t initialSize = 16);
		Forceset(const Forceset &other);
	};
}

