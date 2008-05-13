#include "Expansion.h"
#include "AI.h"
#include "Unit.h"
#include "Globals.h"
#include "..//..//BWAPI//Source//BWAPI//UnitPrototype.h" /**< @todo remove */
namespace BWAI
{
  //------------------------------------ CONSTRUCTOR --------------------------
  Expansion::Expansion(BWAI::Unit* gatherCenter)
   :gatherCenter(gatherCenter)
   ,asignedWorkers(0)
  {
    FILE *f = fopen("bwai.log","at");
    fprintf(f, "New expansion construction start\n");
    fclose(f);
    for (Unit* i = ai->getFirst(); i != NULL; i = i->getNext())
    {
      if (i->isMineral() &&
          i->expansionAssingment == NULL &&
          i->getDistance(this->gatherCenter) < 350)
      {
        FILE *f = fopen("bwai.log","at");
        fprintf(f, "mineral to add to expansion\n");
        fclose(f);
        this->minerals.push_back(new Mineral(i, this));
      }
    }
    if (this->gatherCenter->lastTrainedUnitID == BW::UnitType::None)
      switch (this->gatherCenter->getType())
      {
        case BW::UnitType::Terran_CommandCenter : gatherCenter->lastTrainedUnitID = BW::UnitType::Terran_SCV; break;
        case BW::UnitType::Protoss_Nexus        : gatherCenter->lastTrainedUnitID = BW::UnitType::Protoss_Probe; break;
        case BW::UnitType::Zerg_Hatchery        : gatherCenter->lastTrainedUnitID = BW::UnitType::Zerg_Drone; break;
      }
    gatherCenter->expansionAssingment = this;

    f = fopen("bwai.log","at");
    fprintf(f, "New expansion construction end\n");
    fclose(f);
  }
  //------------------------------ REMOVE WORKER ------------------------------
  Expansion::~Expansion(void)
  {
    for (unsigned int i = 0; i < this->minerals.size(); i++)
      delete this->minerals[i];
    this->minerals.clear();
  }
  //---------------------------------------------------------------------------
  void Expansion::removeWorker(Unit* worker)
  {
    for (unsigned int i = 0; i < this->minerals.size(); i++)
      if (this->minerals[i]->removeGatherer(worker))
        return;
  }
  //------------------------------ CHECK ASSIGNED WORKERS ---------------------
  bool Expansion::checkAssignedWorkers()
  {   
    bool reselected = false;
    for (unsigned int i = 0; i < this->minerals.size(); i++)
      reselected |= this->minerals[i]->checkAssignedWorkers();
    return reselected;
  }
  //---------------------------------------------------------------------------
  void Expansion::removeMineral(BWAI::Unit* mineral)
  {
    for (unsigned int i = 0; i < this->minerals.size(); i++)
      if (this->minerals[i]->mineral == mineral)
      {
         delete this->minerals[i]; // also frees workers
         this->minerals.erase(this->minerals.begin() + i);
         return;
      }
  }
  //---------------------------------------------------------------------------
}