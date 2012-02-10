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

		/// Calculates the average position of all units
		/// in this set.
		///
		/// @returns Average position of all units in the set
		/// @see Unit::getPosition
		Position getPosition() const;

		/// Creates a single set containing all units that
		/// are loaded into units of this set.
		///
		/// @returns The set of all loaded units
		/// @see Unit::getLoadedUnits
		Unitset getLoadedUnits() const;

		/// Creates a single set containing all the
		/// interceptors of Carriers in this set.
		///
		/// @returns The set of all interceptors
		/// @see Unit::getInterceptors
		Unitset getInterceptors() const;

		/// Creates a single set containing all the
		/// larvae of all Hatcheries, Lairs, and
		/// Hives in this set.
		///
		/// @returns The set of all larvae
		/// @see Unit::getLarva
		Unitset getLarva() const;

		/// Sets the client info for every unit
		/// in this set.
		///
		/// @param clientInfo a pointer to client information,
		/// or NULL if no client information is to be specified
		/// @see Unit::setClientInfo
		void setClientInfo(void *clientInfo);

	};
}

