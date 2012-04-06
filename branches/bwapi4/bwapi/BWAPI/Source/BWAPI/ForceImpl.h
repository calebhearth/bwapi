#pragma once
#include <BWAPI/Force.h>
#include <BWAPI/Playerset.h>

namespace BWAPI
{
  // Forwards
  class Player;

  class ForceImpl : public Force
  {
    public:
      ForceImpl(std::string name);
      virtual int getID() const;
      virtual std::string getName() const;
      virtual Playerset getPlayers() const;
      Playerset players;
      std::string name;
      void setID(int newID);
    private:
      int id;
  };
};
