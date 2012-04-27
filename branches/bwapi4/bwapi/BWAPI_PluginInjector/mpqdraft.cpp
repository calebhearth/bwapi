#include <Windows.h>
#include "mpqdraft.h"
#include "common.h"

#include "config.h"

#define THEPLUGINID 0x10001000


BOOL WINAPI MPQDraftPluginInterface::Identify(LPDWORD pluginID)
{
  if (!pluginID)
    return FALSE;

  *pluginID = THEPLUGINID;
  return TRUE;
}
  
BOOL WINAPI MPQDraftPluginInterface::GetPluginName(LPSTR pPluginName, DWORD namebufferlength)
{
  // safeguard
  if ( !pPluginName )
    return FALSE;

  // Copy the plugin name to the target
  strncpy(pPluginName, pszPluginName, namebufferlength-1);
  pPluginName[namebufferlength-1] = '\0';
  return TRUE;
}

BOOL WINAPI MPQDraftPluginInterface::CanPatchExecutable(LPCSTR exefilename)
{  //Generally you check if the version is right here
  return TRUE;
}
  
BOOL WINAPI MPQDraftPluginInterface::Configure(HWND parentwindow)
{
  return RunConfig() ? TRUE : FALSE;
}

BOOL WINAPI MPQDraftPluginInterface::ReadyForPatch()
{  //Right before it opens up SC, checks for panic?
  return TRUE;
}

BOOL WINAPI MPQDraftPluginInterface::GetModules(MPQDRAFTPLUGINMODULE* pluginmodules, LPDWORD nummodules)
{  //Weird stuff, i doubt you'll use it
  if (!nummodules)
    return FALSE;
  *nummodules = 0;
  return TRUE;
}

BOOL WINAPI MPQDraftPluginInterface::InitializePlugin(IMPQDraftServer* server)
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

  if ( !LoadLibrary(envBuffer) )
    return BWAPIError("Could not load \"%s\".", envBuffer);
  return TRUE;
}

BOOL WINAPI MPQDraftPluginInterface::TerminatePlugin()
{  //Called when starcraft closes
  return TRUE;
}

// Define the interface and exported function
MPQDraftPluginInterface pluginInterface;

BOOL WINAPI GetMPQDraftPlugin(IMPQDraftPlugin** lppMPQDraftPlugin)
{
  *lppMPQDraftPlugin = &pluginInterface;
  return TRUE;
}

