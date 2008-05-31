#include "TaskGatherGas.h"

#include <Logger.h>

#include "../../../BWAPI/Source/BWAPI/Globals.h"
#include "Globals.h"
#include "Expansion.h"

namespace BWAI
{
  //----------------- ------------- CONSTRUCTOR --------------------------------
  TaskGatherGas::TaskGatherGas(Unit* refinery, Expansion* expansion)
  :Task()
  ,refinery(refinery)
  ,expansion(expansion)
  {
    this->refinery->expansion = expansion;
    BWAI::ai->log->log("TaskGatherGas::TaskGatherGas called");
  }
  //------------------------------- DESTRUCTOR --------------------------------
  TaskGatherGas::~TaskGatherGas()
  {
    BWAI::ai->log->log("TaskGatherGas::~TaskGatherGas called");
  }
  //-------------------------------- EXECUTE ----------------------------------
  bool TaskGatherGas::execute()
  {
    std::list<Unit*>::iterator i = this->executors.begin();
    while (i != this->executors.end())
      if ((*i)->getOrderIDLocal() != BW::OrderID::PlayerGuard &&
          (*i)->getOrderIDLocal() != BW::OrderID::HarvestGas1 &&
          (*i)->getOrderIDLocal() != BW::OrderID::HarvestGas2 &&
          (*i)->getOrderIDLocal() != BW::OrderID::HarvestGas3 &&
          (*i)->getOrderIDLocal() != BW::OrderID::ReturnGas &&
          (*i)->getOrderIDLocal() != BW::OrderID::MoveToMinerals &&
          (*i)->getOrderIDLocal() != BW::OrderID::HarvestMinerals2 &&
          (*i)->getOrderIDLocal() != BW::OrderID::MiningMinerals &&
          (*i)->getOrderIDLocal() != BW::OrderID::ResetCollision2 &&
          (*i)->getOrderIDLocal() != BW::OrderID::ReturnMinerals)
      {
        BWAI::ai->log->log("Unit will be remmoved from the gas gather because order is (%s) Unit:", BW::OrderID::orderName((*i)->getOrderIDLocal()).c_str(), 
                                                                                                   (*i)->getName().c_str(),
                                                                                                    Util::LogLevel::Detailed);
        this->freeExecutor(*i++);
      }
      else
        i++;

    for (std::list<Unit*>::iterator i = this->executors.begin(); i != this->executors.end(); ++i)
      if ((*i)->getOrderIDLocal() != BW::OrderID::HarvestGas1 &&
          (*i)->getOrderIDLocal() != BW::OrderID::HarvestGas2 &&
          (*i)->getOrderIDLocal() != BW::OrderID::HarvestGas3 &&
          (*i)->getOrderIDLocal() != BW::OrderID::ReturnGas &&
          (*i)->getOrderIDLocal() != BW::OrderID::ResetCollision2)
        (*i)->orderRightClick(refinery);
    return false;
  }
  //-------------------------------- GET TYPE ---------------------------------
  TaskType::Enum TaskGatherGas::getType()
  {
    return TaskType::Gather;
  }
  //------------------------------- GET MINERAL -------------------------------
  Unit* TaskGatherGas::getRefinery()
  {
    return this->refinery;
  }
  //---------------------------------------------------------------------------
  Expansion* TaskGatherGas::getExpansion()
  {
    return expansion;
  }
  //---------------------------------------------------------------------------
}