#include "DevAIModule.h"

using namespace BWAPI;

bool enabled;
int mapH, mapW;

DWORD dwCount = 0;
int bestFPS;

void DevAIModule::onStart()
{
  // enable stuff
  bw->enableFlag(Flag::UserInput);

  // save player info
  self = bw->self();

  // save map info
  mapH = bw->mapHeight();
  mapW = bw->mapWidth();
}

void DevAIModule::onEnd(bool isWinner)
{

}

DWORD dwLastTickCount;
bool testunload;
void DevAIModule::onFrame()
{
  if ( bw->isReplay() )
    return;

  int tFPS = bw->getFPS();
  if ( tFPS > bestFPS )
    bestFPS = tFPS;
  bw->drawTextScreen(4, 4, "Best: %d GFPS\nCurrent: %d GFPS", bestFPS, tFPS);

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
  //bw->printf("%s created", unit->getType().c_str());
}

void DevAIModule::onUnitDestroy(BWAPI::Unit* unit)
{
  //bw->printf("%s destroyed", unit->getType().c_str());
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
  //bw->printf("%s completed", unit->getType().c_str());
}
