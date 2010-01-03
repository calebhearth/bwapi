#pragma once

/**
 *  When BWAPI calls the AI module, it's for one of these purposes
 */

namespace BWAPI
{
  namespace CallTypeIds
  {
    enum Enum
    {
      OnDisconnect,
      OnFrame,
      OnMatchStart
    };
  } typedef CallTypeIds::Enum CallTypeId;
}
