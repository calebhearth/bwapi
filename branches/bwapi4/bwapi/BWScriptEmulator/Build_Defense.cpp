#include "Build_Defense.h"
#include "Controller.h"

using namespace AISCRIPT;

Build_Defense build_bunkers_impl(AISCRIPT::Enum::BUILD_BUNKERS);
Build_Defense build_turrets_impl(AISCRIPT::Enum::BUILD_TURRETS);

bool Build_Defense::execute(aithread &thread) const
{
  thread.saveDebug(Text::Red, this->getOpcode());
  return true;
}
