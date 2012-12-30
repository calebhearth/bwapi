#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Build_Defense : public Opcode
  {
  public:
    // Ctor
    Build_Defense(AISCRIPT::Enum::Enum n) : Opcode(n) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
