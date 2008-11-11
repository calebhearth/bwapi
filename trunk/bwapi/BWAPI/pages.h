/** @page localData Latency and local data issues

  Bw must distribute every command to all players before it can run it and
  it can't wait every game frame until all commands are distributed (would be laggy),
  so it has to run every coommand with deley.
  This deley is called latency, where \#L  is count of frames before commands take effect. 
  As it would be annoying to workaround the latency in the code, as you don't see any result of order
  until the \#L frames pass, we decided to make workaround tool for that directly 
  in our bw api.

  We will have 3 versions of bw data.
  -# Data that starcraft uses directly 
  -# Our copy (that is made every frame), so some multithread version could
     work with that without problems.
  -# Partialy precomputed version of data

  Let's explain it on example, let's assume you ordered your command center to start 
  building scv, but \#L the command center will still not start the build, so code 
  like if (cc->hasEmptyQueue()) -> cc->train(SCV), will order the command to build 
  the scv \#L + 1 times.

  So when you do builSCV command, the bwapi library will call that %command in bw, 
  but also call our function, that modifies the precomuted version of data 
  immediatly, store it (the command) in buffer, and keep it \#L frames, then every 
  frame, we will make fresh copy of bw data (version 3), and apply all commands 
  stored in the buffer manualy.

  This include for now (will be extended)
  -# Training : We will just place the unit to be built in the building queue, 
     decrease mineral count, and increase supply used (all in Local data version ofcourse).
     Note that we will probably implement negative % progress of the unit in construction)
  -# Right click order (Change of unit orderID and target(location/unit)).

  so the code will be (commandCenter->hasEmptyQueueLocal()) -> commandCenter->train(BWAPI::Prototypes::SCV) and 
  the preComputed (with Local postfix) state will change immedeatly.

  Everywhere the "Local" prefix is used, the function/data involves this kind of precomputed data.

  This issue concerns
  <ul>
    <li>BWAPI::Unit#getRawDataLocal</li>
    <li>BWAPI::Unit#getBuildQueueLocal</li>
    <li>BWAPI::Unit#getBuildQueueSlotLocal</li>
    <li>BWAPI::Unit#hasEmptyBuildQueueLocal</li>
    <li>BWAPI::Unit#getOrderIDLocal</li>
    <li>BWAPI::Unit#getTargetLocal</li>
    <li>BWAPI::Unit#getOrderTargetLocal</li>
    <li>BWAPI::Player#getMineralsLocal</li>
    <li>BWAPI::Player#getGasLocal</li>
    <li>BWAPI::Player#getSuppliesAvailableLocal</li>
    <li>BWAPI::Player#getSuppliesUsedLocal</li>
    <li>BWAPI::Player#freeSuppliesFreeLocal</li>
  </ul>


  The user of the library must be aware, that we can't guarantee that the preComputed 
  state will really occur, for example if you order to build scv, and just before 
  the command gets executed you lose minerals, the command will fail.
*/

/** @page functionality List of library functionality
The library provides easy

BWAPI
-# Interface to access units (and it's most important prototype data)
-# Interface to acces players (minerals/gas/supplies/names/forces)
-# Select/RightClick/Train/Upgrade/Invent command execution
-# In game lobby commands (Slot State/Race change command execution)
-# Game start command execution (form chat lobby)
-# catch start/end events, nextFrame, removeUnit events.
-# Precompute game state according to commands immediatly (@ref localData)
-# Compute the distance of units
-# Acces the tile map
-# Recognize buildability map (it also generates nice txt file describing the buildability map for the current map)
BWAI
Documentation of data files - usage of the bwai will be made eventually, current data files are easy tutorial
to understand it's function now.

-# Loading of premated map bulding definitions
-# Loading and execution of simple build order commands (making buildings, training untis, ugprades, techs)
-# Support of simple formulas and equations in the build order conditions
Next task is to make the priority work properly.

All things we would currently like to investigate/develop should be in the issue list http://code.google.com/p/bwapi/issues/list

*/