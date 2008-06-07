#include "CommandBuild.h"

#include <Util/Exceptions.h>
#include <Util/Strings.h>
#include <Util/Logger.h>
#include <Util/Xml.h>

#include <BW/UnitType.h>

#include <BWAI/AI.h>
#include <BWAI/BuildingPosition.h>

#include <BWAI/TaskBuild.h>
#include <BWAI/Globals.h>
#include <BWAI/MapStartingPosition.h>

#include <BWAPI/Player.h>
#include <BWAPI/Globals.h>

#include "ConditionMinimalPopulation.h"
#include "Root.h"

namespace BuildOrder
{
  //---------------------------------- CONSTRUCTOR ----------------------------
  CommandBuild::CommandBuild(TiXmlElement* xmlElement)
  :Command(xmlElement)
  {
    this->name = Util::Xml::getRequiredAttribute(xmlElement, "name");
    this->place = Util::Xml::getRequiredAttribute(xmlElement, "place");
    
    const char * minimalPopulationAttribute = xmlElement->Attribute("minimal-population");
    if (minimalPopulationAttribute != NULL && this->condition == NULL)
    {
      this->condition = new ConditionMinimalPopulation(u16(Util::Strings::stringToInt(minimalPopulationAttribute))); 
      this->conditionRunType = ConditionRunType::WaitToApply;
    }
  }
  //---------------------------------------------------------------------------
  bool CommandBuild::execute()
  {
    BW::UnitType toBuild = BWAI::ai->unitNameToType[this->name];
    if (BWAI::ai->player->getMineralsLocal() - BWAI::ai->moneyToBeSpentOnBuildings >= toBuild.getMineralPrice() &&
        BWAI::ai->player->getGasLocal() >= toBuild.getGasPrice() &&
        this->conditionApplies()) 
    {
      BWAI::BuildingPosition* alternatives = BWAI::ai->getPositionsCalled(this->place); 
      if (alternatives == NULL)
      {
        BWAI::ai->root->log.log("Position '%s' not found", this->place.c_str());
        return true;
      }
      if (alternatives->positions.empty())
      {
        BWAI::ai->root->log.log("Position '%s' is empty", this->place.c_str());
        return true;
      }
     
      if (!toBuild.isAddon())
      {
        BWAI::Unit* scvToUse = NULL;
        BW::TilePosition position = BWAI::ai->getFreeBuildingSpot(this->place, scvToUse); 
        BWAI::ai->root->log.log("Command build '%s' called", this->name.c_str());
        BWAI::ai->plannedBuildings.push_back(new BWAI::TaskBuild(toBuild, position, scvToUse, alternatives));        
      }
      else if (toBuild.isAddon())
      {
        BWAI::Unit* executor;
        BWAI::ai->root->log.log("Command addon '%s' called at '%s'", this->name.c_str(), this->place.c_str());
        BW::TilePosition position = alternatives->positions.front();
        
        if (BWAPI::Broodwar.unitsOnTile[position.x - 1][position.y].empty())
        {
          BWAI::ai->root->log.log("Building for the addon not found");
          return false;
        }
        executor = BWAI::Unit::BWAPIUnitToBWAIUnit(BWAPI::Broodwar.unitsOnTile[position.x - 1][position.y].front());
        BWAI::ai->plannedBuildings.push_back(new BWAI::TaskBuild(toBuild, position, executor, alternatives));        
      }
      return true;
    }
    return false;
  }
  //---------------------------------------------------------------------------
}