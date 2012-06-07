#include "Replay.h"

#include "PKShared.h"

#include <fstream>
#include "FileReader.h"
#include "FileWriter.h"
#include "ReplayReader.h"

#include "RepHeader.h"
#include "Actions.h"

replay_resource replayHeader;

void WriteBuffer(const char *pszFormat, const char *pszFilename, void *pBuffer, DWORD dwBufferSize)
{
  FileWriter fw;
  char szTmp[MAX_PATH];
  sprintf(szTmp, pszFormat, pszFilename);
  if ( fw.Open( szTmp ) )
    fw.WriteRaw(pBuffer, dwBufferSize);
}

bool errSimple(const char *pszText)
{
  //MessageBox(NULL, pszText, NULL, 0);
  std::ofstream log("Replay_errLog.log", std::ios_base::app);
  log << pszText << "\n";
  log.close();
  return false;
}

bool ParseReplay(const char *pszFilename, DWORD dwFlags)
{
  // Open replay file
  FileReader fr;
  if ( !fr.Open(pszFilename) )
    return false;

///////////////////// Header
  // Read replay resource identifier
  DWORD dwRepResourceID = 0;
  // Best guess: "reRS" is "replay RESOURCE"
  if ( !DecompressRead(&dwRepResourceID, sizeof(dwRepResourceID), fr) || dwRepResourceID != mmioFOURCC('r','e','R','S') )
    return errSimple("No Replay resource ID found.");

  // Read replay resource header
  if ( !DecompressRead(&replayHeader, sizeof(replayHeader), fr) )
    return errSimple("Unable to read replay header.");

////////////////// Actions
  // Read replay actions section size
  DWORD dwActionBufferSize = 0;
  if ( !DecompressRead(&dwActionBufferSize, 4, fr) )
    return errSimple("Unable to read actions size.");

  // Allocate and Read replay actions
  ReplayReader repActions(dwActionBufferSize);
  if ( dwActionBufferSize && (!repActions || !DecompressRead(repActions, dwActionBufferSize, fr)) )
    return errSimple("Decompressing actions failed.");

/////////////////// Map Chk
  // get map chunk data size
  DWORD dwChkBufferSize = 0;
  if ( !DecompressRead(&dwChkBufferSize, 4, fr) )
    return errSimple("Unable to read chk size.");

  // Allocate and Read chk data
  void *pChkBuffer = malloc(dwChkBufferSize);
  //FileReader frChk(pChkBuffer, dwChkBufferSize);
  if ( dwChkBufferSize && !DecompressRead(pChkBuffer, dwChkBufferSize, fr) )
    return errSimple("Decompressing map failed.");

  // Write extracted replay data
  if ( dwFlags & RFLAG_EXTRACT )
  {
    WriteBuffer("%s.hdr", pszFilename, &replayHeader, sizeof(replayHeader));
    WriteBuffer("%s.act", pszFilename, repActions, dwActionBufferSize);
    WriteBuffer("%s.chk", pszFilename, pChkBuffer, dwChkBufferSize);
  }

  // parse data for repair
  if ( dwFlags & RFLAG_REPAIR )
  {
    // Parse replay actions
    ParseActions(repActions, pszFilename);

    if ( replayHeader.dwFrameCount < repActions.highestFrameTick() )
    {
      std::ofstream log("Results.txt", std::ios_base::app);
      log << pszFilename << " -- Fixed replay with " << replayHeader.dwFrameCount << " frames. Desired: " << repActions.highestFrameTick() << " frames.\n";

      replayHeader.dwFrameCount = repActions.highestFrameTick() + 100;

      // Repair/reconstruct the replay
      FileWriter fw;
      fw.Open(pszFilename);

      // write rep resource id
      dwRepResourceID = mmioFOURCC('r','e','R','S');
      CompressWrite(&dwRepResourceID, sizeof(dwRepResourceID), fw);

      // write header
      CompressWrite(&replayHeader, sizeof(replayHeader), fw);

      // write actions
      CompressWrite(&dwActionBufferSize, sizeof(dwActionBufferSize), fw);
      if ( dwActionBufferSize )
        CompressWrite(repActions, repActions.size(), fw);
    
      // write chk
      CompressWrite(&dwChkBufferSize, sizeof(dwChkBufferSize), fw);
      if ( dwChkBufferSize )
        CompressWrite(pChkBuffer, dwChkBufferSize, fw);

      log.close();
    } // if replay is damaged
  }

  return true;
}

