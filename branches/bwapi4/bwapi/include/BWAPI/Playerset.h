#pragma once
#include <BWAPI/Vectorset.h>

#include <BWAPI/Race.h>
#include <BWAPI/Filters.h>

namespace BWAPI
{
  // Forward Declarations
  class Player;
  class Unitset;

  class Playerset : public Vectorset<BWAPI::Player*>
  {
  public:
    Playerset(size_t initialSize = 16);
    Playerset(const Playerset &other);
    Playerset(const Playerset &&other);

    /// Returns the set of all units that every
    /// player in this set owns.
    ///
    /// @returns Unitset containing Playerset's units
    /// @see Player::getUnits
    Unitset getUnits(const UnitFilter &pred = NULL) const;

    /// Returns the list of races that each player
    /// in the set is.
    ///
    /// @returns Race::set containing Playerset's races
    /// @see Player::getRace
    Race::set getRaces() const;

  };
}

