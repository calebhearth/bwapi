#include "CommandCancelMorph.h"
#include "UnitImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandCancelMorph::CommandCancelMorph(UnitImpl* executor)
      : Command(executor)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandCancelMorph::execute()
  {
    for (unsigned int i = 0; i < this->executors.size(); i++)
    {
      this->executors[i]->getRawDataLocal()->orderID = BW::OrderID::Nothing2;
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  BWAPI::CommandTypes::Enum CommandCancelMorph::getType()
  {
    return BWAPI::CommandTypes::CancelMorph;
  }
  //----------------------------------------------------------------------------------------------------------
  std::string CommandCancelMorph::describe()
  {
    return this->executors[0]->getName() + " cancelled morph";
  }
  //----------------------------------------------------------------------------------------------------------
}