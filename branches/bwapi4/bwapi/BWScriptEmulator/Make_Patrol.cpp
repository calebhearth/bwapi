#include "Make_Patrol.h"
#include "Controller.h"

using namespace AISCRIPT;

IMPLEMENT(Make_Patrol);

bool Make_Patrol::execute(aithread &thread) const
{
  // Set generic command target
  Broodwar->getUnitsInRectangle(thread.getLocation().topLeft, thread.getLocation().bottomRight,
                    GetPlayer == Broodwar->self() && IsCompleted).patrol( MainController.genCmdTarget.center() );

  // Debug
  thread.saveDebug(Text::Green, this->getOpcode());
  return true;
}
