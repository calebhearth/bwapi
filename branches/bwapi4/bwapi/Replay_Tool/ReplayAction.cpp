#include "ReplayAction.h"

std::map<int, ReplayAction> ReplayAction::actionMap;


ReplayAction::ReplayAction()
{}

ReplayAction::ReplayAction(int iOpcode)
{
  this->actionMap[iOpcode] = *this;
};

size_t ReplayAction::Read(ReplayReader &reader) const
{
  return 0;
};
