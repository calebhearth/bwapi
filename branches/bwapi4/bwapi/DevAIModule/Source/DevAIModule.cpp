#include "DevAIModule.h"

#include <BWAPI/Filters.h>

using namespace BWAPI;

bool enabled;
int mapH, mapW;

DWORD dwCount = 0;
int bestFPS;

Player *self;

void DevAIModule::onStart()
{
  // enable stuff
  bw->enableFlag(Flag::UserInput);

  // save player info
  self = bw->self();

  // save map info
  mapH = bw->mapHeight();
  mapW = bw->mapWidth();

  // make things go fast
  //bw->setLocalSpeed(0);
  //bw->setFrameSkip(16);

  // set command optimization
  bw->setCommandOptimizationLevel(3);
}

void DevAIModule::onEnd(bool isWinner)
{
}

DWORD dwLastTickCount;
bool testunload;
void DevAIModule::onFrame()
{
  if ( bw->isReplay() ) // ignore everything if in a replay
    return;


  // Get the best logical FPS
  int tFPS = bw->getFPS();
  if ( tFPS > bestFPS )
    bestFPS = tFPS;

  // Display it
  bw->drawTextScreen(4, 4, "Best: %d GFPS\nCurrent: %d GFPS", bestFPS, tFPS);
  

  // Limit logical frames processed to prevent stacking commands
  if ( bw->getFrameCount() % bw->getLatencyFrames() != 0 )
    return;

  Unitset myUnits = self->getUnits();
  for ( auto u = myUnits.begin(); u != myUnits.end(); ++u )
  {
    if ( u->isIdle() && u->isCompleted() )  // idle units
    {
      if ( u->getType().isWorker() ) // worker units
      {
        // test radius units
        //Unitset uset = u->getUnitsInRadius(1024, IsMineralField);
        //for ( auto i = uset.begin(); i != uset.end(); ++i )
          //Broodwar->drawLineMap(u->getPosition(), i->getPosition(), Colors::Brown);

        Unit *pMineral = u->getClosestUnit(IsMineralField, 1024);
        if ( pMineral != nullptr )
          Broodwar->drawLineMap(u->getPosition(), pMineral->getPosition(), Colors::Green);
        //u->gather( u->getClosestUnit(IsMineralField, 1024) );

/*        
        int bestDistance = 99999999;
        Unit *pBestUnit = nullptr;

        Unitset resources = Broodwar->getMinerals();
        for ( auto m = resources.begin(); m != resources.end(); ++m )
        {
          int newDistance = m->getDistance(u->getPosition());
          if ( newDistance < bestDistance )
          {
            pBestUnit = *m;
            bestDistance = newDistance;
          }
        }
        if ( pBestUnit != nullptr )
          Broodwar->drawLineMap(u->getPosition(), pBestUnit->getPosition(), Colors::Orange);
          */

      }

      if ( u->getType().isResourceDepot() ) // center
      {
        u->train( u->getType().getRace().getWorker() );
      }
      
    }

  }
}

void DevAIModule::onSendText(std::string text)
{
  if ( text == "/wiki" )
  {
    writeUnitWiki();
    writeWeaponWiki();
    Broodwar->printf("Generated wiki pages!");
  }
  else if ( text == "/best" )
  {
    bw->printf("Best: %d FPS", bestFPS);
  }
  else if ( text == "/races" )
  {
    for ( auto p = bw->getPlayers().begin(),
          pend = bw->getPlayers().end();
          p != pend;
          ++p )
    {
      bw->printf("%s is %s", (*p)->getName().c_str(), (*p)->getRace().c_str());
    }
  }
  else
  {
    Broodwar->sendText("%s", text.c_str());
  }
}

void DevAIModule::onReceiveText(BWAPI::Player* player, std::string text)
{
}

void DevAIModule::onPlayerLeft(BWAPI::Player* player)
{
}

void DevAIModule::onNukeDetect(BWAPI::Position target)
{
}

void DevAIModule::onUnitDiscover(BWAPI::Unit* unit)
{
  //bw->printf("%s discovered", unit->getType().c_str());
}

void DevAIModule::onUnitEvade(BWAPI::Unit* unit)
{
  //bw->printf("%s evaded", unit->getType().c_str());
}

void DevAIModule::onUnitShow(BWAPI::Unit* unit)
{
  //bw->printf("%s shown", unit->getType().c_str());
}

void DevAIModule::onUnitHide(BWAPI::Unit* unit)
{
  //bw->printf("%s hidden", unit->getType().c_str());
}

void DevAIModule::onUnitCreate(BWAPI::Unit* unit)
{
  bw->printf("%s created", unit->getType().c_str());
}

void DevAIModule::onUnitDestroy(BWAPI::Unit* unit)
{
  bw->printf("%s destroyed", unit->getType().c_str());
}

void DevAIModule::onUnitMorph(BWAPI::Unit* unit)
{
  //bw->printf("%s morphed", unit->getType().c_str());
}

void DevAIModule::onUnitRenegade(BWAPI::Unit* unit)
{
  //bw->printf("%s renegaded", unit->getType().c_str());
}

void DevAIModule::onSaveGame(std::string gameName)
{
}

void DevAIModule::onUnitComplete(BWAPI::Unit *unit)
{
  bw->printf("%s completed", unit->getType().c_str());
}
