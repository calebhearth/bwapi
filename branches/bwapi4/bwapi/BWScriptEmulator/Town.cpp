#include "Town.h"

Town::Town()
  : desiredWorkerCount(0)
  , currentWorkerCount(0)
  , location(BWAPI::Positions::Origin)
  , pTownCenter(nullptr)
{}

Town::Town( BWAPI::Position townLocation )
  : desiredWorkerCount(0)
  , currentWorkerCount(0)
  , location(townLocation)
  , pTownCenter(nullptr)
{}
