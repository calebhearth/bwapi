#pragma once
#include "Type.h"

#define BWAPI_GAMETYPEDEF(x) static const GameType x(Enum::x) /** ref x */

namespace BWAPI
{
  namespace GameTypes
  {
    namespace Enum
    {
      enum Enum
      {
        None = 0,
        Custom,          // Warcraft III
        Melee,
        Free_For_All,
        One_on_One,
        Capture_The_Flag,
        Greed,
        Slaughter,
        Sudden_Death,
        Ladder,
        Use_Map_Settings,
        Team_Melee,
        Team_Free_For_All,
        Team_Capture_The_Flag,
        Unknown_0x0E,
        Top_vs_Bottom,
        Iron_Man_Ladder,    // Warcraft II

        Pro_Gamer_League = 32,  // Not valid
        Unknown,
        MAX
      };
    };
  };
  class GameType : public Type<GameType, GameTypes::Enum::Unknown>
  {
    public:
      GameType(int id = GameTypes::Enum::None);

      /** Returns the name of the game type. For example GameTypes::Melee.getName() will return an
       * std::string object containing "Melee". */
      const std::string &getName() const;
      const char *c_str() const;
  };
  namespace GameTypes
  {
    /** Given the name of a game type, this function will return the GameType. For example:
     *  GameTypes::getGameType("Free For All") will return GameTypes::Free_For_All. */
    GameType getGameType(std::string name);

    /** Returns the set of all the GameTypes. */
    const GameType::set& allGameTypes();
    void init();
    
    BWAPI_GAMETYPEDEF(Melee);
    BWAPI_GAMETYPEDEF(Free_For_All);
    BWAPI_GAMETYPEDEF(One_on_One);
    BWAPI_GAMETYPEDEF(Capture_The_Flag);
    BWAPI_GAMETYPEDEF(Greed);
    BWAPI_GAMETYPEDEF(Slaughter);
    BWAPI_GAMETYPEDEF(Sudden_Death);
    BWAPI_GAMETYPEDEF(Ladder);
    BWAPI_GAMETYPEDEF(Use_Map_Settings);
    BWAPI_GAMETYPEDEF(Team_Melee);
    BWAPI_GAMETYPEDEF(Team_Free_For_All);
    BWAPI_GAMETYPEDEF(Team_Capture_The_Flag);
    BWAPI_GAMETYPEDEF(Top_vs_Bottom);
    BWAPI_GAMETYPEDEF(None);
    BWAPI_GAMETYPEDEF(Unknown);
  }
}
