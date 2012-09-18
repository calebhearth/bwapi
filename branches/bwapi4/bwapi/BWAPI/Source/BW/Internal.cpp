#include "Offsets.h"


namespace BW
{
  namespace BWDATA
  {
    TileID      *MapTileArray    = NULL;
    TileType    *TileSet         = NULL;
    SAI_Paths   *SAIPathing      = NULL;
    char        *StringTableOff  = NULL;
    activeTile  *ActiveTileArray = NULL;
    MiniTileMaps_type* MiniTileFlags = NULL;
  }
  void (__stdcall *pOldDrawGameProc)(BW::bitmap *pSurface, BW::bounds *pBounds) = NULL;
  void (__stdcall *pOldDrawDialogProc)(BW::bitmap *pSurface, BW::bounds *pBounds) = NULL;

  const char *GetStatString(int index)
  {
    if ( BWDATA::StringTableOff )
      return (const char*)&BWDATA::StringTableOff[(u16&)BWDATA::StringTableOff[index * 2 + 2]];
    else
      return "Unable to initialize BWDATA::StringTableOff.";
  }

};
