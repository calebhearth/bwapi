#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Make_Patrol : public Opcode
  {
  public:
    // Ctor
    Make_Patrol() : Opcode(Enum::MAKE_PATROL) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
