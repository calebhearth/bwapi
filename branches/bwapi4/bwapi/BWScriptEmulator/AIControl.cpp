#include "AIControl.h"

/*  [ClientInfo values]

      CTRL: The AI controller type
      TIME: A timer
      TOWN: The town ID that the unit belongs to
      ORDR: The emulated order ID
*/

void PerformIndividualUnitControl(BWAPI::Unit *pUnit)
{
  // safety
  if ( !pUnit ) return;

  switch ( (int)pUnit->getClientInfo('CTRL') )
  {
  case 0: // None/Unassigned
    break;
  case 1: // Tracker?
    break;
  case 2: // Worker
    break;
  case 3: // Task
    break;
  case 4: // Military
    break;
  default:
    break;
  }
}
