#include <windows.h>
#include <string>
#include <vector>
#include <math.h>
#include <Util/Exceptions.h>
#include "../Storm/storm.h"

#include "WMode.h"
#include "Resolution.h"
#include "Holiday/Holiday.h"

#include "Detours.h"
#include "BWAPI/GameImpl.h"
#include "BWAPI/PlayerImpl.h"
#include "BW/Offsets.h"
#include "DLLMain.h"
#include "NewHackUtil.h"
#include "BW/TriggerEngine.h"

#include "../../Debug.h"

bool hideHUD;
char gszScreenshotFormat[4];
char gszDesiredReplayName[MAX_PATH];

void *leakUIClassLoc;
void *leakUIGrpLoc;

BOOL   (STORMAPI *_SNetLeaveGameOld)(int type);
int    (STORMAPI *_SStrCopyOld)(char *dest, const char *source, size_t size);
BOOL   (STORMAPI *_SNetReceiveMessageOld)(int *senderplayerid, u8 **data, int *databytes);
BOOL   (STORMAPI *_SFileOpenFileExOld)(HANDLE hMpq, const char *szFileName, DWORD dwSearchScope, HANDLE *phFile);
BOOL   (STORMAPI *_SFileOpenFileOld)(const char *filename, HANDLE *phFile);
void*  (STORMAPI *_SMemAllocOld)(int amount, char *logfilename, int logline, char defaultValue);
BOOL   (STORMAPI *_SNetSendTurnOld)(char *data, unsigned int databytes);
BOOL   (STORMAPI *_SDrawCaptureScreenOld)(const char *pszOutput);
HANDLE (WINAPI   *_FindFirstFileOld)(LPCSTR lpFileName, LPWIN32_FIND_DATA lpFindFileData);
BOOL   (WINAPI   *_DeleteFileOld)(LPCTSTR lpFileName);
DWORD  (WINAPI   *_GetFileAttributesOld)(LPCTSTR lpFileName);
HANDLE (WINAPI   *_CreateFileOld)(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
HWND   (WINAPI   *_CreateWindowExAOld)(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
VOID   (WINAPI   *_SleepOld)(DWORD dwMilliseconds);

bool (__fastcall *BWTriggerActionFxnTable[60])(BW::Triggers::Action*);
bool __fastcall TriggerActionReplacement(BW::Triggers::Action *pAction)
{
  bool rval = BWTriggerActionFxnTable[pAction->bActionType](pAction);
  if ( rval )
  {
    // do stuff
  }
  return rval;
}

//------------------------------------------------ SLEEP ----------------------------------------------------
VOID WINAPI _Sleep(DWORD dwMilliseconds)
{
  if ( dwMilliseconds == 1500 ) // Main menu timer
    return;

  if ( _SleepOld )
    _SleepOld(dwMilliseconds);
}

//------------------------------------------- DIRECT DRAW INIT -----------------------------------------------
void DDInit()
{
  DDrawInitialize(640, 480);
}
//--------------------------------------------- CREATE WINDOW ------------------------------------------------
bool detourCreateWindow = false;
HWND WINAPI _CreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
  HWND hWndReturn = NULL;
  if ( strcmp(lpClassName, "SWarClass") == 0 )
  {
    detourCreateWindow = true;
    if ( switchToWMode )
    {
      HackUtil::CallPatch(BW::BWDATA_DDrawInitCallPatch, &DDInit);
      if ( _CreateWindowExAOld )
        hWndReturn = _CreateWindowExAOld(dwExStyle, 
                                          lpClassName, 
                                          lpWindowName, 
                                          dwStyle | WS_OVERLAPPEDWINDOW, 
                                          windowRect.left, 
                                          windowRect.top, 
                                          windowRect.right, 
                                          windowRect.bottom, 
                                          hWndParent, 
                                          hMenu, 
                                          hInstance, 
                                          lpParam);
      else
        hWndReturn = CreateWindowEx(dwExStyle, 
                                    lpClassName, 
                                    lpWindowName, 
                                    dwStyle | WS_OVERLAPPEDWINDOW, 
                                    windowRect.left, 
                                    windowRect.top, 
                                    windowRect.right, 
                                    windowRect.bottom, 
                                    hWndParent, 
                                    hMenu, 
                                    hInstance, 
                                    lpParam);
      ghMainWnd = hWndReturn;
      SetWMode(windowRect.right, windowRect.bottom, true);
    }
    else
    {
      if ( _CreateWindowExAOld )
        hWndReturn = _CreateWindowExAOld(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
      else
        hWndReturn = CreateWindowEx(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
      ghMainWnd = hWndReturn;
    }
    switchToWMode = false;
    // Obtain/hack WndProc
    wOriginalProc = (WNDPROC)GetWindowLong(hWndReturn, GWLP_WNDPROC);
    SetWindowLong(ghMainWnd, GWLP_WNDPROC, (LONG)&WindowProc);
  }
  else
  {
    if ( _CreateWindowExAOld )
      hWndReturn = _CreateWindowExAOld(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
    else
      hWndReturn = CreateWindowEx(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
  }
  return hWndReturn;
}

//----------------------------------------------- FILE HOOKS -------------------------------------------------
HANDLE WINAPI _FindFirstFile(LPCSTR lpFileName, LPWIN32_FIND_DATA lpFindFileData)
{
  const char *pszFile = lpFileName;
  if ( BWAPI::BroodwarImpl.autoMenuMapPath.size() > 0 && 
       BWAPI::BroodwarImpl.autoMenuMode != ""         &&
       BWAPI::BroodwarImpl.autoMenuMode != "OFF"      &&
       BWAPI::BroodwarImpl.lastMapGen.size() > 0      &&
       strstr(lpFileName, "*.*")  )
    pszFile = BWAPI::BroodwarImpl.lastMapGen.c_str();

  if ( _FindFirstFileOld )
    return _FindFirstFileOld(pszFile, lpFindFileData);
  return FindFirstFile(pszFile, lpFindFileData);
}
void setReplayName(char *pOutFilename, const char *pInFileName)
{
  if ( !BWAPI::BroodwarImpl.outOfGame && strstr(pInFileName, "LastReplay.rep") )
  {
    if ( gszDesiredReplayName[0] )
      strcpy(pOutFilename, gszDesiredReplayName);
    else
      strcpy(pOutFilename, pInFileName);

    if ( gdwProcNum )
    {
      char tmp[16];
      sprintf(tmp, " (%u).rep", gdwProcNum);
      char *ext = strrchr(pOutFilename, '.');
      strcpy_s(ext, MAX_PATH - (ext ? ext - pOutFilename : 0), tmp);
    }
  }
  else
    strcpy(pOutFilename, pInFileName);
}
BOOL WINAPI _DeleteFile(LPCSTR lpFileName)
{
  char szNewFileName[MAX_PATH];
  setReplayName(szNewFileName, lpFileName);
  if ( _DeleteFileOld )
    return _DeleteFileOld(szNewFileName);
  return DeleteFile(szNewFileName);
}
DWORD WINAPI _GetFileAttributes(LPCSTR lpFileName)
{
  char szNewFileName[MAX_PATH];
  setReplayName(szNewFileName, lpFileName);
  if ( _GetFileAttributesOld )
    return _GetFileAttributesOld(szNewFileName);
  return GetFileAttributes(szNewFileName);
}
HANDLE WINAPI _CreateFile(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
  char szNewFileName[MAX_PATH];
  setReplayName(szNewFileName, lpFileName);
  if ( _CreateFileOld )
    return _CreateFileOld(szNewFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
  return CreateFile(szNewFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}
//--------------------------------------------- CAPTURE SCREEN -----------------------------------------------
BOOL STORMAPI _SDrawCaptureScreen(const char *pszOutput)
{
  char *ext = strrchr((char*)pszOutput, '.');
  if ( ext && strlen(gszScreenshotFormat) == 3 )
  {
    ++ext;
    strncpy(ext, gszScreenshotFormat, 3);
  }
  if ( wmode && pBits )
  {
    // Create compatible palette
    PALETTEENTRY pal[256];
    for ( int i = 0; i < 256; ++i )
    {
      pal[i].peRed    = palette[i].rgbRed;
      pal[i].peGreen  = palette[i].rgbGreen;
      pal[i].peBlue   = palette[i].rgbBlue;
      pal[i].peFlags  = 0;
    }
    return SBmpSaveImage(pszOutput, pal, pBits, BW::BWDATA_GameScreenBuffer->wid, BW::BWDATA_GameScreenBuffer->ht);
  }
  if ( _SDrawCaptureScreenOld )
    return _SDrawCaptureScreenOld(pszOutput);
  return SDrawCaptureScreen(pszOutput);
}

//----------------------------------------------- ON GAME END ------------------------------------------------
BOOL __stdcall _SNetLeaveGame(int type)
{
  BWAPI::BroodwarImpl.onGameEnd();
  if ( _SNetLeaveGameOld )
    return _SNetLeaveGameOld(type);
  return SNetLeaveGame(type);
}

//--------------------------------------------- NEXT FRAME HOOK ----------------------------------------------
// Broodwar isGamePaused()
int __cdecl _nextFrameHook()
{
  BWAPI::BroodwarImpl.update();
  return *BW::BWDATA_isGamePaused;
}

//------------------------------------------------- SEND TEXT ------------------------------------------------
int __stdcall _SStrCopy(char *dest, const char *source, size_t size)
{
  if ( strlen(source) > 0 )
  {
    if ( size == 0x7FFFFFFF && *BW::BWDATA_gwGameMode == 3 )
    {
      if ( dest == BW::BWDATA_SaveGameFile )
      {
        /* onSaveGame */
        BWAPI::BroodwarImpl.onSaveGame((char*)source);
      }
      else
      {
        /* onSend Game */
        BWAPI::BroodwarImpl.sentMessages.push_back(std::string(source));
        dest[0] = 0;
        return 0;
      }
    }
    else if ( size == 120 && *BW::BWDATA_gwGameMode != 3 )
    {
      /* onSend Lobby */
    }
  }
  if ( _SStrCopyOld )
    return _SStrCopyOld(dest, source, size);
  return SStrCopy(dest, source, size);
}

//----------------------------------------------- RECEIVE TEXT -----------------------------------------------
BOOL __stdcall _SNetReceiveMessage(int *senderplayerid, u8 **data, int *databytes)
{
  BOOL rval;
  if ( _SNetReceiveMessageOld )
    rval = _SNetReceiveMessageOld(senderplayerid, data, databytes);
  else
    rval = SNetReceiveMessage(senderplayerid, (char**)data, databytes);

  if ( rval && *databytes > 2 && (*data)[0] == 0)
    BWAPI::BroodwarImpl.onReceiveText(*senderplayerid, std::string((char*)&(*data)[2]) );

  return rval;
}

//----------------------------------------------- DRAW HOOK --------------------------------------------------
bool wantRefresh = false;
DWORD dwLastAPMCount;
double botAPM_noSelect;
double botAPM_select;
void __stdcall DrawHook(BW::bitmap *pSurface, BW::bounds *pBounds)
{
  if ( wantRefresh )
  {
    wantRefresh = false;
    memset(BW::BWDATA_RefreshRegions, 1, 1200);
  }

  //GameUpdate(pSurface, pBounds);
  if ( BW::pOldDrawGameProc )
    BW::pOldDrawGameProc(pSurface, pBounds);

  if ( BW::BWDATA_GameScreenBuffer->data )
  {
    if ( gdwHoliday )
      DrawHoliday();

    if ( !BWAPI::BroodwarImpl.isPaused() )
    {
      DWORD dwThisTickCount = BWAPI::BroodwarImpl.getFrameCount()*42;
      if ( dwThisTickCount > dwLastAPMCount )
      {
        double timeDiff = dwThisTickCount - dwLastAPMCount;
        dwLastAPMCount = dwThisTickCount;
        botAPM_noSelect *= exp((double)(-timeDiff/57000)); // 0.95 * 60000
        botAPM_select *= exp((double)(-timeDiff/57000)); // 0.95 * 60000
      }
    }

    unsigned int numShapes = BWAPI::BroodwarImpl.drawShapes();
    
    if ( numShapes )
      wantRefresh = true;
  }
}
//------------------------------------------------- MENU HOOK ------------------------------------------------
bool nosound = false;
void __stdcall DrawDialogHook(BW::bitmap *pSurface, BW::bounds *pBounds)
{
  if ( BW::pOldDrawDialogProc && !hideHUD )
    BW::pOldDrawDialogProc(pSurface, pBounds);

  if ( *BW::BWDATA_gwGameMode == 4 )
    BWAPI::BroodwarImpl.onMenuFrame();

  // NOSOUND config option
  if ( !nosound )
  {
    nosound = true;
    char szNosound[8];
    GetPrivateProfileString("starcraft", "sound", "ON", szNosound, 8, szConfigPath);
    if ( strcmpi(szNosound, "OFF") == 0 )
      BW::BWDATA_DSoundDestroy();
  }

  // WMODE config option
  if ( switchToWMode && ghMainWnd )
  {
    switchToWMode = false;
    SetWMode(windowRect.right, windowRect.bottom, true);
  }

  //click the menu dialog that pops up when you win/lose a game
  BW::dialog *endDialog = BW::FindDialogGlobal("LMission");
  if ( !endDialog )
    endDialog = BW::FindDialogGlobal("WMission");
  if ( endDialog )
    endDialog->findIndex(-2)->activate();
}

//------------------------------------------- AUTH ARCHIVE HOOK ----------------------------------------------
BOOL __stdcall _SFileAuthenticateArchive(HANDLE hArchive, DWORD *dwReturnVal)
{
  /* Always return a successful check to bypass our custom SNP module authentication */
  if ( dwReturnVal )
    *dwReturnVal = 5;
  return TRUE;
}

//--------------------------------------------- OPEN FILE HOOK -----------------------------------------------
std::string lastFile;
BOOL __stdcall _SFileOpenFileEx(HANDLE hMpq, const char *szFileName, DWORD dwSearchScope, HANDLE *phFile)
{
  /* Store the name of the last-opened file to retrieve the pointer once it's allocated */
  lastFile = szFileName;

  if ( !phFile )
    return FALSE;

  if ( !SFileOpenFileEx(NULL, szFileName, SFILE_FROM_ABSOLUTE | SFILE_FROM_RELATIVE, phFile) || !(*phFile) )
  {
    if ( _SFileOpenFileExOld )
      return _SFileOpenFileExOld(hMpq, szFileName, dwSearchScope, phFile);
    return SFileOpenFileEx(hMpq, szFileName, dwSearchScope, phFile);
  }
  return TRUE;
}

BOOL __stdcall _SFileOpenFile(const char *filename, HANDLE *phFile)
{
  lastFile = filename;
  if ( !phFile )
    return FALSE;

  if ( !SFileOpenFileEx(NULL, filename, SFILE_FROM_ABSOLUTE | SFILE_FROM_RELATIVE, phFile) || !(*phFile) )
  {
    if ( _SFileOpenFileOld )
      return _SFileOpenFileOld(filename, phFile);
    return SFileOpenFile(filename, phFile);
  }
  return TRUE;
}

//--------------------------------------------- MEM ALLOC HOOK -----------------------------------------------
void *__stdcall _SMemAlloc(int amount, char *logfilename, int logline, char defaultValue)
{
  /* Call the original function */
  void *rval = NULL;
  if ( _SMemAllocOld )
    rval = _SMemAllocOld(amount, logfilename, logline, defaultValue);
  else
    rval = SMemAlloc(amount, logfilename, logline, defaultValue);

  if ( lastFile == "dlgs\\protoss.grp" || 
       lastFile == "dlgs\\terran.grp"  ||
       lastFile == "dlgs\\zerg.grp" )
  {
    if ( strcmpi(logfilename, ".?AU_DLGGRP@@") == 0 )
    {
      if ( leakUIClassLoc )
        SMFree(leakUIClassLoc);
      leakUIClassLoc = rval;
      BW::BWDATA_customList_UIDlgData[0] = BW::BWDATA_customList_UIDlgData;  // list with custom allocator?
      BW::BWDATA_customList_UIDlgData[1] = (void*)~(u32)BW::BWDATA_customList_UIDlgData;
    }
    else if ( strcmpi(logfilename, "Starcraft\\SWAR\\lang\\game.cpp") == 0 )
    {
      if ( leakUIGrpLoc )
        SMFree(leakUIGrpLoc);
      leakUIGrpLoc = rval;
      BW::BWDATA_customList_UIDlgData[0] = BW::BWDATA_customList_UIDlgData;  // list with custom allocator?
      BW::BWDATA_customList_UIDlgData[1] = (void*)~(u32)BW::BWDATA_customList_UIDlgData;
    }
  }

  /* Save the allocated string table pointer */
  if ( lastFile == "rez\\stat_txt.tbl" )
  {
    BW::BWDATA_StringTableOff = (char*)rval;
    lastFile = "";
    //MessageBox(0, "BWDATA_StringTableOff", "", 0);
  }

  /* Save the allocated fog of war pointer */
  if ( amount == 0x40000 && strcmpi(logfilename, "Starcraft\\SWAR\\lang\\Gamemap.cpp") == 0 && logline == 606 )
  {
    BW::BWDATA_ActiveTileArray = (BW::activeTile*)rval;
    //MessageBox(0, "BWDATA_ActiveTileArray", "", 0);
  }

  /* Save the allocated mini-tile flags pointer */
  if ( lastFile.find(".vf4") != std::string::npos )
  {
    BW::BWDATA_MiniTileFlags = (BW::MiniTileMaps_type*)rval;
    lastFile = "";
    //MessageBox(0, "BWDATA_MiniTileFlags", "", 0);
  }

  /* Save the allocated SAI_Paths pointer */
  if ( strcmpi(logfilename, "Starcraft\\SWAR\\lang\\sai_PathCreate.cpp") == 0 && logline == 210 )
  {
    BW::BWDATA_SAIPathing = (BW::SAI_Paths*)rval;
    //MessageBox(0, "BWDATA_SAIPathing", "", 0);
  }

  /* Save the allocated tileset pointer */
  if ( lastFile.find(".cv5") != std::string::npos )
  {
    BW::BWDATA_TileSet    = (BW::TileType*)rval;
    lastFile = "";
    //MessageBox(0, "BWDATA_TileSet", "", 0);
  }

  /* Save the allocated map tile array pointer */
  if ( amount == 0x20000 && strcmpi(logfilename, "Starcraft\\SWAR\\lang\\Gamemap.cpp") == 0 && logline == 603 )
  {
    BW::BWDATA_MapTileArray = (u16*)rval;
    //MessageBox(0, "BWDATA_MapTileArray", "", 0);
  }

  return rval;
}

//--------------------------------------------- SEND TURN HOOK -----------------------------------------------
DWORD lastTurnTime;
DWORD lastTurnFrame;
BOOL __stdcall _SNetSendTurn(char *data, unsigned int databytes)
{
  /* Save tick/frame counts for getRemainingLatency*  */
  lastTurnTime  = GetTickCount();
  lastTurnFrame = BWAPI::BroodwarImpl.getFrameCount();
  if ( _SNetSendTurnOld )
    return _SNetSendTurnOld(data, databytes);
  return SNetSendTurn(data, databytes);
}

//---------------------------------------- USER ISSUE COMMAND HOOK -------------------------------------------
int   lastHotkey;
DWORD lastHotkeyTime;
void __fastcall CommandFilter(BYTE *buffer, DWORD length)
{
  /* Filter commands using BWAPI rules */
  if ( BWAPI::BroodwarImpl.isFlagEnabled(BWAPI::Flag::UserInput) || 
       !BWAPI::BroodwarImpl.onStartCalled ||
       buffer[0] <= 0x0B ||
       (buffer[0] >= 0x0F && buffer[0] <= 0x12) ||
       (length >= 3 && buffer[0] == 0x13 && buffer[1] == 1)    || // Hotkey (select only)
       (buffer[0] >= 0x37 && buffer[0] <= 0x59) ||
       buffer[0] >= 0x5B )
  {
    // Custom select code
    if ( buffer[0] == 0x09 ||
         buffer[0] == 0x0A ||
         buffer[0] == 0x0B ||
         (length >= 3 && buffer[0] == 0x13 && buffer[1] == 1) ) // Select Units
    {
      // Do our own center view on hotkeys, since BWAPI introduces a bug that destroys this
      if ( length >= 3 && buffer[0] == 0x13 && buffer[1] == 1 ) // Recall Hotkey
      {
        DWORD thisHotkeyTime = GetTickCount();
        if ( lastHotkey == buffer[2] && (thisHotkeyTime - lastHotkeyTime) < 800 )
        {
          // do center view here
          BWAPI::BroodwarImpl.moveToSelected();
          lastHotkeyTime = 0;
          lastHotkey     = -1;
        }
        else
        {
          lastHotkeyTime = thisHotkeyTime;
          lastHotkey     = buffer[2];
        }
      }
      BWAPI::BroodwarImpl.wantSelectionUpdate = true;
      return;
    } // selections

    if ( buffer[0] == 0x0C ||
         (buffer[0] == 0x13 && !(buffer[1] & 1)) ||
         buffer[0] == 0x14 ||
         buffer[0] == 0x15 ||
         (buffer[0] >= 0x18 && buffer[0] <= 0x36) ||
         buffer[0] == 0x5A )
    {
      //reload the unit selection states (so that the user doesn't notice any changes in selected units in the Starcraft GUI.
      BW::Orders::Select sel = BW::Orders::Select(*BW::BWDATA_ClientSelectionCount, BW::BWDATA_ClientSelectionGroup);
      QueueGameCommand(&sel, sel.size);
    } // user select
    QueueGameCommand(buffer, length);
  }
}
