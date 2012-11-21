#include "ExampleAIModule.h"
#include <iostream>

using namespace BWAPI;

void ExampleAIModule::onStart()
{
  // Hello World!
  Starcraft->sendText("Hello world!");

  // Print the map name.
  // BWAPI returns std::string when retrieving a string, don't forget to add .c_str() when printing!
  Starcraft << "The map is " << Broodwar->mapName() << "!" << std::endl;

  // Enable the UserInput flag, which allows us to control the bot and type messages.
  Starcraft->enableFlag(Flag::UserInput);

  // Uncomment the following line and the bot will know about everything through the fog of war (cheat).
  //Starcraft->enableFlag(Flag::CompleteMapInformation);

  // Set the command optimization level so that common commands can be grouped
  // and reduce the bot's APM (Actions Per Minute).
  Starcraft->setCommandOptimizationLevel(2);

  // Check if this is a replay
  if ( Starcraft->isReplay() )
  {

    // Announce the players in the replay
    Starcraft << "The following players are in this replay:" << std::endl;
    
    // Iterate all the players in the game using a std:: iterator
    Playerset players = Starcraft->getPlayers();
    for(auto p = players.begin(); p != players.end(); ++p)
    {
      // Only print the player if they are not an observer
      if ( !p->isObserver() )
        Starcraft << p->getName() << ", playing as " << p->getRace() << std::endl;
    }

  }
  else // if this is not a replay
  {
    // Retrieve you and your enemy's races. enemy() will just return the first enemy.
    // If you wish to deal with multiple enemies then you must use enemies().
    if ( Starcraft->enemy() ) // First make sure there is an enemy
      Starcraft << "The matchup is " << Starcraft->self()->getRace() << " vs " << Starcraft->enemy()->getRace() << std::endl;
  }

}

void ExampleAIModule::onEnd(bool isWinner)
{
  // Called when the game ends
  if ( isWinner )
  {
    // Log your win here!
  }
}

void ExampleAIModule::onFrame()
{
  // Called once every game frame

  // Display the game frame rate as text in the upper left area of the screen
  Starcraft->drawTextScreen(200, 0,  "FPS: %d", Starcraft->getFPS() );
  Starcraft->drawTextScreen(200, 20, "Average FPS: %f", Starcraft->getAverageFPS() );

  // Return if the game is a replay or is paused
  if ( Starcraft->isReplay() || Starcraft->isPaused() )
    return;

  // Prevent spamming by only running our onFrame once every number of latency frames.
  // Latency frames are the number of frames before commands are processed.
  if ( Starcraft->getFrameCount() % Starcraft->getLatencyFrames() != 0 )
    return;

  // Iterate through all the units that we own
  Unitset myUnits = Starcraft->self()->getUnits();
  for ( Unitset::iterator u = myUnits.begin(); u != myUnits.end(); ++u )
  {
    // Ignore the unit if it no longer exists
    // Make sure to include this block when handling any Unit pointer!
    if ( !u->exists() )
      continue;

    // Ignore the unit if it has one of the following status ailments
    if ( u->isLockedDown() || u->isMaelstrommed() || u->isStasised() )
      continue;

    // Ignore the unit if it is in one of the following states
    if ( u->isLoaded() || u->isUnpowered() || u->isStuck() )
      continue;

    // Ignore the unit if it is incomplete or busy constructing
    if ( !u->isCompleted() || u->isConstructing() )
      continue;


    // Finally make the unit do some stuff!


    // If the unit is a worker unit
    if ( u->getType().isWorker() )
    {

      // if our worker is idle
      if ( u->isIdle() )
      {
        // Order workers carrying a resource to return them to the center,
        // otherwise find a mineral patch to harvest.
        if ( u->isCarryingGas() || u->isCarryingMinerals() )
        {
          u->returnCargo();
        }
        else if ( !u->getPowerUp() )  // The worker cannot harvest anything if it
        {                             // is carrying a powerup such as a flag
          
          // Harvest from the nearest mineral patch or gas refinery
          if ( !u->gather( u->getClosestUnit( IsMineralField || IsRefinery )) )
          {

            // If the call fails, then print the last error message
            Starcraft << Starcraft->getLastError() << std::endl;

          }

        } // closure: has no powerup
      } // closure: if idle

    }
    else if ( u->getType().isResourceDepot() ) // A resource depot is like a Command Center, Nexus, or Hatchery
    {

      // Order the depot to construct more workers! But only when it is idle.
      if ( u->isIdle() && !u->train(u->getType().getRace().getWorker()) )
      {

        // If that fails, draw the error at the location so that you can visibly see what went wrong!
        Starcraft->drawTextMap( u->getPosition().x, u->getPosition().y, "%s", Starcraft->getLastError().c_str() );
      }

    }

  } // closure: unit iterator
}

