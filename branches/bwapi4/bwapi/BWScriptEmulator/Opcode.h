#pragma once
#include <array>
#include "ScriptEnum.h"
#include "ScriptThread.h"

namespace AISCRIPT
{
  class Opcode
  {
  public:
    // Mapping of opcodes
    static std::array<Opcode*,AISCRIPT::LAST> opcodeList;

    // Constructor
    Opcode(AISCRIPT::Enum id);

    // Execute an opcode.
    // Return true if script execution should continue, false if execution should break
    virtual bool execute(aithread &thread) const = 0;

    // Read an opcode from the script file
    // Return true if we should continue reading the script or false if we should return
    // execution to Starcraft.
    static bool readOpcode(aithread &thread);

  private:

  };

}
