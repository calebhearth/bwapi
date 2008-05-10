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
    for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
    {
      if (ai.getUnit(i)->isValid() &&
          ai.getUnit(i)->isMineral() &&
          ai.getUnit(i)->getDistance(this->gatherCenter) < 300)
       this->minerals.push_back(new Mineral(ai.getUnit(i), this));
    }
  }
  //---------------------------------------------------------------------------
}