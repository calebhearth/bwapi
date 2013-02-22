#pragma once
#include <cstdio>
#include <map>
class ReplayReader;

class ReplayAction
{
public:
  ReplayAction();
  ReplayAction(int iOpcode);

  virtual size_t Read(ReplayReader &reader) const;


  static std::map<int, ReplayAction> actionMap;
};
