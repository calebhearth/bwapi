#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Attack_Add : public Opcode
  {
  public:
    // Ctor
    Attack_Add() : Opcode(Enum::ATTACK_ADD) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
