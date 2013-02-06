#include "DevAIModule.h"

#include <BWAPI/Filters.h>
#include <algorithm>

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

  UnitType::set allTypes(UnitTypes::allUnitTypes());
  for ( auto it = allTypes.begin(); it != allTypes.end(); ++it )
    Broodwar << *it << std::endl;

  Unitset myUnits = self->getUnits();
  bool hasConstructor = std::find_if(myUnits.begin(), myUnits.end(), IsConstructing) != myUnits.end();
  for ( auto u = myUnits.begin(); u != myUnits.end(); ++u )
  {
    if ( !u->exists() || !u->isCompleted() )
      continue;

    UnitType t = u->getType();
    if ( t.isWorker() )
    {
      if ( u->isIdle() )
        u->gather( u->getClosestUnit(IsMineralField) );
    }
    else if ( t.isResourceDepot() )
    {
      if ( !u->train( t.getRace().getWorker() ) )
      {
        if ( Broodwar->getLastError() == Errors::Insufficient_Supply )
        {
          UnitType providerType = t.getRace().getSupplyProvider();
          Unit *pSupplyBuilder = u->getClosestUnit(IsOwned && IsCompleted && GetType == providerType.whatBuilds().first );
          if ( pSupplyBuilder )
          {
            if ( providerType.isBuilding() )
            {
              if ( !hasConstructor )
              {
                BWAPI::TilePosition tp = Broodwar->getBuildLocation(UnitTypes::Terran_Barracks, TilePosition(u->getPosition()));
                if ( !pSupplyBuilder->build(UnitTypes::Terran_Barracks, tp ) )
                  Broodwar << Broodwar->getLastError() << std::endl;
              }
            }
            else
            {
              pSupplyBuilder->train(providerType);
            }
          }
        }
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
}

void DevAIModule::onUnitEvade(BWAPI::Unit* unit)
{
}

void DevAIModule::onUnitShow(BWAPI::Unit* unit)
{
}

void DevAIModule::onUnitHide(BWAPI::Unit* unit)
{
}

void DevAIModule::onUnitCreate(BWAPI::Unit* unit)
{
}

void DevAIModule::onUnitDestroy(BWAPI::Unit* unit)
{
}

void DevAIModule::onUnitMorph(BWAPI::Unit* unit)
{
}

void DevAIModule::onUnitRenegade(BWAPI::Unit* unit)
{
}

void DevAIModule::onSaveGame(std::string gameName)
{
}

void DevAIModule::onUnitComplete(BWAPI::Unit *unit)
{
}
