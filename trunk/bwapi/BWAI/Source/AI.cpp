#include "AI.h"
#include <algorithm>

#include "Mineral.h"
#include "Unit.h"
#include "Expansion.h"
#include "MapInfo.h"
#include "MapExpansion.h"
#include "MapStartingPosition.h"
#include "BuildingToMake.h"

#include "../../BWAPI/Source/BWAPI/Unit.h"
#include "../../BWAPI/Source/BWAPI/Player.h"
#include "../../BWAPI/Source/BWAPI/Globals.h"
//#include "../../BWAPI/Source/BW/Bitmask.h" /**< @todo remove */
#include "../../BWAPI/Source/BW/MovementFlags.h" /**< @todo remove */
#include "../../BWAPI/Source/BW/OrderFlags.h" /**< @todo remove */
#include "../../BWAPI/Source/BWAPI/Map.h"

#include <Exceptions.h>
#include <Logger.h>
#include <Dictionary.h>



namespace BWAI
{
  BWAPI::Unit *cc;
  //----------------------------- MINERAL VALUE -------------------------------
  bool mineralValue(BWAI::Mineral*& mineral1, BWAI::Mineral*& mineral2)
  {
    if (mineral1->gatherersAssigned.size() < mineral2->gatherersAssigned.size())
      return true;
    if (mineral1->gatherersAssigned.size() > mineral2->gatherersAssigned.size())
      return false;
    if (AI::optimizeMineralFor != NULL &&
        mineral1->expansion->gatherCenter != mineral2->expansion->gatherCenter)
    {
      u16 distance1 = AI::optimizeMineralFor->getDistance(mineral1->expansion->gatherCenter);
      u16 distance2 = AI::optimizeMineralFor->getDistance(mineral2->expansion->gatherCenter);
      if (distance1 < distance2)
        return true;
      if (distance1 > distance2)
       return false;
    }
    u16 distance1 = mineral1->mineral->getDistance(mineral1->expansion->gatherCenter);
    u16 distance2 = mineral2->mineral->getDistance(mineral1->expansion->gatherCenter);
    if (distance1 < distance2)
      return true;
    if (distance1 > distance2)
      return false;
    return false;
  }
  //------------------------------- UPDATE ------------------------------------
  void AI::update(void)
  {
    this->first = Unit::BWUnitToBWAIUnit(*BW::BWXFN_UnitNodeTable_FirstElement);
    if (this->first != NULL)
      this->first->updateNext();
  }
  //------------------------------ ON START -----------------------------------
  void AI::onStart(BWAPI::Player *player)
  {
    try
    {
      this->log->log("Ai::onStart start", LogLevel::Important);
      this->player = player;
      BWAPI::Map::saveBuildabilityMap(BWAPI::Broodwar.configuration->getValue("data_path") + "\\buildability.txt");
      std::string mapNameAbsolute = BWAPI::Map::getFileName();
      size_t lastDelimiterPos = mapNameAbsolute.rfind('\\');
      std::string mapName = mapNameAbsolute.substr(lastDelimiterPos + 1, mapNameAbsolute.size() - lastDelimiterPos - 1);
      mapInfo = new MapInfo(BWAPI::Broodwar.configuration->getValue("maps_path") + "\\" + mapName + ".xml");
      this->checkNewExpansions();
      
      this->log->log("Help pre-prepared information found for the curent map");
      
      if (this->expansions.size())
      {
        for (std::list<MapStartingPosition*>::iterator i = mapInfo->startingPositions.begin(); i != mapInfo->startingPositions.end(); ++i)
         if (this->expansions.front()->gatherCenter->getDistance((*i)->expansion->getPosition()) < 100)
          {
            this->startingPosition = *i;
            break;
          }
        if (this->startingPosition)
        {
         this->log->log("Starting position is (%s) at (%d, %d)", this->startingPosition->expansion->getID().c_str(), 
                                                                 this->startingPosition->expansion->getPosition().x, 
                                                                 this->startingPosition->expansion->getPosition().y);
         for (std::list<BW::Position>::iterator j = this->startingPosition->nonProducing3X2BuildingPositions.begin(); 
              j != this->startingPosition->nonProducing3X2BuildingPositions.end(); 
              ++j)
           this->log->log("3X2 building at at (%d, %d)", (*j).x, (*j).y);
        }
      }
     this->log->log("Ai::onStart end", LogLevel::Important);
    }
    catch (GeneralException& exception)
    {
      this->log->log("Exception in AI::onStart: " + exception.getMessage());
      delete this->mapInfo;
      this->mapInfo = NULL;
    }
  }
  //--------------------------------- ON END ---------------------------------
  void AI::onEnd()
  {
    this->log->log("Ai::onEnd start", LogLevel::Important);
    for (std::list<Expansion*>::iterator i = this->expansions.begin(); i != this->expansions.end(); ++i)
      delete *i;
    this->activeMinerals.clear();
    this->expansions.clear();
    this->expansionsSaturated = false;
    for (Unit* i = this->getFirst(); i != NULL; i = i->getNext())
      i->expansionAssingment = NULL;
    this->log->log("Ai::onEnd end", LogLevel::Detailed);
    delete mapInfo;
    for (std::list<BuildingToMake*>::iterator i = this->plannedBuildings.begin(); i != this->plannedBuildings.end(); ++i)
      delete *i;
    this->plannedBuildings.clear();
  }
  //------------------------------- CONSTRUCTOR -------------------------------
  AI::AI(void)
  :mapInfo(NULL)
  ,startingPosition(NULL)
  ,log    (new Util::Logger(BWAPI::Broodwar.configuration->getValue("log_path") + "\\ai",   LogLevel::MicroDetailed))
  ,deadLog(new Util::Logger(BWAPI::Broodwar.configuration->getValue("log_path") + "\\dead", LogLevel::MicroDetailed))
  {
    this->suppliesOrdered = 0;
    for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
      this->units[i] = new Unit(BWAPI::Broodwar.getUnit(i));
    this->first = NULL;
  }
  //-------------------------------- DESTRUCTOR -------------------------------
  AI::~AI(void)
  {
    delete this->log;
    delete deadLog;

    for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
      delete new Unit(BWAPI::Broodwar.getUnit(i));

    for (std::list<BuildingToMake*>::iterator i = this->plannedBuildings.begin(); i != this->plannedBuildings.end(); ++i)
      delete *i;
  }
  //-------------------------------  ON FRAME ---------------------------------
  void AI::onFrame(void)
  {
    if (BWAPI::Broodwar.frameCount < 2)
      return;
     
    this->checkSupplyNeed();
    this->checkPlannedBuildings();
        
    bool reselected = false;
    BW::Unit** selected = BWAPI::Broodwar.saveSelected();
    this->refreshSelectionStates(selected);

    this->checkNewExpansions();

    for (Unit* i = this->getFirst(); i != NULL; i = i->getNext())
      if (!i->hasEmptyBuildQueueLocal())
        i->lastTrainedUnit = i->getBuildQueueLocal()[i->getBuildQueueSlotLocal()];

    std::list<Unit*> idleWorkers;
    this->getIdleWorkers(idleWorkers);
    this->assignIdleWorkersToMinerals(idleWorkers);

    this->checkWorkersNeed();
    reselected |= this->performAutoBuild();
    this->rebalanceMiners();
    reselected |= this->checkAssignedWorkers();
    if (reselected)
      BWAPI::Broodwar.loadSelected(selected);
    else
      delete [] selected;
  }
  //-------------------------------- GET UNIT ---------------------------------
  Unit* AI::getUnit(int index)
  {
    return units[index];
  }
  //------------------------------- ON CANCEL TRAIN ---------------------------
  void AI::onCancelTrain()
  {
    this->log->log("Cancelled unit caught");
    BW::Unit** selected = BWAPI::Broodwar.saveSelected();
    
    if (selected[0] != NULL)
    {
      Unit::BWUnitToBWAIUnit(selected[0])->lastTrainedUnit = BW::UnitID::None;
      this->log->log("Cancelled production caught - %s", Unit::BWUnitToBWAIUnit(selected[0])->getType().getName(), LogLevel::Detailed);
    }
  }
  //---------------------------- START NEW EXPANSION -------------------------
   void AI::startNewExpansion(Unit *gatherCenter)
   {
     this->expansions.push_back(new Expansion(gatherCenter));
     this->rebalanceMiners();
   }
  u16 pos = 0;
  //---------------------------- REBALANCE MINERS -----------------------------
   void AI::rebalanceMiners(void)
   {
     if (activeMinerals.size() < 2)
       return;
     Unit* gatherer;
     anotherStep:
     Mineral* best = *activeMinerals.begin();
     Mineral* worst = *activeMinerals.begin();
     for (std::list<Mineral*>::iterator i = this->activeMinerals.begin(); i != this->activeMinerals.end(); ++i)
       if (best->gatherersAssigned.size() > (*i)->gatherersAssigned.size())
         best = (*i);
       else 
         if (worst->gatherersAssigned.size() < (*i)->gatherersAssigned.size())
           worst = (*i);

     if (best->gatherersAssigned.size() + 1 < worst->gatherersAssigned.size())
     {
       gatherer = worst->gatherersAssigned[0];
       worst->removeGatherer(gatherer);
       AI::optimizeMineralFor = gatherer;
       activeMinerals.sort(mineralValue);
       (*activeMinerals.begin())->assignGatherer(gatherer);
       goto anotherStep;
     }
   }
  //---------------------------- CHECK ASSIGNED WORKERS -----------------------
  bool AI::checkAssignedWorkers(void)
  {
    bool reselected = false;
    for (std::list<Expansion*>::iterator i = this->expansions.begin(); i != this->expansions.end(); ++i)
      reselected |= (*i)->checkAssignedWorkers();
    return reselected;
  }
  //---------------------------------------------------------------------------
  Unit* AI::optimizeMineralFor = NULL;
  
