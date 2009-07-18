#pragma once
#include <vector>
#include <map>

#include <BWAPI.h>
#include <Util/RectangleArray.h>

#include "UnitModel.h"
#include "WalkabilityPosition.h"


/* All the stuff here will be sorted somehow soon . */
static const int BASIC_DIRECTION_COUNT = 4;
static const int ADVANCED_DIRECTION_COUNT = 8;

namespace Direction
{
enum Enum : u8
  {
    Up        = 0,
    Right     = 1,
    Down      = 2,
    Left      = 3,
    UpRight   = 4,
    RightDown = 5,
    DownLeft  = 6,
    LeftUp    = 7,
    Near      = 8,
    Unset     = 9
  };
}

static const int forwardDirection[ADVANCED_DIRECTION_COUNT][2] =
{
 { 0, -1},
 { 1,  0},
 { 0,  1},
 {-1,  0},
 { 1, -1},
 { 1,  1},
 {-1,  1},
 {-1, -1},
};


static const int reverseDirection[ADVANCED_DIRECTION_COUNT][2] =
{
 { 0,  1},
 {-1,  0},
 { 0, -1},
 { 1,  0},
 {-1,  1},
 {-1, -1},
 { 1, -1},
 { 1,  1}, 
};

static Direction::Enum directionConditions[ADVANCED_DIRECTION_COUNT][2] =
{
  {Direction::Unset , Direction::Unset},
  {Direction::Unset , Direction::Unset},
  {Direction::Unset , Direction::Unset},
  {Direction::Unset , Direction::Unset},
  {Direction::Up    , Direction::Right},
  {Direction::Right , Direction::Down},
  {Direction::Down  , Direction::Left},
  {Direction::Left  , Direction::Up},
};
/**
 * All stuff related directly to path finding.
 */
namespace PathFinding
{
  /**
   * Utilities related to pathfinding.
   */
  class Utilities
  {
    public:
      Utilities();
      ~Utilities();
      bool generatePath(const UnitModel& unit, WalkabilityPosition target);
      /** static help variables. */
      bool canStay(const BWAPI::UnitType& type, const WalkabilityPosition& position) const;
    private :
      static const u16 STRAIGHT_SPOT_DIRECTION = 8;
      static const u16 ANGLED_SPOT_DIRECTION = 11; // Near to sqrt(8^2+8^2)
      static const u16 SPOT_DISTANCE_WINDOW_SIZE = 16; /**< should be 2^n as I will divide the distance by
                                                       *   this and division by 2^n is a LOT faster as it is
                                                       *   just bit shift. */
      static const u16 SPOT_DISTANCE_WINDOW_SIZE_BITS = 15;
      static const u16 MAXIMUM_COUNT_OF_SPOTS_IN_THE_SAME_DISTANCE = 5000;
      struct Spot
      {
        u32 vaweID;
        u16 distance;
        Direction::Enum from;
      };
      Util::RectangleArray<Spot> world;
      Util::RectangleArray<bool> walkability;
      u32 vaweID;
      WalkabilityPosition vawe[SPOT_DISTANCE_WINDOW_SIZE][MAXIMUM_COUNT_OF_SPOTS_IN_THE_SAME_DISTANCE];
      u16 count[SPOT_DISTANCE_WINDOW_SIZE];

      bool canMove(const WalkabilityPosition& position, Direction::Enum direction);
      void setDirectionBuffer(const UnitModel& unit);
      std::vector<std::pair<int, int> > directionBuffer[BASIC_DIRECTION_COUNT];
      /** Quite huge structure that could optimise the path finding, it contains info about placability of
       * unit on every spot.
       */
      std::map<BWAPI::UnitType, Util::RectangleArray<bool>* > precomputedPlacebility;
      std::map<u32, Util::RectangleArray<bool>*> precomputedPlacebilityContent;
      bool Utilities::conflictsWithMap(const WalkabilityPosition& position, const BWAPI::UnitType& type);
  };
}