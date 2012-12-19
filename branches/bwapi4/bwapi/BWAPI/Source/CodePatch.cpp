#include "CodePatch.h"
#include "NewHackUtil.h"

#include "BW/Offsets.h"
#include "BW/MenuPosition.h"
#include "BWAPI/GameImpl.h"
#include "Detours.h"
#include "Resolution.h"
#include "WMode.h"
#include "Assembly.h"

#include "Config.h"

#include "../../starcraftver.h"
#include "../../Debug.h"
#include <windows.h>

void ApplyCodePatches()
{
  // ---------------------------------- VERSION DEPENDENT ----------------------------------------------------
  // Only apply if version is correct
  if ( isCorrectVersion )
  {
    // Create function-level hooks
    HackUtil::CallPatch(BW::BWFXN_P_IsGamePaused, &_nextFrameHook);
    HackUtil::CallPatch(BW::BWDATA::ExecuteGameTrigsCallPatch, &ExecuteGameTriggers);
    HackUtil::WriteNops(BW::BWFXN_SpendRepair, 7);
    HackUtil::JmpPatch(BW::BWFXN_SpendRepair, &_repairHook);
    HackUtil::JmpPatch(BW::BWFXN_RefundMinerals, &_refundMineralsHook);
    HackUtil::JmpPatch(BW::BWFXN_RefundGas, &_refundGasHook);
    HackUtil::JmpPatch(BW::BWFXN_RefundMinAndGas2, &_refundMinAndGas2Hook);
    HackUtil::JmpPatch(BW::BWFXN_RefundMin3, &_refundMin3Hook);
    HackUtil::JmpPatch(BW::BWFXN_RefundGas3, &_refundGas3Hook);
    HackUtil::JmpPatch(BW::BWFXN_RefundMin4, &_refundMin4Hook);
    HackUtil::JmpPatch(BW::BWFXN_RefundGas4, &_refundGas4Hook);
    HackUtil::JmpPatch(BW::BWFXN_RefundMinAndGas5, &_refundMinAndGas5Hook);
    HackUtil::JmpPatch(BW::BWFXN_QueueCommand, &CommandFilter);
    HackUtil::JmpPatch(BW::BWFXN_DDrawDestroy, &DDrawDestroy);
    HackUtil::JmpPatch(BW::BWFXN_NetSelectReturnMenu, &_SelectReturnMenu);
    HackUtil::CallPatch(BW::BWDATA::RandomizeRacePatch, &_RandomizePlayerRaces);
    HackUtil::CallPatch(BW::BWDATA::InitPlayerConsolePatch, &_InitializePlayerConsole);
    
    // Perform code patches
    char zero = 0;
    HackUtil::WriteMem(BW::BWDATA::ServerMenuIn, &zero, 1);        // BNET Server menu in speed
    HackUtil::WriteMem(BW::BWDATA::ServerMenuOut, &zero, 1);       // BNET Server menu out speed
    HackUtil::WriteMem(BW::BWDATA::OpponentStartHack, &zero, 1);   // Start without an opponent
    HackUtil::WriteNops(BW::BWDATA::SingleSpeedHack, 11);          // Use multiplayer speed setting instead
                                                                  // of always setting speed to "Normal" in
                                                                  // single player
    // Write menu animation speed hack
    for ( int i = 0; i < 43; ++i )
      BW::BWDATA::commonSwishControllers[i].wType = 4;
    for ( int i = 0; i < 5; ++i )
    {
      BW::BWDATA::gluCustmSwishController[i].wType = 4;
      BW::BWDATA::gluChatSwishController[i].wType  = 4;
    }
    BW::BWDATA::gluCmpgnSwishController[0].wType = 4;
    BW::BWDATA::gluCmpgnSwishController[1].wType = 4;
    BW::BWDATA::gluScoreSwishController[0].wType = 4;

    // Write trigger action detours
    memcpy(BWTriggerActionFxnTable, BW::TriggerActionCallbacks, sizeof(BWTriggerActionFxnTable));
    for ( int i = 0; i < countof(BWTriggerActionFxnTable); ++i )
      BW::TriggerActionCallbacks[i] = &TriggerActionReplacement;
  }
  // ---------------------------------- VERSION INDEPENDENT --------------------------------------------------
  // Write storm authentication patch (allow custom network modes)
  HackUtil::JmpPatch(HackUtil::GetImport("storm.dll", 251), &_SFileAuthenticateArchive);

  // Storm detours
  _SNetLeaveGameOld       = HackUtil::PatchImport("storm.dll", 119, &_SNetLeaveGame);
  _SNetReceiveMessageOld  = HackUtil::PatchImport("storm.dll", 121, &_SNetReceiveMessage);
  _SNetSendTurnOld        = HackUtil::PatchImport("storm.dll", 128, &_SNetSendTurn);
  _SFileOpenFileOld       = HackUtil::PatchImport("storm.dll", 267, &_SFileOpenFile);
  _SFileOpenFileExOld     = HackUtil::PatchImport("storm.dll", 268, &_SFileOpenFileEx);
  _SDrawCaptureScreenOld  = HackUtil::PatchImport("storm.dll", 342, &_SDrawCaptureScreen);
  _SMemAllocOld           = HackUtil::PatchImport("storm.dll", 401, &_SMemAlloc);
  _SStrCopyOld            = HackUtil::PatchImport("storm.dll", 501, &_SStrCopy);

  // wmode/drawing detours
  _GetCursorPosOld        = HackUtil::PatchImport("user32.dll", "GetCursorPos", &_GetCursorPos);
  _SetCursorPosOld        = HackUtil::PatchImport("user32.dll", "SetCursorPos", &_SetCursorPos);
  _ClipCursorOld          = HackUtil::PatchImport("user32.dll", "ClipCursor", &_ClipCursor);
  _SDrawLockSurfaceOld    = HackUtil::PatchImport("storm.dll", 350, &_SDrawLockSurface);
  _SDrawRealizePaletteOld = HackUtil::PatchImport("storm.dll", 354, &_SDrawRealizePalette);
  _SDrawUnlockSurfaceOld  = HackUtil::PatchImport("storm.dll", 356, &_SDrawUnlockSurface);
  _SDrawUpdatePaletteOld  = HackUtil::PatchImport("storm.dll", 357, &_SDrawUpdatePalette);
  _CreateWindowExOld      = HackUtil::PatchImport("user32.dll", "CreateWindowExA", &_CreateWindowEx);
  
  // Other Detours
  _DeleteFileOld        = HackUtil::PatchImport("kernel32.dll", "DeleteFileA", &_DeleteFile);
  _GetFileAttributesOld = HackUtil::PatchImport("kernel32.dll", "GetFileAttributesA", &_GetFileAttributes);
  _CreateFileOld        = HackUtil::PatchImport("kernel32.dll", "CreateFileA", &_CreateFile);
  _FindFirstFileOld     = HackUtil::PatchImport("kernel32.dll", "FindFirstFileA", &_FindFirstFile);
  _SleepOld             = HackUtil::PatchImport("kernel32.dll", "Sleep", &_Sleep);
  _CreateThreadOld      = HackUtil::PatchImport("kernel32.dll", "CreateThread", &_CreateThread);
}

//----------------------------------------- NET-MODE RETURN MENU ---------------------------------------------
void _SelectReturnMenu()
{
  switch ( *BW::BWDATA::NetMode )
  {
  case 'BNET':
    *BW::BWDATA::glGluesMode = BW::GLUE_BATTLE;  // battle.net
    break;
  case 'IPXN':
  case 'ATLK':
  case 'IPXX':
  case 'UDPN':
  case 'LUDP':
  case 'LPIP':
  case 'DRIP':
  case 'SMEM':
    *BW::BWDATA::glGluesMode = BW::GLUE_GAME_SELECT; // game select
    break;
  case 'MDMX':
  case 'MODM':
    *BW::BWDATA::glGluesMode = BW::GLUE_MODEM; // modem
    break;
  case 'SCBL':
    *BW::BWDATA::glGluesMode = BW::GLUE_DIRECT; // direct connect
    break;
  default:
    *BW::BWDATA::glGluesMode = BW::GLUE_MAIN_MENU;  // main menu
    break;
  }
}