#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Wait_Defense : public Opcode
  {
  public:
    // Ctor
    Wait_Defense(AISCRIPT::Enum::Enum n) : Opcode(n) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
