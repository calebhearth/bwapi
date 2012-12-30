#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Attack_Prepare : public Opcode
  {
  public:
    // Ctor
    Attack_Prepare() : Opcode(Enum::ATTACK_PREPARE) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
