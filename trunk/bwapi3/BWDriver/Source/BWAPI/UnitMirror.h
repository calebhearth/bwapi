#pragma once
/***********************************************************************
 *  The UnitMirror struct is a proxy of the BW::Unit struct, with
 *  additional data for lifetime detection
 *
 *******/

#include <Util\Types.h>

#include <Bridge\SharedStuff.h>

#include <BWAPITypes\UnitId.h>

namespace Bridge { struct KnownUnitEntry; };

namespace BWAPI
{
  struct UnitMirror
  {
    // isInChain is true when this slot is detected to be an element of BroodWars
    // UnitNodeChain in memory. wasInChain is the previus frame's isInChain
    bool            isInChain;
    bool            wasInChain;
    // is true several frames before the unit gets removed from the chain
    bool            isDying;

    // if this unit is known, this is it's personal KnownUnit entry
    KnownUnit* knownUnit;       // saving, to not to have to recompute
    UnitId     knownUnitIndex;  // needed for removing from units
  };
}
