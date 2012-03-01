#pragma once
#include <BWAPI.h>
#include <set>
#include <vector>

using namespace BWAPI;

class UnitProc
{
public:
  UnitProc();
  UnitProc(Unit *pUnit);
  ~UnitProc();
  void Init();
  void Init(Unit *pUnit);
  void EmulateIdleOrder();
  void execute();
  void StandardUnitProc();

  Position  guardLocation;
  Position  destLocation;
  Unit    *thisUnit;
  int      iEmulatedOrder;
  int      dwState;
  int      aiCaptain;
  bool    initialized;
};

UnitProc *getUnitProc(BWAPI::Unit *pUnit);
