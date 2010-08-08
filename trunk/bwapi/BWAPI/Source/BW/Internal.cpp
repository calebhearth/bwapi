#include "Offsets.h"


namespace BW
{
  TileID      *BWDATA_ZergCreepArray  = NULL;
  TileID      *BWDATA_MapTileArray    = NULL;
  TileType    *BWDATA_TileSet         = NULL;
  DoodatType  *BWDATA_DoodatSet       = NULL;

  char *BWDATA_StringTableOff = NULL;
  u32  *BWDATA_MapFogOfWar    = NULL;

  MiniTileMaps_type* BWDATA_MiniTileFlags = NULL;

  const char *GetStatString(int index)
  {
    if ( BWDATA_StringTableOff )
      return (const char*)&BWDATA_StringTableOff[*(u16*)&BWDATA_StringTableOff[index * 2]];
    else
      return "Unable to initialize BWDATA_StringTableOff.";
  }

};
