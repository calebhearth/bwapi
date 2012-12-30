#include "Attack_Prepare.h"
#include <tuple>

#include "Controller.h"

using namespace AISCRIPT;

IMPLEMENT(Attack_Prepare);

bool Attack_Prepare::execute(aithread &thread) const
{
  // AI_AttackManager(thisLocation, 1, 0);

  // Save debug info and return
  thread.saveDebug(Text::Red, this->getOpcode());
  return true;
}
