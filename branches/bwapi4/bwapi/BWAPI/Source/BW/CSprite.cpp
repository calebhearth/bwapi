#include "CSprite.h"
#include "CImage.h"
#include <Windows.h>

namespace BW
{
  void CSprite::draw()
  {
    BYTE color = 0;
    //BYTE color = getColourID(this->playerID);
    
    //imgDrawPalettteIndex[2] = tunit_image_palette[2*color];
    //imgDrawPalettteIndex[3] = tunit_image_palette[2*color + 1];
    //gdrawColorID = color;
    
    //if ( this->mainGraphic )
      // unknownColorShiftSomething(this->mainGraphic->paletteType, this->playerID);

    for ( auto i = this->overlay; i; i = i->prev )
      i->drawImage();
  }






}
