#include "Utilities.h"

#include <Util/RectangleArray.h>
#include <Util/Strings.h>
#include <Util/Logger.h>

#include <BWAI/Unit.h>
#include <BWAI/Globals.h>
#include <BWAI/AI.h>
#include <BWAPI/Map.h>
#include <BWAPI/ScreenLogger.h>


namespace PathFinding
{
  //------------------------------------------ CONSTRUCTOR ---------------------------------------------------
  Utilities::Utilities()
  :world      (BWAPI::Map::getWidth()*4 + 2, BWAPI::Map::getWidth()*4 + 2)
  ,walkability(BWAPI::Map::getWidth()*4 + 2, BWAPI::Map::getWidth()*4 + 2)
  ,vaweID(0)
  {
    Spot pattern;
    pattern.distance = 0; // This really doesn't matter
    pattern.vaweID = 0;
    pattern.from = Direction::Unset;
    world.setTo(pattern);
    for (u16 x = 0; x < BWAPI::Map::getWidth()*4; x++)
      for (u16 y = 0; y < BWAPI::Map::getHeight()*4; y++)
        walkability[x + 1][y + 1] = BWAI::ai->map->getWalkabilityArray()[x][y];
    for (unsigned int x = 0; x < walkability.getWidth(); x++)
    {
      walkability[x][0] = false;
      walkability[x][walkability.getHeight() - 1] = false;
    }
    
    for (unsigned int y = 0; y < walkability.getHeight(); y++)
    {
      walkability[0][y] = false;
      walkability[walkability.getWidth() - 1][y] = false;
    }
    for (int i = 0; i < SPOT_DISTANCE_WINDOW_SIZE; i++)
      count[i] = 0;
    for (int i = 0; i < BW::UNIT_TYPE_COUNT; i++)
    {
      BW::UnitType type((BW::UnitID::Enum)i);
      if (type.isBuilding() ||
          type.isFlayer() ||
          type.isNeutral())
        {
          this->precomputedPlacebility[i] = NULL;
          continue;
        }
      int borderLeft   = (type.dimensionLeft()  + 7)/8;
      int borderRight  = (type.dimensionRight() + 7)/8;
      int borderTop    = (type.dimensionUp()    + 7)/8;
      int borderBottom = (type.dimensionDown()  + 7)/8;
      u32 dimensions = borderLeft          + 
                       (borderRight  << 8 ) +
                       (borderTop    << 16) +
                       (borderBottom << 24);
      std::map<u32, Util::RectangleArray<bool>*>::iterator value = this->precomputedPlacebilityContent.find(dimensions);
      if (value != this->precomputedPlacebilityContent.end())
        this->precomputedPlacebility[i] = (*value).second;
      else
      {
        this->precomputedPlacebility[i] = new Util::RectangleArray<bool>(BWAPI::Map::getWidth()*4  + 2, 
                                                                         BWAPI::Map::getHeight()*4 + 2);
        this->precomputedPlacebilityContent.insert(std::pair<u32, Util::RectangleArray<bool>*>
                                                            (dimensions, this->precomputedPlacebility[i]));
        BWAI::ai->log->log("Initilising for unit %22s with dimensions %2d %2d %2d %2d", type.getName(), borderLeft, borderRight, borderTop, borderBottom);
        BWAI::ai->log->log("is neutral = %d", type.isNeutral());
        precomputedPlacebility[i]->setTo(false);  
        WalkabilityPosition here;
        for (unsigned int x = borderLeft; x < precomputedPlacebility[i]->getWidth() - borderRight; x++)
        {
          here.x = x;
          for (unsigned int y = borderTop; y < precomputedPlacebility[i]->getHeight() - borderBottom; y++)
          {
            here.y = y;
            (*precomputedPlacebility[i])[x][y] = !Utilities::conflictsWithMap(here, type);
          }
        }
      }
    }
    BWAPI::ScreenLogger().log("Initialised %u walkability arrays", this->precomputedPlacebilityContent.size());
  }
  //--------------------------------------- UTILITIES --------------------------------------------------------
  Utilities::~Utilities()
  {
   for each (std::pair<u32, Util::RectangleArray<bool>*> i in this->precomputedPlacebilityContent)
     delete i.second;
  }
  //-------------------------------------- CONFLICTS WITH MAP ------------------------------------------------
  bool Utilities::conflictsWithMap(const WalkabilityPosition& position, const BW::UnitType& type)
  {
    int x1 = position.x - (type.dimensionLeft() + 7)/8;
    int x2 = position.x + (type.dimensionRight() + 7)/8;
    int y1 = position.y - (type.dimensionUp() + 7)/8;
    int y2 = position.y + (type.dimensionDown() + 7)/8;
    for (int x = x1; x <= x2; x++)
      for (int y = y1; y <= y2; y++)
        if (!this->walkability[x][y])
          return true;
    return false;
  }
  //------------------------------------------------------------------------------------------------
  bool Utilities::generatePath(const UnitModel& unit, WalkabilityPosition target)
  {
    this->vaweID++;
    Utilities::setDirectionBuffer(unit);
    #pragma region DisabledDebugOutput
   /* if (Utilities::conflictsWithMap(unit))
    {
      FILE* f = fopen("Path.txt","at");
      fprintf(f, "Conflicts\n");
      fclose(f); 
      return false;
    }*/
    #pragma endregion DisabledDebugOutput    
    // sets starting spots of the search
    for (int i = 0; i < SPOT_DISTANCE_WINDOW_SIZE; i++)
      count[i] = 0;
    for (int x = unit.walkabilityPosition.x; x <= (unit.position.x + 7)/8; x++)
      for (int y = unit.walkabilityPosition.y; y <= (unit.position.y + 7)/8; y++)
      {
        WalkabilityPosition here(x, y);
        u16 distance = here.toBWPosition().getDistance(unit.position);
        here.x ++;
        here.y ++;
        world[here.x][here.y].distance = distance;
        world[here.x][here.y].vaweID = this->vaweID;
        world[here.x][here.y].from = Direction::Near;
        vawe[distance][count[distance]] = here;
        count[distance]++;
      }
    
    bool couldGo[BASIC_DIRECTION_COUNT];
    u16 countOfStepsWithoutChanges;
    Spot spot;
    WalkabilityPosition here;
    WalkabilityPosition next;
    u16 newDistancePared, newDistanceAngledPared;
    u16 newDistance, newDistanceAngled;
    Util::RectangleArray<bool>* canStay = this->precomputedPlacebility[unit.original->getType().getID()];
    u16 position = 0;
    u16 distance = 0;
    
    for (; true; position = (position + 1) & SPOT_DISTANCE_WINDOW_SIZE_BITS, distance++)
    {
      if (vawe[position])
        countOfStepsWithoutChanges = 0;
      else
        countOfStepsWithoutChanges++;

      newDistance = distance + STRAIGHT_SPOT_DIRECTION;
      newDistancePared = newDistance & SPOT_DISTANCE_WINDOW_SIZE_BITS;
      newDistanceAngled = distance + ANGLED_SPOT_DIRECTION;
      newDistanceAngledPared = newDistanceAngled & SPOT_DISTANCE_WINDOW_SIZE_BITS;

      for (int i = 0; i < count[position]; i++)
      {
        if (vawe[position][i] == target)
          goto foundTarget;
        here = vawe[position][i];
        spot = world[here.x][here.y];
        
        for (u8 j = 0; j < BASIC_DIRECTION_COUNT; j++)
        {
          next.x = here.x + forwardDirection[j][0];
          next.y = here.y + forwardDirection[j][1];
          if ((*canStay)[next.x][next.y])
          //if (this->canMove(here, (Direction::Enum)j))
          {
            couldGo[j] = true;
            if (world[next.x][next.y].vaweID != this->vaweID ||
                world[next.x][next.y].distance > newDistance)
            {
              vawe[newDistancePared][count[newDistancePared]] = next;
              count[newDistancePared]++;
              world[next.x][next.y].vaweID = this->vaweID;
              world[next.x][next.y].from = (Direction::Enum) j;
              world[next.x][next.y].distance = newDistance;
            }
          }
          else 
            couldGo[j] = false;
        }
        for (u8 j = BASIC_DIRECTION_COUNT; j < ADVANCED_DIRECTION_COUNT; j++)       
          if (couldGo[directionConditions[j][0]] &&
              couldGo[directionConditions[j][1]])
          {
            next.x = here.x + forwardDirection[j][0];
            next.y = here.y + forwardDirection[j][1];
            if (world[next.x][next.y].vaweID != this->vaweID ||
                world[next.x][next.y].distance > newDistanceAngled)
            {
              vawe[newDistanceAngledPared][count[newDistanceAngledPared]] = next;
              count[newDistanceAngledPared]++;
              world[next.x][next.y].vaweID = this->vaweID;
              world[next.x][next.y].from = (Direction::Enum) j;
              world[next.x][next.y].distance = newDistanceAngled;
            }
          }
      }
     count[position] = 0;
     if (countOfStepsWithoutChanges >= SPOT_DISTANCE_WINDOW_SIZE)
       break;
    }
    #pragma region DisabledDebugOutput
    /* FILE* f = fopen("Path.txt","at");
    fprintf(f, "Path not found\n");
    
    Util::RectangleArray<char> resultArray = 
        Util::RectangleArray<char>(BWAI::ai->map->getWalkabilityArray().getWidth(), 
                                   BWAI::ai->map->getWalkabilityArray().getHeight());
    for (unsigned int x = 0; x < BWAI::ai->map->getWalkabilityArray().getWidth(); x++)
      for (unsigned int y = 0; y < BWAI::ai->map->getWalkabilityArray().getHeight(); y++)
        if (temp[x][y].first == -1)
          resultArray[x][y] = BWAI::ai->map->getWalkabilityArray()[x][y] ? '.' : 'X';
        else
          resultArray[x][y] = 'O';
    Util::Strings::makeBorder(resultArray).printToFile(f); 
    fclose(f); */
    #pragma endregion DisabledDebugOutput
    return false;
    foundTarget:
    #pragma region DisabledDebugOutput
    /*
    Util::RectangleArray<char> resultArray = 
        Util::RectangleArray<char>(BWAI::ai->map->getWalkabilityArray().getWidth(), 
                                   BWAI::ai->map->getWalkabilityArray().getHeight());
    for (unsigned int x = 0; x < BWAI::ai->map->getWalkabilityArray().getWidth(); x++)
      for (unsigned int y = 0; y < BWAI::ai->map->getWalkabilityArray().getHeight(); y++)
        resultArray[x][y] = BWAI::ai->map->getWalkabilityArray()[x][y] ? '.' : 'X';
    
    Direction::Enum direction = world[target.x][target.y].from;
    for (WalkabilityPosition i(target);
         direction != Direction::Near;
         direction = world[i.x][i.y].from,
         i.x += reverseDirection[direction][0],
         i.y += reverseDirection[direction][1])
      resultArray[i.x - 1][i.y - 1] = '0';
    FILE* f = fopen("Path.txt","at");
    Util::Strings::makeBorder(resultArray).printToFile(f); 
    fclose(f);*/
    #pragma endregion DisabledDebugOutput
    return true;
  }
  //------------------------------------------------------------------------------------------------
  bool Utilities::canMove(const WalkabilityPosition& position, Direction::Enum direction)
  {
    bool result = true;
    for (size_t i = 0; i < directionBuffer[direction].size(); i++)
      result &= this->walkability[position.x + directionBuffer[direction][i].first]
                                 [position.y + directionBuffer[direction][i].second];
    return result;                                                     
  }
  //------------------------------------------------------------------------------------------------
  void Utilities::setDirectionBuffer(const UnitModel& unit)
  {
    for (int i = 0; i < BASIC_DIRECTION_COUNT; i++)
      directionBuffer[i].clear();
    
    int x1 = (-unit.original->getType().dimensionLeft() - 7)/8;
    int x2 = ((unit.original->getType().dimensionRight()) + 7)/8;
    int y1 = (-unit.original->getType().dimensionUp() - 7)/8;
    int y2 = ((unit.original->getType().dimensionDown()) + 7)/8;
    
    for (int i = y1; i <= y2; i++)
    {
      directionBuffer[Direction::Left].push_back(std::pair<int, int>(x1 - 1,i));
      directionBuffer[Direction::Right].push_back(std::pair<int, int>(x2 + 1,i));
    }

    for (int i = x1; i <= x2; i++)
    {
      directionBuffer[Direction::Up].push_back(std::pair<int, int>(i, y1 - 1));
      directionBuffer[Direction::Down].push_back(std::pair<int, int>(i, y2 + 1));
    }
  }
  //------------------------------------------------------------------------------------------------
  bool Utilities::canStay(const BW::UnitType& type, const WalkabilityPosition& position) const
  {
    if (!this->precomputedPlacebility[type.getID()])
      return true;
    return (*this->precomputedPlacebility[type.getID()])[position.x][position.y];
  }
  //------------------------------------------------------------------------------------------------
}