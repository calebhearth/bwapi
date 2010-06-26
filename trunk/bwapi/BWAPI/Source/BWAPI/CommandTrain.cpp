#include "CommandTrain.h"
#include "UnitImpl.h"
#include "PlayerImpl.h"
#include <BW/UnitType.h>
#include <BW/Unit.h>
#include <Util/Logger.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandTrain::CommandTrain(UnitImpl* building, BW::UnitType toTrain)
      : Command(building)
      , toTrain(toTrain)
  {
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  CommandTrain::~CommandTrain()
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandTrain::execute()
  {
    if (!this->executors[0]->_exists) return;
    if (toTrain == BW::UnitID::Terran_NuclearMissile)
    {
      this->executors[0]->getRawDataLocal()->secondaryOrderID = BW::OrderID::Train;

      //a hack to disambiguate between nuke silo lag compensation and nuke about to complete
      //used in UnitImpl::getRemainingTrainTime
      this->executors[0]->getRawDataLocal()->driftPosX = 1; 
      return;
    }
    int slotToAffect = this->executors[0]->getBuildQueueSlot();
    if (this->executors[0]->getBuildQueue()[slotToAffect] != BW::UnitID::None)
      slotToAffect  = (slotToAffect + 1) % 5;

    if (this->executors[0]->getBuildQueue()[slotToAffect] != BW::UnitID::None)
    {
      this->failed = true;
      return;
    }

    executors[0]->getBuildQueue()[slotToAffect] = this->toTrain.getID();
    this->executors[0]->getRawDataLocal()->buildQueueSlot = (u8)slotToAffect;
    PlayerImpl* p = static_cast<PlayerImpl*>(this->executors[0]->getPlayer());
    p->spend(this->toTrain.mineralPrice(), this->toTrain.gasPrice());
    p->useSupplies(toTrain.supplyRequired(), toTrain._getRace());
    p->planToMake(toTrain);
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandTrain::getType()
  {
    return CommandTypes::Train;
  }
  //----------------------------------------------------------------------------------------------------------
};
