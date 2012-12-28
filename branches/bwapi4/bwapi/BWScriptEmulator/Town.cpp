#include "Town.h"

Town::Town()
  : desiredWorkerCount(0)
  , currentWorkerCount(0)
  , location(BWAPI::Positions::Origin)
  , pTownCenter(nullptr)
{}

