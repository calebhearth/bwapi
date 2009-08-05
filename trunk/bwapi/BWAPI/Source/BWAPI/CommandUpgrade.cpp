#include "CommandUpgrade.h"

#include <Util/Logger.h>

#include "UnitImpl.h"
#include "PlayerImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandUpgrade::CommandUpgrade(UnitImpl* building, BW::UpgradeType upgrade)
      : Command(building)
      , upgrade(upgrade)
  {
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  CommandUpgrade::~CommandUpgrade()
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandUpgrade::execute()
  {
    if (this->executors[0]->isCompleted())
      this->executors.front()->getRawDataLocal()->orderID = BW::OrderID::Upgrade;
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  BWAPI::CommandTypes::Enum CommandUpgrade::getType()
  {
    return CommandTypes::Upgrade;
  }
  //----------------------------------------------------------------------------------------------------------
  std::string CommandUpgrade::describe()
  {
    return this->executors[0]->getName() + " will upgrade (" + upgrade.getName() + ")";
  }
  //----------------------------------------------------------------------------------------------------------
}