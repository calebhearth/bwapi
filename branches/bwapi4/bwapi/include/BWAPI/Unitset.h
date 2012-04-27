#pragma once
#include <BWAPI/Vectorset.h>
#include <BWAPI/Position.h>
#include <BWAPI/PositionUnit.h>

namespace BWAPI
{
  // Forward declarations
  class Unit;
  class UnitType;
  class Regionset;
  class UnitCommand;
  class TechType;

  /// @~English
  /// The Unitset is a container for a set of pointers
  /// to Unit objects. It is typically used for groups
  /// of units instead of having to manage each Unit
  /// individually.
  ///
  /// @note Because of the efficiency, it is recommended
  /// to use the Unitset over any of the STL containers.
  ///
  /// @~
  /// @see Unit, UnitType, UnitSizeType
  class Unitset : public Vectorset<BWAPI::Unit*>
  {
  public:
    /// @~English
    /// The Unitset constructor.
    ///
    /// @param initialSize The initial maximum size of
    /// the Unitset before expanding it is necessary.
    /// @~
    Unitset(size_t initialSize = 16);
    
    /// @overload
    /// @~English 
    ///
    /// @param other The Unitset to construct this one
    /// from.
    /// @~
    Unitset(const Unitset &other);

    /// @~English
    /// Calculates the average of all valid Unit positions
    /// in this set.
    ///
    /// @returns Average Position of all units in the set.
    /// @~
    /// @see Unit::getPosition
    Position getPosition() const;

    /// @~English
    /// Creates a single set containing all units that
    /// are loaded into units of this set.
    ///
    /// @returns The set of all loaded units.
    /// @~
    /// @see Unit::getLoadedUnits
    Unitset getLoadedUnits() const;

    /// @~English
    /// Creates a single set containing all the
    /// @Interceptors of all @Carriers in this set.
    ///
    /// @returns The set of all @Interceptors .
    /// @~
    /// @see Unit::getInterceptors
    Unitset getInterceptors() const;

    /// @~English
    /// Creates a single set containing all the
    /// @Larvae of all @Hatcheries, @Lairs, and
    /// @Hives in this set.
    ///
    /// @returns The set of all @Larvae .
    /// @~
    /// @see Unit::getLarva
    Unitset getLarva() const;

    /// @~English
    /// Sets the client info for every unit
    /// in this set.
    ///
    /// @param clientInfo A pointer to client information,
    /// managed by the AI module, or NULL if client 
    /// information is to be cleared.
    /// @~
    /// @see Unit::setClientInfo
    void setClientInfo(void *clientInfo = NULL);

    /// @copydoc Unit::issueCommand
    bool issueCommand(UnitCommand command);
    
    /// @copydoc Unit::attack
    bool attack(PositionOrUnit target, bool shiftQueueCommand = false);

    /// @copydoc Unit::build
    bool build(UnitType type, TilePosition target = TilePositions::None);

    /// @copydoc Unit::buildAddon
    bool buildAddon(UnitType type);

    /// @copydoc Unit::train
    bool train(UnitType type);

    /// @copydoc Unit::morph
    bool morph(UnitType type);

    /// @copydoc Unit::setRallyPoint
    bool setRallyPoint(PositionOrUnit target);

    /// @copydoc Unit::move
    bool move(Position target, bool shiftQueueCommand = false);

    /// @copydoc Unit::patrol
    bool patrol(Position target, bool shiftQueueCommand = false);

    /// @copydoc Unit::holdPosition
    bool holdPosition(bool shiftQueueCommand = false);

    /// @copydoc Unit::stop
    bool stop(bool shiftQueueCommand = false);

    /// @copydoc Unit::follow
    bool follow(Unit* target, bool shiftQueueCommand = false);

    /// @copydoc Unit::gather
    bool gather(Unit* target, bool shiftQueueCommand = false);

    /// @copydoc Unit::returnCargo
    bool returnCargo(bool shiftQueueCommand = false);

    /// @copydoc Unit::repair
    bool repair(Unit* target, bool shiftQueueCommand = false);

    /// @copydoc Unit::burrow
    bool burrow();

    /// @copydoc Unit::unburrow
    bool unburrow();

    /// @copydoc Unit::cloak
    bool cloak();

    /// @copydoc Unit::decloak
    bool decloak();

    /// @copydoc Unit::siege
    bool siege();

    /// @copydoc Unit::unsiege
    bool unsiege();

    /// @copydoc Unit::lift
    bool lift();

    /// @copydoc Unit::load
    bool load(Unit* target, bool shiftQueueCommand = false);

    /// @copydoc Unit::unloadAll(bool)
    bool unloadAll(bool shiftQueueCommand = false);

    /// @copydoc Unit::unloadAll(Position,bool)
    bool unloadAll(Position target, bool shiftQueueCommand = false);

    /// @copydoc Unit::rightClick
    bool rightClick(PositionOrUnit target, bool shiftQueueCommand = false);

    /// @copydoc Unit::haltConstruction
    bool haltConstruction();

    /// @copydoc Unit::cancelConstruction
    bool cancelConstruction();

    /// @copydoc Unit::cancelAddon
    bool cancelAddon();

    /// @copydoc Unit::cancelTrain
    bool cancelTrain(int slot = -2);

    /// @copydoc Unit::cancelMorph
    bool cancelMorph();

    /// @copydoc Unit::cancelResearch
    bool cancelResearch();

    /// @copydoc Unit::cancelUpgrade
    bool cancelUpgrade();
    
    /// @copydoc Unit::useTech
    bool useTech(TechType tech, PositionOrUnit target = NULL);

    /// @copydoc Unit::placeCOP
    bool placeCOP(TilePosition target);
  };
}

