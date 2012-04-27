#include <Windows.h>
#include <stdio.h>

#include "chaoslauncher.h"
#include "common.h"

#include "config.h"

// GET Functions for BWLauncher
//
extern "C" __declspec(dllexport) void GetPluginAPI(ExchangeData& Data)
{
  //BWL Gets version from Resource - VersionInfo
  Data.iPluginAPI      = 4;
  Data.iStarCraftBuild = -1;
  Data.bConfigDialog   = TRUE;
  Data.bNotSCBWmodule  = FALSE;
}

extern "C" __declspec(dllexport) void GetData(char* name, char* description, char* updateurl)
{
  char newDescription[512];
  sprintf_s(newDescription, 512, "Injects " MODULE " into the Broodwar process.\r\n\r\nRevision %s.\r\nCheck for updates at http://bwapi.googlecode.com/ \r\n\r\nCreated by the BWAPI Project Team", SVN_REV_STR);
  strcpy(name, "BWAPI Injector (" STARCRAFT_VER ") " BUILD_STR);
  strcpy(description, newDescription);
  strcpy(updateurl, "http://bwapi.googlecode.com/files/");
}

// Functions called by BWLauncher
//
extern "C" __declspec(dllexport) bool OpenConfig()
{
  return RunConfig();
}

extern "C" __declspec(dllexport) bool ApplyPatch(HANDLE, DWORD)
{
  return true;
}

extern "C" __declspec(dllexport) bool ApplyPatchSuspended(HANDLE hProcess, DWORD)
{
  char envBuffer[MAX_PATH];
  bool envFailed = false;
  if ( !GetEnvironmentVariable("ChaosDir", envBuffer, MAX_PATH) )
  {
    envFailed = true;
    if ( !GetCurrentDirectory(MAX_PATH, envBuffer) )
      return BWAPIError("Could not find ChaosDir or CurrentDirectory.");
  }

  strcat(envBuffer, "\\" MODULE);
  DWORD dwFileAttribs = GetFileAttributes(envBuffer);
  if ( dwFileAttribs == INVALID_FILE_ATTRIBUTES || dwFileAttribs & FILE_ATTRIBUTE_DIRECTORY )
  {
    if ( !envFailed && !GetCurrentDirectory(MAX_PATH, envBuffer) )
      return BWAPIError("Could not find CurrentDirectory.");
    strcat(envBuffer, "\\" MODULE);
    dwFileAttribs = GetFileAttributes(envBuffer);
    if ( dwFileAttribs == INVALID_FILE_ATTRIBUTES || dwFileAttribs & FILE_ATTRIBUTE_DIRECTORY )
      return BWAPIError("Could not find file \"%s\".", envBuffer);
  }
  DWORD dwDllSize = strlen(envBuffer)+1;

  LPTHREAD_START_ROUTINE loadLibAddress = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("Kernel32"), "LoadLibraryA" );
  if ( !loadLibAddress )
    return BWAPIError("Could not get Proc Address for LoadLibraryA.");


  void* pathAddress = VirtualAllocEx(hProcess, NULL, dwDllSize, MEM_COMMIT, PAGE_READWRITE);
  if ( !pathAddress )
    return BWAPIError("Could not allocate memory for DLL path.");

  SIZE_T bytesWritten;
  BOOL success = WriteProcessMemory(hProcess, pathAddress, envBuffer, dwDllSize, &bytesWritten);
  if ( !success )
  {
    VirtualFreeEx(hProcess, pathAddress, dwDllSize, MEM_RELEASE);
    return BWAPIError("Unable to write process memory.");
  }
  if ( bytesWritten != dwDllSize )
    BWAPIError("WriteToProcessMemory bytesWritten is not the expected value.");

  HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, loadLibAddress, pathAddress, 0, NULL);
  if ( !hThread )
  {
    VirtualFreeEx(hProcess, pathAddress, dwDllSize, MEM_RELEASE);
    return BWAPIError("Unable to create remote thread.");
  }

  if ( WaitForSingleObject(hThread, INFINITE) == WAIT_FAILED )
  {
    VirtualFreeEx(hProcess, pathAddress, dwDllSize, MEM_RELEASE);
    CloseHandle(hThread);
    return BWAPIError("WaitForSingleObject failed.");
  }

  DWORD dwExitCode = NULL;
  if ( !GetExitCodeThread(hThread, &dwExitCode) )
  {
    VirtualFreeEx(hProcess, pathAddress, dwDllSize, MEM_RELEASE);
    CloseHandle(hThread);
    return BWAPIError("GetExitCodeThread failed.");
  }

  if ( !dwExitCode )
    BWAPIError("Injection failed.\nThis is caused when BWAPI crashes before injecting completely.");

  VirtualFreeEx(hProcess, pathAddress, dwDllSize, MEM_RELEASE);
  CloseHandle(hThread);
  return true; //everything OK
}
