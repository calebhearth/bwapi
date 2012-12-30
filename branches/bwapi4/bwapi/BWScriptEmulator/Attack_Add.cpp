#include "Attack_Add.h"
#include <tuple>

#include "Controller.h"

using namespace AISCRIPT;

IMPLEMENT(Attack_Add);

bool Attack_Add::execute(aithread &thread) const
{
  // Retrieve parameters
  BYTE bCount;
  WORD wUnitType;
  thread.readTuple( std::tie(bCount, wUnitType) );

  // Execution
  MainController.AttackAdd(bCount, wUnitType);

  // Save debug info and return
  thread.saveDebug(Text::Green, this->getOpcode(), "%u %s", bCount, AISCRIPT::getUnitName(wUnitType));
  return true;
}
