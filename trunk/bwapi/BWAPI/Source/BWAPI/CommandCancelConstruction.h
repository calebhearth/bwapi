#pragma once
#include "Command.h"
namespace BWAPI { class UnitImpl; }
namespace BWAPI
{
  /** BWAPI internal representation of Cancel Construction command. */
  class CommandCancelConstruction : public Command
  {
    public :
      /**
       * @param executor Executor of the Cancel Construction command (will be passed as executor
                to Command ancestor)
       */
      CommandCancelConstruction(UnitImpl* building);
      int getType();
      virtual void execute();
      int startFrame;
  };
};
