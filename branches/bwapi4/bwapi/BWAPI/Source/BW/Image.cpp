#include "Image.h"

#include <BW/Dialog.h>
#include <BW/Sprite.h>

namespace BW
{
  /////////////////////////////////////// IMAGE functions
  grpFrame *Image::getCurrentFrame() const
  {
    return &this->GRPFile->frames[this->frameIndex];
  }
  BW::Position Image::getPosition() const
  {
    return Position(this->horizontalOffset, this->verticalOffset) + this->spriteOwner->position;
  }
  void Image::updateGraphicData()
  {
    grpHead  *pGrpHead  = this->GRPFile;
    grpFrame *pGrpFrame = this->getCurrentFrame();
    
    if ( !(this->flags & 2) ) // Sets the left boundary of the image
      this->mapPosition.x = this->getPosition().x - pGrpHead->width/2 + pGrpFrame->left;
    else // Not sure
      this->mapPosition.x = this->getPosition().x + pGrpHead->width/2 - (pGrpFrame->right + pGrpFrame->left);

    if ( !(this->flags & 4) ) // Sets the top boundary of the image
      this->mapPosition.y = this->getPosition().y - pGrpHead->height/2 + pGrpFrame->top;

    rect bounds = { 0, 0, pGrpFrame->right, pGrpFrame->bottom };
    BW::Position screen(this->mapPosition);
    screen -= Position((short)*BW::BWDATA_MoveToX, (short)*BW::BWDATA_MoveToY);
    
    if ( screen.x < 0 )
    {
      bounds.right  += screen.x;  // screenX is negative
      bounds.left   += -screen.x;  // -screenX is positive
      screen.x  = 0;
    }
    if ( screen.y < 0 )
    {
      bounds.bottom += screen.y;   // screenY is negative
      bounds.top    += -screen.y;  // -screenY is positive
      screen.y  = 0;
    }
    bounds.right  = (s16)std::min<int>(bounds.right,  BW::BWDATA_GameScreenBuffer->wid - screen.x);
    bounds.bottom = (s16)std::min<int>(bounds.bottom, BW::BWDATA_GameScreenBuffer->ht  - screen.y);

    this->screenPosition = screen;
    this->grpBounds = bounds;
  }

  void Image::drawImage()
  {
    if ( (this->flags & 0x40)       && 
         this->grpBounds.bottom > 0 &&
         this->grpBounds.right  > 0 )
    {
      if ( (this->flags & 1) /*|| this->needsRefresh()*/ )
      {
        RECT rctDraw = { this->grpBounds.left, this->grpBounds.top, this->grpBounds.right, this->grpBounds.bottom };
        this->renderFunction(this->screenPosition.x, this->screenPosition.y, &this->GRPFile->frames[this->frameIndex], &rctDraw, (int)this->coloringData);
      }
    }
    this->flags &= 0xFFFE;
  }
}
