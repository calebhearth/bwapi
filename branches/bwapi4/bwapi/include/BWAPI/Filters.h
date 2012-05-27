#pragma once
#include <BWAPI/UnaryFilter.h>
#include <BWAPI/ComparisonFilter.h>

namespace BWAPI
{
  class Unit;

  typedef UnaryFilter<Unit*> UnitFilter;

  extern const UnitFilter Workers;
  extern const UnitFilter Transports;
  extern const UnitFilter ResourceDepots;

  extern const CompareFilter<Unit*> HP;
  extern const CompareFilter<Unit*> HP_Percent;
  extern const CompareFilter<Unit*> Shields;
  extern const CompareFilter<Unit*> Shields_Percent;
  extern const CompareFilter<Unit*> Energy;
  extern const CompareFilter<Unit*> Energy_Percent;

  extern const CompareFilter<Unit*> GetType;

}

