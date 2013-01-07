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

  self->getUnits().move(bw->getMousePosition()+bw->getScreenPosition());
  self->getUnits().burrow();
  self->getUnits().siege();
  self->getUnits().attack(bw->getMousePosition()+bw->getScreenPosition());
  self->getUnits().holdPosition();
  self->getUnits().rightClick(bw->getMousePosition()+bw->getScreenPosition());
  self->getUnits().cloak();
  self->getUnits().stop();

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
