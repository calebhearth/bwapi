#pragma once
#include <string>
//#include "Position.h"

AGENT_STRIP;

namespace BWAgent
{
  class Unit;
  class Player;
  class AIModule
  {
    public:
      AIModule();
      virtual ~AIModule();
      virtual void onStartMatch();
      virtual void onFrame();
      virtual void onSendText(std::string text);
  };
}