void ExampleAIModule::onSendText(std::string text)
{

  // Send the text to the game if it is not being processed.
  Starcraft->sendText("%s", text.c_str());


  // Make sure to use %s and pass the text as a parameter,
  // otherwise you may run into problems when you use the %(percent) character!

}

void ExampleAIModule::onReceiveText(BWAPI::Player* player, std::string text)
{
  // Parse the received text
  Starcraft << player->getName() << " said \"" << text << "\"" << std::endl;
}

void ExampleAIModule::onPlayerLeft(BWAPI::Player* player)
{
  // Interact verbally with the other players in the game by
  // announcing that the other player has left.
  Starcraft->sendText("Goodbye %s!", player->getName().c_str());
}

void ExampleAIModule::onNukeDetect(BWAPI::Position target)
{

  // Check if the target is a valid position
  if ( target )
  {
    // if so, print the location of the nuclear strike target
    Starcraft << "Nuclear Launch Detected at " << target << std::endl;
  }
  else 
  {

    // Otherwise, ask other players where the nuke is!
    Starcraft->sendText("Where's the nuke?");

  }

  // You can also retrieve all the nuclear missile targets using Starcraft->getNukeDots()!

}

void ExampleAIModule::onUnitDiscover(BWAPI::Unit* unit)
{
}

void ExampleAIModule::onUnitEvade(BWAPI::Unit* unit)
{
}

void ExampleAIModule::onUnitShow(BWAPI::Unit* unit)
{
}

void ExampleAIModule::onUnitHide(BWAPI::Unit* unit)
{
}

void ExampleAIModule::onUnitCreate(BWAPI::Unit* unit)
{
  if ( Starcraft->isReplay() )
  {
    // if we are in a replay, then we will print out the build order of the structures
    if ( unit->getType().isBuilding() && !unit->getPlayer()->isNeutral() )
    {
      int seconds = Starcraft->getFrameCount()/24;
      int minutes = seconds/60;
      seconds %= 60;
      Starcraft->sendText("%.2d:%.2d: %s creates a %s", minutes, seconds, unit->getPlayer()->getName().c_str(), unit->getType().getName().c_str());
    }
  }
}

void ExampleAIModule::onUnitDestroy(BWAPI::Unit* unit)
{
}

void ExampleAIModule::onUnitMorph(BWAPI::Unit* unit)
{
  if ( Starcraft->isReplay() )
  {
    // if we are in a replay, then we will print out the build order of the structures
    if ( unit->getType().isBuilding() && !unit->getPlayer()->isNeutral() )
    {
      int seconds = Starcraft->getFrameCount()/24;
      int minutes = seconds/60;
      seconds %= 60;
      Starcraft->sendText("%.2d:%.2d: %s morphs a %s", minutes, seconds, unit->getPlayer()->getName().c_str(), unit->getType().getName().c_str());
    }
  }
}

void ExampleAIModule::onUnitRenegade(BWAPI::Unit* unit)
{
}

void ExampleAIModule::onSaveGame(std::string gameName)
{
  Starcraft << "The game was saved to \"" << gameName << "\"" << std::endl;
}

void ExampleAIModule::onUnitComplete(BWAPI::Unit *unit)
{
}
