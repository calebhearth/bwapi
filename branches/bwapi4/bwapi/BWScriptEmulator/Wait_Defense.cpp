#include "Wait_Defense.h"
#include "Controller.h"

using namespace AISCRIPT;

Wait_Defense build_bunkers_impl(AISCRIPT::Enum::WAIT_BUNKERS);
Wait_Defense build_turrets_impl(AISCRIPT::Enum::WAIT_TURRETS);

bool Wait_Defense::execute(aithread &thread) const
{
  thread.saveDebug(Text::Red, this->getOpcode());
  return true;
}
