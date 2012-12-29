#pragma once

#include "Opcode.h"

namespace AISCRIPT
{
  class Goto : public Opcode
  {
  public:
    Goto();

    virtual bool execute(aithread &thread) const;

    // also something to debug
  };



}
