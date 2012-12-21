#include "BWScriptEmulator.h"
#include <vector>

#include "ScriptThread.h"
#include "Controller.h"
#include "OrderEmulate.h"

using namespace BWAPI;

int mapH, mapW;
Player *self;

bool isResourceClaimed[256];

bool farcasting;

DWORD dwCount = 0;

void BWScriptEmulator::onStart()
{
  // enable stuff
  bw->enableFlag(Flag::UserInput);
  bw->enableFlag(Flag::CompleteMapInformation);
  farcasting = true;

  // Save some BW info locally for easier access
  self = bw->self();
  mapW = bw->mapWidth();
  mapH = bw->mapHeight();

  // Access full map just like the normal AI
  if ( !bw->isMultiplayer() )
    bw->sendText("black sheep wall");

  if ( !self )
    return;
  
  if ( !LoadAIBinary("bwapi-data\\AISCRIPT.BIN") )
    Broodwar->printf("%cFailed to load AISCRIPT binary!", 6);

  Race      selfRace(self->getRace());
  Position  sLoc(self->getStartLocation());

  MainController = new AIController();
  /*
  if      ( selfRace == Races::Zerg )
    AICreateThread("ZMCx", sLoc );
  else if ( selfRace == Races::Protoss )
    AICreateThread("PMCx", sLoc );
  else // ( selfRace == Races::Terran )
    AICreateThread("TMCx", sLoc );
    */
  srand(GetTickCount());
}

void BWScriptEmulator::onEnd(bool isWinner)
{
  if ( MainController )
    delete MainController;
}

void BWScriptEmulator::onFrame()
{
  if ( bw->isReplay() )
    return;

  UpdateScripts();

  Unitset myUnits( self->getUnits() );
  for ( auto u = myUnits.begin(); u != myUnits.end(); ++u )
  {
    if ( !u->exists() ) continue;
    EmulateOrder(*u);

    Broodwar->drawTextMap(u->getPosition(), "    %s", Order(GetUnitOrder(*u)).c_str() );
  }
}

void BWScriptEmulator::onSendText(std::string text)
{
  std::stringstream ss(text);
  std::string cmd;
  ss >> cmd;

  if ( cmd == "/fc" || cmd == "/farcast" )
  {
    farcasting = !farcasting;
    Broodwar << "Farcasting " << (farcasting ? "ENABLED" : "DISABLED") << std::endl;
  }
  else if ( cmd == "/r" || cmd == "/run" || cmd == "/script" )
  {
    std::string script;
    ss >> script;

    AICreateThread(script.c_str(), Broodwar->getScreenPosition() + Broodwar->getMousePosition() );
  }
  else
  {
    Broodwar->sendText("%s", text.c_str());
  }
}

void BWScriptEmulator::onReceiveText(BWAPI::Player* player, std::string text)
{}

void BWScriptEmulator::onPlayerLeft(BWAPI::Player* player)
{}

void BWScriptEmulator::onNukeDetect(BWAPI::Position target)
{}

void BWScriptEmulator::onUnitDiscover(BWAPI::Unit* unit)
{}

void BWScriptEmulator::onUnitEvade(BWAPI::Unit* unit)
{}

void BWScriptEmulator::onUnitShow(BWAPI::Unit* unit)
{}

void BWScriptEmulator::onUnitHide(BWAPI::Unit* unit)
{}

void BWScriptEmulator::onUnitCreate(BWAPI::Unit* unit)
{
  if ( unit->getPlayer() == self )  // If we own it
  {

  }
}

void BWScriptEmulator::onUnitDestroy(BWAPI::Unit* unit)
{
  if ( unit->getPlayer() == self )  // If we own it
  {

  }
}

void BWScriptEmulator::onUnitMorph(BWAPI::Unit* unit)
{}

void BWScriptEmulator::onUnitRenegade(BWAPI::Unit* unit)
{}

void BWScriptEmulator::onSaveGame(std::string gameName)
{}
