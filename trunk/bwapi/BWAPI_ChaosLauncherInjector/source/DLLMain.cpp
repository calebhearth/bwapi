// DllTemplate.cpp : Defines the entry point for the DLL application.
//

#include <windows.h>
#include <string>
#include <assert.h>

#define BWLAPI 4
#define STARCRAFTBUILD 13

/*  STARCRAFTBUILD
   -1   All
   0   1.04
   1   1.08b
   2   1.09b
   3   1.10
   4   1.11b
   5   1.12b
   6   1.13f
   7   1.14
   8   1.15
   9   1.15.1
   10  1.15.2
   11  1.15.3
   12  1.16.0
   13  1.16.1
*/

#ifdef _MANAGED
#pragma managed(push, off)
#endif

struct ExchangeData
{
   int iPluginAPI;
   int iStarCraftBuild;
   bool bConfigDialog;                 //Is Configurable
   bool bNotSCBWmodule;                //Inform user that closing BWL will shut down your plugin
};
#include <stdio.h>
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                )
{
   switch (ul_reason_for_call)
   {
      case DLL_PROCESS_ATTACH:
      case DLL_THREAD_ATTACH:
      case DLL_THREAD_DETACH:
      case DLL_PROCESS_DETACH:
         break;
   }
   return TRUE;
}

//
// GET Functions for BWLauncher
//
//
extern "C" __declspec(dllexport) void GetPluginAPI(ExchangeData &Data)
{
   //BWL Gets version from Resource - VersionInfo
   Data.iPluginAPI = BWLAPI;
   Data.iStarCraftBuild = STARCRAFTBUILD;
   Data.bConfigDialog = false;
   Data.bNotSCBWmodule = true;
}

extern "C" __declspec(dllexport) void GetData(char *name, char *description, char *updateurl)
{
   //if necessary you can add Initialize function here
   //possibly check CurrentCulture (CultureInfo) to localize your DLL due to system settings
   strcpy(name, "BWAPI Injector (1.16.1)");
   strcpy(description, "Injects BWAPI.dll into the Broodwar process.   Be sure to get the latest revision!      - BWAI Project Team");
   strcpy(updateurl, "http://bwapi.googlecode.com/files/");
}


//
// Functions called by BWLauncher
//
//
extern "C" __declspec(dllexport) bool OpenConfig()
{
   return true; //everything OK

   //return false; //something went wrong
}

extern "C" __declspec(dllexport) bool ApplyPatchSuspended(HANDLE hProcess, DWORD dwProcessID)
{
   return true; //everything OK

   //return false; //something went wrong
}

#pragma warning(push)
#pragma warning(disable:4189)
extern "C" __declspec(dllexport) bool ApplyPatch(HANDLE hProcess, DWORD dwProcessID)
{
  std::string dllFileName = "BWAPI.dll";

  LPTHREAD_START_ROUTINE loadLibAddress = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("Kernel32"), "LoadLibraryA" );
  assert(NULL != loadLibAddress);

  void* pathAddress = VirtualAllocEx(hProcess, NULL, dllFileName.size()+1, MEM_COMMIT, PAGE_READWRITE);
  assert(NULL != pathAddress);

  SIZE_T bytesWritten;

  BOOL success = WriteProcessMemory(hProcess, pathAddress, dllFileName.c_str(), dllFileName.size()+1, &bytesWritten);
  assert(success && bytesWritten == dllFileName.size()+1);

  HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, loadLibAddress, pathAddress, 0, NULL);
  assert(NULL != hThread);

  WaitForSingleObject(hThread, INFINITE);

  DWORD hLibModule; // Base address of the loaded module
  GetExitCodeThread(hThread, &hLibModule);
  assert(0 != hLibModule);

  VirtualFreeEx(hProcess, pathAddress, dllFileName.size()+1, MEM_RELEASE);
  CloseHandle(hThread);

  return true; //everything OK
}
#pragma warning(pop)

#ifdef _MANAGED
#pragma managed(pop)
#endif 