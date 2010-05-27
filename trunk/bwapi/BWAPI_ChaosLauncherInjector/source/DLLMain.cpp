#include <stdio.h>
#include <windows.h>
#include <string>
#include <assert.h>
#include "../../svnrev.h"

#define BWLAPI 4
#define STARCRAFTBUILD 13
#define ENV_BUFFER_SIZE 512

struct ExchangeData
{
  int iPluginAPI;
  int iStarCraftBuild;
  BOOL bNotSCBWmodule;                //Inform user that closing BWL will shut down your plugin
  BOOL bConfigDialog;                 //Is Configurable
};

void BWAPIError(const char *format, ...)
{
  char buffer[MAX_PATH];
  va_list ap;
  va_start(ap, format);
  vsnprintf_s(buffer, MAX_PATH, MAX_PATH, format, ap);
  va_end(ap);

  FILE* f = fopen("bwapi-error.txt", "a+");
  fprintf(f, "%s\n", buffer);
  fclose(f);
  MessageBoxA(NULL, buffer, "Error", MB_OK);
}

BOOL APIENTRY DllMain(HMODULE, DWORD, LPVOID)
{
  return TRUE;
}

// GET Functions for BWLauncher
//
extern "C" __declspec(dllexport) void GetPluginAPI(ExchangeData& Data)
{
  //BWL Gets version from Resource - VersionInfo
  Data.iPluginAPI = BWLAPI;
  Data.iStarCraftBuild = STARCRAFTBUILD;
  Data.bConfigDialog = TRUE;
  Data.bNotSCBWmodule = FALSE;
}

extern "C" __declspec(dllexport) void GetData(char* name, char* description, char* updateurl)
{
  char newDescription[512];
  sprintf_s(newDescription, 512, "Injects BWAPI.dll into the Broodwar process.\r\n\r\nRevision %s.\r\nCheck for updates at http://bwapi.googlecode.com/ \r\n\r\nCreated by the BWAPI Project Team", SVN_REV_STR);
  
  strcpy(name, "BWAPI Injector (1.16.1)");
  strcpy(description, newDescription);
  strcpy(updateurl, "http://bwapi.googlecode.com/files/");
}

// Functions called by BWLauncher
//
extern "C" __declspec(dllexport) bool OpenConfig()
{
  if (!ShellExecuteA(NULL, "open", "..\\bwapi-data\\bwapi.ini", NULL, NULL, SW_SHOWNORMAL))
    return false;
  return true;
}

extern "C" __declspec(dllexport) bool ApplyPatchSuspended(HANDLE, DWORD)
{
  return true;
}

extern "C" __declspec(dllexport) bool ApplyPatch(HANDLE hProcess, DWORD)
{
  char envBuffer[ENV_BUFFER_SIZE];
  if ( !GetEnvironmentVariableA("ChaosDir", envBuffer, ENV_BUFFER_SIZE) )
    if ( !GetCurrentDirectoryA(ENV_BUFFER_SIZE, envBuffer) )
      BWAPIError("Could not find ChaosDir or current directory.");

  std::string dllFileName(envBuffer);
  dllFileName.append("\\BWAPI.dll");

  LPTHREAD_START_ROUTINE loadLibAddress = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("Kernel32"), "LoadLibraryA" );
  if ( !loadLibAddress )
    BWAPIError("Could not get Proc Address for LoadLibraryA.");

  void* pathAddress = VirtualAllocEx(hProcess, NULL, dllFileName.size() + 1, MEM_COMMIT, PAGE_READWRITE);
  if ( !pathAddress )
    BWAPIError("Could not allocate memory for DLL path.");

  SIZE_T bytesWritten;
  BOOL success = WriteProcessMemory(hProcess, pathAddress, dllFileName.c_str(), dllFileName.size() + 1, &bytesWritten);
  if (success == FALSE || bytesWritten != dllFileName.size() + 1)
    BWAPIError("Unable to write process memory.");

  HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, loadLibAddress, pathAddress, 0, NULL);
  if ( !hThread )
    BWAPIError("Unable to create remote thread.");

  WaitForSingleObject(hThread, INFINITE);

  DWORD hLibModule = NULL; // Base address of the loaded module
  GetExitCodeThread(hThread, &hLibModule);
  if ( !hLibModule )
    BWAPIError("Could not get hLibModule.");

  VirtualFreeEx(hProcess, pathAddress, dllFileName.size() + 1, MEM_RELEASE);
  CloseHandle(hThread);
  return true; //everything OK
}
