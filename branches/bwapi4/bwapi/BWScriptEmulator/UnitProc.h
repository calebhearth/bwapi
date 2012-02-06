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

	int			 iEmulatedOrder;
	Position	guardLocation;
	Position	destLocation;
	Unit			*thisUnit;
	bool			initialized;
	int			 dwState;
	int			 aiCaptain;
};

UnitProc *getUnitProc(BWAPI::Unit *pUnit);
