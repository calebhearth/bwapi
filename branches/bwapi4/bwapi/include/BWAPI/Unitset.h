#pragma once
#include <BWAPI/Vectorset.h>
#include <BWAPI/Position.h>
#include <BWAPI/Regionset.h>

namespace BWAPI
{
	class Unit;
	class Unitset : public Vectorset<BWAPI::Unit*>
	{
	public:
		// Ctors
		Unitset(size_t initialSize = 16);
		Unitset(const Unitset &other);

		// Members
		// const members

		// Average position
		Position getPosition() const;

		// sets
		Unitset getLoadedUnits() const;
		Unitset getInterceptors() const;
		Unitset getLarva() const;

		// non-const
		void setClientInfo(void *clientInfo);


	};
}

