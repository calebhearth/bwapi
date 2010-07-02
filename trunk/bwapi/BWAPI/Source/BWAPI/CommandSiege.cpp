#include "CommandSiege.h"
#include "UnitImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandSiege::CommandSiege(UnitImpl* executor)
      : Command(executor)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandSiege::execute()
  {
    if (!executors[0]->_exists) return;
    executors[0]->self->order = BW::OrderID::SiegeMode;
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandSiege::getType()
  {
    return BWAPI::CommandTypes::Siege;
  }
  //----------------------------------------------------------------------------------------------------------
};
