#include "Goto.h"

using namespace AISCRIPT;


Goto::Goto() : Opcode(AISCRIPT::GOTO) {};

bool Goto::execute(aithread &thread) const
{

  return true;
}
