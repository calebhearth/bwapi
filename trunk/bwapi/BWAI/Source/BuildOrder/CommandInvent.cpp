#include "CommandInvent.h"

#include <Util/Exceptions.h>
#include <Util/Strings.h>
#include <Util/Logger.h>
#include <Util/Xml.h>
#include <BWAPI/TechType.h>
#include <BWAI/Globals.h>
#include <BWAI/TaskInvent.h>
#include <BWAI/Player.h>
#include <BWAPI/Globals.h>
#include "ConditionMinimalPopulation.h"
#include "Root.h"

namespace BuildOrder
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandInvent::CommandInvent(TiXmlElement* xmlElement)
  :Command(xmlElement)
  {
    this->name = Util::Xml::getRequiredAttribute(xmlElement, "name");
  }
  //----------------------------------------------------------------------------------------------------------
  bool CommandInvent::executeInternal(Executor* executor)
  {
    BWAPI::TechType toInvent = BWAPI::TechTypes::getTechType(this->name);
    if (toInvent==BWAPI::TechTypes::Unknown || toInvent==BWAPI::TechTypes::None)
    {
      BWAI::ai->root->log->log("Unknown invention name '%s'", this->name.c_str());
      return true;
    }
    if (BWAI::ai->player->canAfford(toInvent) &&
        this->conditionApplies()) 
    {
      BWAI::ai->root->log->log("Command to invent '%s' called", this->name.c_str());
      BWAI::ai->plannedInvents.push_back(new BWAI::TaskInvent(toInvent, priority));        
      return true;
    }
    return false;
  }
  //----------------------------------------------------------------------------------------------------------
}