#include <BWAPI/Position.h>
#include <BWAPI/Game.h>

namespace BWAPI
{
  template<typename _T, int __Scale>
  bool Point<_T,__Scale>::isValid() const
  {
    // Not valid if < 0
    if ( this->x() < 0 || this->y() < 0 )
      return false;
    
    // If Broodwar pointer is not initialized, just assume maximum map size
    if ( !Broodwar ) 
      return this->x() < (256 * 32)/__Scale &&
              this->y() < (256 * 32)/__Scale;

    // If BW ptr exists then compare with actual map size
    return this->x() < (Broodwar->mapWidth()  * 32)/__Scale && 
           this->y() < (Broodwar->mapHeight() * 32)/__Scale;
  }

  template<typename _T, int __Scale>
  Point<_T,__Scale> &Point<_T,__Scale>::makeValid()
  {
    // Set x/y to 0 if less than 0
    if ( this->_x < 0 ) this->_x = 0;
    if ( this->_y < 0 ) this->_y = 0;
    
    if ( !Broodwar )  // If broodwar ptr doesn't exist, set to below max size
    {
      _T max = (256*32)/__Scale - 1;
      if ( this->_x > max ) this->_x = max;
      if ( this->_y > max ) this->_y = max;
      return *this;
    }

    // If BW ptr exists then set it to map width/height
    _T max = (_T)(Broodwar->mapWidth() * 32)/__Scale - 1;
    if ( this->_x > max ) this->_x = max;

    max = (_T)(Broodwar->mapHeight() * 32)/__Scale - 1;
    if ( this->_y > max ) this->_y = max;
    return *this;
  };

  template Point<int,32> &Point<int,32>::makeValid();
  template Point<int,8> &Point<int,8>::makeValid();
  template Point<int,1> &Point<int,1>::makeValid();
  template Point<short,32> &Point<short,32>::makeValid();
  template Point<short,1> &Point<short,1>::makeValid();
  
  template bool Point<int,32>::isValid() const;
  template bool Point<int,8>::isValid() const;
  template bool Point<int,1>::isValid() const;
  template bool Point<short,32>::isValid() const;
  template bool Point<short,1>::isValid() const;
}

