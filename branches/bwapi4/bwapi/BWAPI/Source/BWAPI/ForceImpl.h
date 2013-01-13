#pragma once
#include <string>
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
      virtual int getID() const override;
      virtual std::string getName() const override;
      virtual Playerset getPlayers() const override;
      Playerset players;
      std::string name;
      void setID(int newID);
    private:
      int id;
  };
};
