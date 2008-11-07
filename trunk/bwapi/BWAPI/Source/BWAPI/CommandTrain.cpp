#include "CommandTrain.h"
#include "Unit.h"
#include "Player.h"
#include <BW/UnitType.h>
#include <BW/Unit.h>
#include <Util/Logger.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandTrain::CommandTrain(Unit* building, BW::UnitType toTrain)
  :Command(building)
  ,toTrain(toTrain)
  {
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  CommandTrain::~CommandTrain()
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandTrain::execute()
  {
   if (toTrain == BW::UnitID::Terran_NuclearMissile)
   {
     this->executors[0]->getRawDataLocal()->orderID = BW::OrderID::NukeTrain;
     return;
   }
   int slotToAffect = this->executors[0]->getBuildQueueSlotLocal();
   if (this->executors[0]->getBuildQueueLocal()[slotToAffect] != BW::UnitID::None)
      slotToAffect  = (slotToAffect + 1) % 5;
   
   if (this->executors[0]->getBuildQueueLocal()[slotToAffect] != BW::UnitID::None)
   {
     this->failed = true;
     return;
   }
 
   executors[0]->getBuildQueueLocal()[slotToAffect] = this->toTrain.getID();
   this->executors[0]->getOwner()->spendLocal(this->toTrain.getMineralPrice(),
                                              this->toTrain.getGasPrice());
   this->executors[0]->getRawDataLocal()->buildQueueSlot = slotToAffect;
   executors[0]->getOwner()->useSuppliesLocal(toTrain.getSupplies(), toTrain.getRace());
   executors[0]->getOwner()->toMake[toTrain.getID()]++;
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  BWAPI::CommandTypes::Enum CommandTrain::getType()
  {
    return CommandTypes::Train;
  }
  //----------------------------------------------------------------------------------------------------------
  std::string CommandTrain::describe()
  {
    std::string toReturn = this->executors[0]->getName() + " started to build (" + toTrain.getName() + ")";
    return toReturn;
  }
  //----------------------------------------------------------------------------------------------------------
}