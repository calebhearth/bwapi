#pragma once
#include "Type.h"

#define BWAPI_ERRORDEF(x) static const Error x(Enum::x) /** ref x */

namespace BWAPI
{
  class UnitType;

  namespace Errors
  {
    /// Enumeration of Error types
    namespace Enum
    {
      enum Enum
      {
        Unit_Does_Not_Exist = 0,
        Unit_Not_Visible,
        Unit_Not_Owned,
        Unit_Busy,
        Incompatible_UnitType,
        Incompatible_TechType,
        Incompatible_State,
        Already_Researched,
        Fully_Upgraded,
        Currently_Researching,
        Currently_Upgrading,
        Insufficient_Minerals,
        Insufficient_Gas,
        Insufficient_Supply,
        Insufficient_Energy,
        Insufficient_Tech,
        Insufficient_Ammo,
        Insufficient_Space,
        Invalid_Tile_Position,
        Unbuildable_Location,
        Unreachable_Location,
        Out_Of_Range,
        Unable_To_Hit,
        Access_Denied,
        File_Not_Found,
        Invalid_Parameter,
        None,
        Unknown,
        MAX
      };
    };
  };
  /// @~English
  /// The Error object is generally used to determine
  /// why certain functions in BWAPI have failed.
  ///
  /// For example, you may not have enough resources
  /// to construct a unit.
  /// @~
  /// @see Game::getLastError, Game::setLastError
  class Error : public Type<Error, Errors::Enum::Unknown>
  {
  public:
    Error(int id = Errors::Enum::None);

    /// @~English
    /// Retrieves the name of the error.
    ///
    /// @returns std::string object containing the name.
    /// @~
    std::string toString() const;

    /// @copydoc getName
    /// @~English
    /// @returns Pointer to c-style string.
    /// @~
    const char *c_str() const;
  };
  namespace Errors
  {
    /** Given the name of an error, this function will return the error code. For example:
     * Errors::getError("Unbuildable Location") will return Errors::Unbuildable_Location?. */
    Error getError(std::string name);

    /** The set of all the error codes. */
    const Error::set& allErrors();

    void init();

    /** Returned if you try to order a unit or get information from a unit that no longer exists. */
    BWAPI_ERRORDEF(Unit_Does_Not_Exist);

    /** Returned if you try to retrieve information about a unit that is not currently visible or is dead. */
    BWAPI_ERRORDEF(Unit_Not_Visible);

    /** Returned when attempting to order a unit that BWAPI does not own (i.e. can't order enemy army to go
     *  away) */
    BWAPI_ERRORDEF(Unit_Not_Owned);

    /** Returned when trying to order a unit to do something when it is performing another order or is in a
     * state which prevents it from performing the desired order. For example, ordering a Terran Engineering
     * Bay to upgrade something while it is already upgrading something else will return this error.
     * Similarly, trying to train units from a factory that is lifted will return this error. */
    BWAPI_ERRORDEF(Unit_Busy);

    /** Returned if you do something weird like try to build a Pylon with an SCV, or train Vultures in a
     * Barracks, or order a Hydralisk to lay a spider mine. */
    BWAPI_ERRORDEF(Incompatible_UnitType);

    /** Returned when trying to use a tech type with the wrong Unit::useTech method. */
    BWAPI_ERRORDEF(Incompatible_TechType);

    /** Returned if you to do something like try to cancel an upgrade when the unit isn't upgrading. */
    BWAPI_ERRORDEF(Incompatible_State);

    /** Returned if you try to research something that is already researched. */
    BWAPI_ERRORDEF(Already_Researched);

    /** Returned if you try to upgrade something that is already fully upgraded. */
    BWAPI_ERRORDEF(Fully_Upgraded);

    /** Returned if you try to research something that is already being researched. */
    BWAPI_ERRORDEF(Currently_Researching);

    /** Returned if you try to upgrade something that is already being upgraded. */
    BWAPI_ERRORDEF(Currently_Upgrading);

    /** Returned if you try to train or build something without enough minerals. */
    BWAPI_ERRORDEF(Insufficient_Minerals);

    /** Returned if you try to train or build something without enough vespene gas. */
    BWAPI_ERRORDEF(Insufficient_Gas);

    /** Returned if you try to train something without enough supply. */
    BWAPI_ERRORDEF(Insufficient_Supply);

    /** Returned if you to do something like try to order a Defiler to cast a Dark Swarm without enough
     * energy. */
    BWAPI_ERRORDEF(Insufficient_Energy);

    /** Returned if you do something like try to train Medics when you don't have an Academy, or try to lay
     * Spider Mines before spider mines have been researched. */
    BWAPI_ERRORDEF(Insufficient_Tech);

    /** Returned if you do something like try to lay Spider Mines when your Vulture is out of Spider Mines.
     * Same thing with Reavers and Scarabs. */
    BWAPI_ERRORDEF(Insufficient_Ammo);

    /** Returned if you try to train more Interceptors than the Carrier can hold, try to train more Scarabs
      * than a Reaver can hold, or try to load more units into a transport than there is space. */
    BWAPI_ERRORDEF(Insufficient_Space);

    /** Returned if you try to build a barracks at TilePositions::None or something similar */
    BWAPI_ERRORDEF(Invalid_Tile_Position);

    /** Returned if you try to construct a building on an unbuildable location */
    BWAPI_ERRORDEF(Unbuildable_Location);

    /** Returned if you try to construct a building where the worker cannot reach based on static map data. */
    BWAPI_ERRORDEF(Unreachable_Location);

    /** Returned if you order an immovable unit, like a Protoss Photon Cannon, to attack a unit that is out of
     * range.*/
    BWAPI_ERRORDEF(Out_Of_Range);

    /** Returned if you do something like order a Vulture to attack a flying unit. */
    BWAPI_ERRORDEF(Unable_To_Hit);

    /** Returned if you try to get information that is not allowed with the given flag settings. For example,
     * trying to read the enemy's resource counts while the CompleteMapInformation?  flag is not enabled will
     * return this error. Similarly, trying to read the coordinates of the screen or mouse while the UserInput
     *  flag is not enabled will also return this error. */
    BWAPI_ERRORDEF(Access_Denied);

    /** Used when a file can't be found. */
    BWAPI_ERRORDEF(File_Not_Found);

    /** Used for bad parameters, like passing NULL or an empty string. */
    BWAPI_ERRORDEF(Invalid_Parameter);

    /** Used when no error has been encountered. */
    BWAPI_ERRORDEF(None);

    /** Used when the error code is not recognized or can not be determined. */
    BWAPI_ERRORDEF(Unknown);
  }
}
