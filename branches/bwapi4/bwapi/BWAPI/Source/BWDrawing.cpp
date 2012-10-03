#include "GameDrawing.h"

#include <BW/CImage.h>
#include <BW/CSprite.h>
#include <BW/CThingy.h>
#include <BW/Dialog.h>
#include <BW/Offsets.h>
#include <BW/TileType.h>


/*
void BlitToBitmap(DWORD dwOffset, int height, BYTE *pbBuffer, BYTE *pbData)
{
  for ( int i = height; i; --i )
  {
    DWORD _dwDrawW = gdwDrawWdth;
    if ( gdwDrawWdth + dwOffset >= (480-32)*(640+32) )
    {
      if ( dwOffset < (480-32)*(640+32) )
      {
        _dwDrawW = gdwDrawWdth + dwOffset - (480-32)*(640+32);
        DWORD dwFixDrawCopySize = (gdwDrawWdth - _dwDrawW) & 0xFFFFFFFC;
        memcpy(pbBuffer, pbData, dwFixDrawCopySize);
        pbData   += dwFixDrawCopySize;
        pbBuffer += dwFixDrawCopySize;
      }
      pbData   -= (480-32)*(640+32);
      dwOffset -= (480-32)*(640+32);
    }
    DWORD dwDrawCopySize = _dwDrawW & 0xFFFFFFFC;
    memcpy(pbBuffer, pbData, dwDrawCopySize);
    dwOffset += 640 + 32;
    pbData   = &pbData  [dwDrawCopySize + 640 + 32] - gdwDrawWdth;
    pbBuffer = &pbBuffer[dwDrawCopySize + 640] - gdwDrawWdth;
  }
}*/

void updateImageDrawingData()
{
  for ( int y = 0; y < BW::BWDATA::MapSize->y; ++y )
  {
    for ( BW::CSprite *s = BW::BWDATA::spriteGroups[y]; s; s = s->next )
    {
      for ( BW::CImage *i = s->underlay; i; i = i->next )
        i->updateGraphicData();
    }
  }
}

void blitGameText(int line, int x, int y)
{
  if ( BW::BWDATA::Chat_GameText[line].txt[0] )
  {
    char szString[256];
    szString[0] = 0;
    if ( BW::BWDATA::Chat_ColorBytes[line] )
    {
      szString[0] = BW::BWDATA::Chat_ColorBytes[line];
      szString[1] = 0;
    }
    strncat(szString, BW::BWDATA::Chat_GameText[line].txt, sizeof(BW::_gametext));
    BW::BlitText(szString, BW::BWDATA::GameScreenBuffer, x, y, 1);
  }
}

void drawGameText()
{
  int l = *BW::BWDATA::Chat_NextLine;

  int y = 112;
  for ( int i = 11; i > 0; i-- )
  {
    blitGameText(l, 10, y);
    y += *BW::BWDATA::Chat_IncrementY;
    l = (l + 1) % 11;
  }
  blitGameText(12, 10, 295);
  blitGameText(11, 420, 24);
}

void drawThingys()
{
  if ( *BW::BWDATA::wantThingyUpdate )
  {
    BW::CThingy *pThingyFirst = (*BW::BWDATA::ThingyList_UsedFirst);
    for ( BW::CImage *p = pThingyFirst->sprite->underlay; p; p = p->next )
      p->updateGraphicData();

    pThingyFirst->sprite->draw();
    for ( BW::CImage *p = pThingyFirst->sprite->underlay; p; p = p->next )
      p->flags |= 1;
  }
}

BW::activeTile *getActiveTile(int x, int y)
{
  return &(*BW::BWDATA::ActiveTileArray)[x + y * BW::BWDATA::MapSize->x];
}

WORD *getMtxTile(int x, int y)
{
  return &(*BW::BWDATA::MapTileArray)[x + y * BW::BWDATA::MapSize->x];
}

WORD *getCellTile(int x, int y)
{
  return &(*BW::BWDATA::CellMap)[x + y * BW::BWDATA::MapSize->x];
}

bool hasCreep(int x, int y)
{
  return (*getMtxTile(x,y) & 0x7FF0) == 0x10;
}

WORD getTileRef(WORD tile)
{
  return (*BW::BWDATA::TileSet)[(tile >> 4) & 0x7FF].megaTileRef[tile & 0xF];
}

#define TILE_SIZE (32)

BW::Position directions[] = { 
  BW::Position( 1, 1),
  BW::Position( 0, 1),
  BW::Position(-1, 1),
  BW::Position( 1, 0),
  BW::Position(-1, 0),
  BW::Position( 1,-1),
  BW::Position( 0,-1),
  BW::Position(-1,-1),
  BW::Position( 0, 0)
};

// bool (__stdcall *cb)(int direction, WORD *pMtxTile, int x, int y, int *lParam);
void IterateDirectionalCallback( bool (__stdcall *cb)(int,WORD*,int,int,int*), int x, int y, int *lParam = nullptr)
{
  BW::Position origin(x,y);
  for ( int i = 0; i < countof(directions); ++i )
  {
    BW::Position p = origin + directions[i];
    if ( p.isValid() && !cb(i, getMtxTile(p.x, p.y), p.x, p.y, lParam) )
      return;
  }
}

void drawMapTiles()
{
  BW::TilePosition moveTo = *BW::BWDATA::MoveToTile;
  BW::TilePosition mapMax = *BW::BWDATA::MapSize;
  int win_wid = BW::BWDATA::GameScreenBuffer->wid + 32;
  int win_hgt = BW::BWDATA::GameScreenBuffer->ht - 32;

  unsigned tileOffs = moveTo.x + (moveTo.y * mapMax.x);

  WORD *pCellMap;
  WORD *pMIMap;
  BW::activeTile *pActiveTiles;

  for ( int y = moveTo.y; y < min(moveTo.y + win_hgt/TILE_SIZE, mapMax.y); ++y )
  {
    for ( int x = moveTo.x; x < min(moveTo.x + win_wid/TILE_SIZE, mapMax.x); ++x )
    {
      WORD tileRef = getTileRef( *getMtxTile(x,y) );
      WORD *drawTile = getCellTile(x,y);
      if ( tileRef != *drawTile )
      {
        *drawTile = tileRef;
        getActiveTile(x,y)->bTemporaryCreep = hasCreep(x,y);

        IterateDirectionalCallback(BW::BWFXN_CreepManagementCB, x, y);

        //drawMegatileImageData(tileType, offset, __x, tileY);
        *BW::BWDATA::HasMegatileUpdate = true;
      }

    }
  }
}

void GameUpdate(BW::bitmap* /*pSurface*/, BW::bounds* /*pBounds*/)
{
  // if gameLayer.bits & 1 )
  {
    // maskSomething0();
    // memcpy(sgpMaskMapCopy, sgpCurrMaskMap, 408);
    updateImageDrawingData();   //BW::BWFXN_updateImageData();
    // maskSomething2();
    BW::BWFXN_drawMapTiles();
    BW::BWFXN_blitMapTiles();
  }
  // else
  //{
  //  maskSomething1();
  //  refreshImageRange(_top_y, _bot_y);
  //  maskSomething2();
  //  drawMapTiles();
  //  blitMapTiles2();
  //}

  //
  BW::BWFXN_drawAllSprites();// sprites
  // Space tileset stars/parallax thing goes here
  // updateAllFog(gameLayer.bits & 1);
  drawGameText();
  BW::BWFXN_drawDragSelBox();
  
  //BW::BWFXN_drawAllThingys(); // draw thingys
  drawThingys();
}