  //------------------------------- GET FIRST ---------------------------------
  Unit* AI::getFirst()
  {
    return this->first;
  }
  //----------------------------- ON REMOVE UNIT ------------------------------
  void AI::onRemoveUnit(BW::Unit* unit)
  {
    Unit* dead = BWAI::Unit::BWUnitToBWAIUnit(unit);
    this->deadLog->log("%s just died", dead->getName().c_str());

    if (dead->isMineral())
      if (dead->expansionAssingment != NULL)
        dead->expansionAssingment->removeMineral(dead);
    else if (dead->getType().isWorker())
      if (dead->expansionAssingment != NULL)
        dead->expansionAssingment->removeWorker(dead);
    dead->lastTrainedUnit = BW::UnitID::None;
    dead->expansionAssingment = NULL;
   }
  //------------------------------ CHECK NEW EXPANSION ------------------------
  void AI::checkNewExpansions()
  {
    for (BWAI::Unit* i = this->getFirst(); i != NULL; i = i->getNext())
    {
      if (
           i->isReady() &&
           (
             i->getType() == BW::UnitID::Terran_CommandCenter ||
             i->getType() == BW::UnitID::Protoss_Nexus ||
             i->getType() == BW::UnitID::Zerg_Hatchery
           ) &&
           i->expansionAssingment == NULL &&
           i->getOwner() == player)
      {
        this->log->log("Starting new expansion - %s", i->getName().c_str(), LogLevel::Important);
        this->expansionsSaturated = false;
        this->startNewExpansion(i);
      }
    }
  }
  //----------------------------- REFRESH SELECTION STATES --------------------
  void AI::refreshSelectionStates(BW::Unit** selected)
  {
    for (BWAI::Unit* i = this->getFirst(); i != NULL; i = i->getNext())
      i->selected = false;
    for (int i = 0; selected[i] != NULL; i++)
      BWAI::Unit::BWUnitToBWAIUnit(selected[i])->selected = true;
  }
  //---------------------------- PERFRORM AUTOBUILD ---------------------------
  bool AI::performAutoBuild()
  {
    bool reselected = false;
    for (Unit* i = this->getFirst(); i != NULL; i = i->getNext())
      if (i->isReady() &&
          i->getType() != BW::UnitID::None &&
          i->hasEmptyBuildQueueLocal() &&
          i->lastTrainedUnit != BW::UnitID::None &&
          i->getType().canProduce() &&
          i->getOwner() == player &&
          i->lastTrainedUnit.isValid())
      {
       BW::UnitType typeToBuild = BW::UnitType(i->lastTrainedUnit);
       if (
           typeToBuild.isValid() &&
            (
              (
                 typeToBuild.isTerran() &&
                 player->freeSuppliesTerranLocal() >= typeToBuild.getSupplies()
               ) ||
               (
                 typeToBuild.isProtoss() &&
                 player->freeSuppliesProtossLocal() >= typeToBuild.getSupplies()
               ) ||
               (
                 typeToBuild.isZerg() &&
                 player->freeSuppliesZergLocal() >= typeToBuild.getSupplies()
               )
             ) &&
             player->getMineralsLocal() >= typeToBuild.getMineralPrice() &&
             player->getGasLocal() >= typeToBuild.getGasPrice()
           )
        {
          reselected = true;
          i->trainUnit(typeToBuild);
        }
      }
    return reselected;
  }
  //------------------------------ GET IDLE WORKERS ---------------------------
  void AI::getIdleWorkers(std::list<Unit*>& workers)
  {
    if (!this->expansionsSaturated)
      for (Unit* i = this->getFirst(); i != NULL; i = i->getNext())
      {
        if (i->isReady() &&
            i->getOwner() == player &&
              (
               i->getOrderIDLocal() == BW::OrderID::PlayerGuard ||
               i->getOrderIDLocal() == BW::OrderID::MoveToMinerals ||
               i->getOrderIDLocal() == BW::OrderID::HarvestMinerals2 ||
               i->getOrderIDLocal() == BW::OrderID::MiningMinerals ||
               i->getOrderIDLocal() == BW::OrderID::ResetCollision2 ||
               i->getOrderIDLocal() == BW::OrderID::ReturnMinerals
             ) &&
             !i->selected &&
             (i->getType().isWorker()) &&
              i->expansionAssingment == NULL)
          workers.push_back(i); 
      }
  }
  //--------------------------------- CHECK WORKERS NEED ----------------------
  void AI::checkWorkersNeed(void)
  {
    unsigned int workersTogether = 0;
    for (std::list<Expansion*>::iterator i = this->expansions.begin(); i != this->expansions.end(); ++i)
      workersTogether += (*i)->asignedWorkers;
    if (workersTogether >= this->activeMinerals.size()*2.5 ||
        workersTogether >= 100)
    {
      this->expansionsSaturated = true;
      for (std::list<Expansion*>::iterator i = this->expansions.begin(); i != this->expansions.end(); ++i)
        (*i)->gatherCenter->lastTrainedUnit = BW::UnitID::None;
    }
  }
  //----------------------------- ASSIGN IDLE WORKERS TO MINERALS -------------
  void AI::assignIdleWorkersToMinerals(std::list<Unit*>& idleWorkers)
  {
    if (!this->expansionsSaturated)
      for (std::list<Unit*>::iterator i = idleWorkers.begin(); i != idleWorkers.end(); ++i)
      {
        AI::optimizeMineralFor = *i;
        /** @todo Just find smallest, no need to sort */
        activeMinerals.sort(mineralValue);
        if ((*activeMinerals.begin())->gatherersAssigned.size() >= 2)
        {
          this->expansionsSaturated = true;
          break;
        }
        (*activeMinerals.begin())->assignGatherer(*i);
      }
  }
  //---------------------------------------------------------------------------
  int AI::countOfProductionBuildings()
  {
    int countOfFactories = 0;
    for (Unit* i = this->getFirst(); i != NULL; i = i->getNext())
      if (i->isReady() && 
          i->getType().canProduce() &&
          i->getOwner() == this->player)
         countOfFactories++;
    return countOfFactories;
  }
  //---------------------------------------------------------------------------
  void AI::checkSupplyNeed()
  {
    int countOfFactories = this->countOfProductionBuildings();
    if (countOfFactories * 1.5 > player->freeSuppliesTerranLocal() + plannedTerranSupplyGain())
    {
      this->log->log("Not enough supplies factories = %d freeSupplies = %d plannedToBuildSupplies = %d", countOfFactories , player->freeSuppliesTerranLocal(), plannedTerranSupplyGain());
      for (std::list<BW::Position>::iterator i = this->startingPosition->nonProducing3X2BuildingPositions.begin();
          i != this->startingPosition->nonProducing3X2BuildingPositions.end();
          ++i)
      {
        bool occupied = false;
        for (int j = (*i).x; 
              j < (*i).x + BW::UnitType(BW::UnitID::Terran_SupplyDepot).getTileWidth(); 
              j++)
          for (int k = (*i).y; 
               k < (*i).y + BW::UnitType(BW::UnitID::Terran_SupplyDepot).getTileHeight(); 
               k++)
           occupied |= BWAPI::Broodwar.unitsOnTile[j][k].size() > 0;
        if (!occupied)
        {
          this->log->log("Found free spot for supply depot at (%d,%d)", (*i).x, (*i).y);
          this->plannedBuildings.push_back(new BuildingToMake(NULL, BW::UnitID::Terran_SupplyDepot, (*i)));
          break;
        }
      } 
    }        
  }
  //---------------------------------------------------------------------------
  s32 AI::plannedTerranSupplyGain()
  {
    s32 returnValue = 0;
    for (std::list<BuildingToMake*>::iterator i = this->plannedBuildings.begin(); i != this->plannedBuildings.end(); i++)
      if ((*i)->getType() == BW::UnitID::Terran_SupplyDepot)
        returnValue += 8;
      else if ((*i)->getType() == BW::UnitID::Terran_CommandCenter)
        returnValue += 10;
    return returnValue;
  }
  //---------------------------------------------------------------------------
  void AI::checkPlannedBuildings()
  {
    std::list<BuildingToMake*>::iterator i = this->plannedBuildings.begin();
    while (i != this->plannedBuildings.end())
      if ((*i)->execute())
      {
        delete *i;
        i = this->plannedBuildings.erase(i);
      }
      else
        ++i;
  }
  //---------------------------------------------------------------------------
  Unit* AI::freeBuilder()
  {
    for (Unit* i = this->getFirst(); i != NULL; i = i->getNext())
      if (i->getOwner() == player &&
          i->getType().isWorker())
      {
       this->log->log("%s is going to be freed to do something else", i->getName().c_str());
       if (i->expansionAssingment != NULL)
           i->expansionAssingment->removeWorker(i);
       return i;
      }
    return NULL;
  }
  //---------------------------------------------------------------------------
}
