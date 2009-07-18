#include "CommandLift.h"
#include "UnitImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandLift::CommandLift(UnitImpl* executor)
      : Command(executor)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandLift::execute()
  {
    for (unsigned int i = 0; i < this->executors.size(); i++)
    {
      this->executors[i]->getRawDataLocal()->orderID = BW::OrderID::BuildingLiftoff;
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  BWAPI::CommandTypes::Enum CommandLift::getType()
  {
    return BWAPI::CommandTypes::Lift;
  }
  //----------------------------------------------------------------------------------------------------------
  std::string CommandLift::describe()
  {
    return this->executors[0]->getName() + " lifted off";
  }
  //----------------------------------------------------------------------------------------------------------
}