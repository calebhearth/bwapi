#include "DevAIModule.h"

#include <BWAPI/Filters.h>
#include <algorithm>

using namespace BWAPI;

bool enabled;
int mapH, mapW;

DWORD dwCount = 0;
int bestFPS;

Player *self;
void onResearchComplete(Player *player, TechType tech);
void onUpgradeComplete(Player *player, UpgradeType upgrade, int level);


std::map<UpgradeType,int> lastUpgradeLevel;

std::function<bool(Player*)> HasNextUpgrade(UpgradeType upgrade)
{
  return [=](Player *p){ return p->getUpgradeLevel(upgrade) >= lastUpgradeLevel[upgrade]+1; };
}

std::function<bool(Player*)> HasTechnology(TechType tech)
{
  return [=](Player *p){ return p->hasResearched(tech); };
}

std::function<void(Player*)> OnResearchAct(TechType tech)
{
  return [=](Player *p){ onResearchComplete(p, tech); };
}

std::function<void(Player*)> OnUpgradeAct(UpgradeType upg)
{
  return [=](Player *p){ lastUpgradeLevel[upg] = p->getUpgradeLevel(upg);
                          onUpgradeComplete(p, upg, p->getUpgradeLevel(upg)); };
}

////////////////////////////
// User callbacks
void onResearchComplete(Player *player, TechType tech)
{
  Broodwar << player->getName() << " has researched " << tech << std::endl;
}

void onUpgradeComplete(Player *player, UpgradeType upgrade, int level)
{
  Broodwar << player->getName() << " has upgraded " << upgrade << " to level " << level << std::endl;
}
////////////////////////////////

void DevAIModule::onStart()
{
  // enable stuff
  bw->enableFlag(Flag::UserInput);

  // save player info
  self = bw->self();

  // Register tech/upgrade callbacks
  for( auto it = UpgradeTypes::allUpgradeTypes().begin(); it != UpgradeTypes::allUpgradeTypes().end(); ++it )
  {
    lastUpgradeLevel[*it] = self->getUpgradeLevel(*it);
    self->registerEvent(OnUpgradeAct(*it), HasNextUpgrade(*it));
  }
  for( auto it = TechTypes::allTechTypes().begin(); it != TechTypes::allTechTypes().end(); ++it )
  {
    self->registerEvent(OnResearchAct(*it), HasTechnology(*it), 1);
  }

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

  Unitset myUnits = Broodwar->self()->getUnits();
  for ( auto u = myUnits.begin(); u != myUnits.end(); ++u )
  {
    if ( u->getType().isWorker() && u->isIdle() )
    {
      u->gather( u->getClosestUnit(IsMineralField) );
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

void newOnUnitComplete(BWAPI::Unit *unit);

void DevAIModule::onUnitCreate(BWAPI::Unit* unit)
{
  unit->registerEvent(newOnUnitComplete, IsCompleted, 1);
  Broodwar << __FUNCTION__ " -- " << unit->getPlayer()->getName() << ": " << unit->getType() << std::endl;
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
  Broodwar << __FUNCTION__ << " -- " << unit->getType() << std::endl;
}

void newOnUnitComplete(BWAPI::Unit *unit)
{
  Broodwar << __FUNCTION__ " -- " << unit->getType() << std::endl;
}
