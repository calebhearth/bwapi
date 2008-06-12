#include "TaskBuild.h"

#include <Util/Logger.h>
#include <Util/Types.h>
#include <BW/UnitType.h>
#include <BW/OrderID.h>
#include <BW/Unit.h>
#include <BWAPI/Player.h>
#include <BWAPI/Globals.h>
#include <BWAPI/Game.h>

#include "Unit.h"
#include "AI.h"
#include "Globals.h"
#include "BuildingPosition.h"

namespace BWAI
{
  //------------------------------ CONSTRUCTOR --------------------------------
  TaskBuild::TaskBuild(BW::UnitType buildingType, BW::TilePosition position, Unit* builder, BuildingPosition* alternatives)
  :Task(builder)
  ,buildingType(buildingType)
  ,position(position)
  ,building(NULL)
  ,alternatives(alternatives)
  {
  }
  //------------------------------- DESTRUCTOR --------------------------------
  TaskBuild::~TaskBuild()
  {
  }
  //-------------------------------- EXECUTE ----------------------------------
  bool TaskBuild::execute()
  {
    if (!this->executors.empty() &&
        this->building != NULL &&
        this->building->isCompleted())
    {
      BWAI::ai->log->log("(%s) finished production of (%s)", this->executors.front()->getType().getName(), this->buildingType.getName());
      return true;
    }
    if (this->executors.empty() &&
        !this->buildingType.isAddon())
    {
      Unit* builder = ai->freeBuilder(this->position);
      if (builder)
        this->addExecutor(builder);
    }
    
    if (this->building == NULL && 
        !this->executors.empty())
      if (!this->buildingType.isAddon())
      {
        if (
          this->executors.front()->getOrderTarget() != NULL &&
          this->executors.front()->getOrderID() == BW::OrderID::ConstructingBuilding &&
          this->executors.front()->getOrderTarget()->getType() == buildingType)
        {
          this->building = executors.front()->getOrderTarget();
          BWAI::ai->log->log("(%s) construction started", building->getName().c_str());
        }
      }
      else
        if (this->executors.front()->getBuildUnit() != NULL &&
            this->executors.front()->getBuildUnit()->getType() == buildingType)
        {
          this->building = BWAI::Unit::BWAPIUnitToBWAIUnit(executors.front()->getBuildUnit());
          BWAI::ai->log->log("(%s) construction of addon started", building->getName().c_str());
        }
    
    if (this->building != NULL &&
        !this->executors.empty() &&
        this->executors.front()->getOrderTargetLocal() != this->building &&
        !this->building->isCompleted() &&
        !this->buildingType.isAddon())
    {
       this->executors.front()->orderRightClick(this->building);
       BWAI::ai->log->log("(%s) Builder died - new builder sent to finish", building->getName().c_str());
    }
    
    if (!this->executors.empty() && this->building == NULL)
    {
      if ((!this->position.isValid() ||
          !this->canIBuild(position)) &&
          !this->buildingType.isAddon())
      {
        std::list<BW::TilePosition>::iterator i;
        for (i = alternatives->positions.begin();
             i != alternatives->positions.end();
             ++i)
          if (!this->position.isValid())
           this->position = *i;
          else if (this->canIBuild(*i))
          {
            this->position = *i;
            break;
          }
        if (i == this->alternatives->positions.end())
          return false;
      }
      BW::Position center(this->position);
      center.x += (BW::TILE_SIZE*this->getBuildingType().getTileWidth())/2;
      center.y += (BW::TILE_SIZE*this->getBuildingType().getTileHeight())/2;
      if (this->position.isValid())
        // Note that the auto conversion constructor is used here, so it takes care of conversion between tile position and position
        if (this->executors.front()->getDistance(center) > 100 &&
            !this->buildingType.isAddon())
        {
          if (
               (
                 this->executors.front()->getOrderIDLocal() != BW::OrderID::BuildTerran &&
                 this->executors.front()->getOrderIDLocal() != BW::OrderID::BuildProtoss1 &&
                 this->executors.front()->getOrderIDLocal() != BW::OrderID::DroneStartBuild
               ) &&
               (
                  this->executors.front()->getOrderIDLocal() != BW::OrderID::Move ||
                  this->executors.front()->getTargetPositionLocal().getDistance(center) > 300
               )
             )
          {
            this->executors.front()->orderRightClick(center);
            BWAI::ai->log->log("(%s) sent to build (%s) at (%d,%d)", this->executors.front()->getName().c_str(), buildingType.getName(), center.x, center.y);
          }
        }
        else
          if (!this->buildingType.isAddon())
          {
            if (this->executors.front()->getOrderIDLocal() != BW::OrderID::BuildTerran &&
                this->executors.front()->getOrderIDLocal() != BW::OrderID::BuildProtoss1 &&
                this->executors.front()->getOrderIDLocal() != BW::OrderID::DroneStartBuild &&
                this->executors.front()->getOwner()->canAfford(buildingType))
            {
              BWAI::ai->log->log("(%s) ordered to build (%s)", this->executors.front()->getName().c_str(), buildingType.getName());
              this->executors.front()->build(this->position, buildingType);
            }
          }
          else
            if (this->executors.front()->getSecondaryOrderIDLocal() != BW::OrderID::PlaceAddon &&
                this->executors.front()->hasEmptyBuildQueueLocal() &&
                this->executors.front()->getSecondaryOrderIDLocal() != BW::OrderID::BuildAddon)
            {
              BWAI::ai->log->log("(%s) ordered to build addon (%s)", this->executors.front()->getName().c_str(), buildingType.getName());
              BWAI::ai->log->log("secondary order id local = %d", this->executors.front()->getSecondaryOrderIDLocal());
              this->executors.front()->build(this->position, buildingType);
            }
    }
    return false;
  }
  //-------------------------------- GET TYPE ---------------------------------
  TaskType::Enum TaskBuild::getType()
  {
    return TaskType::Build;
  }
  //------------------------------- GET MINERAL -------------------------------
  BW::UnitType TaskBuild::getBuildingType()
  {
    return this->buildingType;
  }
  //---------------------------------------------------------------------------
  bool TaskBuild::canIBuild(BW::TilePosition here)
  {
    for (int k = here.x; 
         k < here.x + this->alternatives->tileWidth; 
              k++)
      for (int l = here.y; 
           l < here.y + this->alternatives->tileHeight; 
           l++)
        if (BWAPI::Broodwar.unitsOnTile[k][l].empty() == false &&
             (
               BWAPI::Broodwar.unitsOnTile[k][l].front()->getIndex() != this->executors.front()->getIndex() &&
               BWAPI::Broodwar.unitsOnTile[k][l].front()->getType() != BW::UnitID::Resource_VespeneGeyser ||
               BWAPI::Broodwar.unitsOnTile[k][l].size() != 1
             )
           )
          return false;
    return true;
  }
  //---------------------------------------------------------------------------
  Unit* TaskBuild::getBuilding()
  {
    return this->building;
  }
  //---------------------------------------------------------------------------
}