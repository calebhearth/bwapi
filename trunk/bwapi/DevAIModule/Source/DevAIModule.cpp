#include "DevAIModule.h"
#include <set>

using namespace BWAPI;

bool enabled;
int mapH, mapW;

DWORD dwCount = 0;

void DevAIModule::onStart()
{
  // enable stuff
  bw->enableFlag(Flag::UserInput);
  enabled = true;

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
void DevAIModule::onFrame()
{
  if ( bw->isReplay() )
    return;

  if ( !enabled )
    return;

  for each ( Bullet *b in bw->getBullets() )
  {
    if ( b && b->exists() )
    {
      bw->drawTextMap(b->getPosition().x(), b->getPosition().y(), "%cO", b->getPlayer() ? b->getPlayer()->getTextColor() : 2);
      bw->drawTextMap(b->getPosition().x(), b->getPosition().y(), "%cX", b->getPlayer() ? b->getPlayer()->getTextColor() : 2);
    }
  }
}

void DevAIModule::onSendText(std::string text)
{
  if ( text == "/t" )
  {
    enabled = !enabled;
    Broodwar->printf("DevAITest %s", enabled ? "ENABLED" : "DISABLED");
  }
  else if ( text == "/wiki" )
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
