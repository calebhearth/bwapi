#pragma once
#include "..//..//BWAPI//Source//BWAPI//Game.h"
//#include "..//..//BWAPI//Source//BW//Unit.h"
#include <vector>

namespace BWAI { class Unit; }
namespace BWAI { class Mineral; }
namespace BWAI { class Expansion; }

namespace BWAPI { class Player; }
namespace BWAPI { class UnitPrototype; }

/** 
 * Set of classes providing ai tools.
 */
namespace BWAI
{
  class AI
  {
    public :
      AI();
      /** Call in the start of the game */
      void onStart(BWAPI::Player *player);
      void onEnd();
      void onFrame();
      void onCancelTrain();

      int suppliesOrdered;
      std::vector<Expansion*> expansions;
      std::vector<Mineral*> activeMinerals;
      Unit* getUnit(int index);
      static Unit* optimizeMineralFor;
      int expansionsSaturated;
   private :
      BWAPI::Player* player;
      Unit* units[BW::UNIT_ARRAY_MAX_LENGTH];
      BWAPI::UnitPrototype* worker;
      void startNewExpansion(Unit *gatherCenter);
      void rebalanceMiners();
      bool checkAssignedWorkers(void);
      void checkDeadWorkers(void);
      void logUnknownOrStrange();
  };
}
