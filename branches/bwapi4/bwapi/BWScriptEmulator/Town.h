#pragma once
#include <BWAPI.h>
#include <map>

namespace TaskType
{
  enum Enum
  {
    Build,
    Research,
    Upgrade,
    Need
  };
}

class Town
{
public:
  Town();

private:
  BWAPI::Unitset townWorkers;
  BWAPI::Unitset townTaskers;

  int desiredWorkerCount;
  int currentWorkerCount;

  BWAPI::Position location;

  BWAPI::Unit *pTownCenter;
  BWAPI::Unitset resources;
  BWAPI::Unitset gasGeysers;

  std::multimap<int, std::pair<TaskType::Enum,int> > townQueue;
};
