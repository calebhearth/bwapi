#pragma once
#include <BWAPI/UnaryFilter.h>
#include <BWAPI/ComparisonFilter.h>

namespace BWAPI
{
  class Unit;

  extern const UnaryFilterBase<Unit*> Workers;
  extern const UnaryFilterBase<Unit*> Transports;
  extern const UnaryFilterBase<Unit*> ResourceDepots;

  extern const CompareFilterBase<Unit*> HP;
  extern const CompareFilterBase<Unit*> HP_Percent;
  extern const CompareFilterBase<Unit*> Shields;
  extern const CompareFilterBase<Unit*> Shields_Percent;
  extern const CompareFilterBase<Unit*> Energy;
  extern const CompareFilterBase<Unit*> Energy_Percent;

  extern const CompareFilterBase<Unit*> GetType;

}

