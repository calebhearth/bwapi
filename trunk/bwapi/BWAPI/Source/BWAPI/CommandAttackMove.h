#pragma once
#include "Command.h"
#include <BW/PositionUnitTarget.h>
namespace BWAPI { class UnitImpl; }
namespace BWAPI
{
  /** BWAPI internal representation of Attack Move command. */
  class CommandAttackMove : public Command
  {
    public :
      /**
       * @param executor Executor of the Attack Move command (will be passed as executor
                to Command ancestor)
       * @param targetPosition Target of the Attack Move command.
       */
      CommandAttackMove(UnitImpl* executor, const BW::Position& targetPosition);
      virtual void execute();
      BWAPI::CommandTypes::Enum getType();
      std::string describe();
    private :
      BW::Position targetPosition;
  };
}