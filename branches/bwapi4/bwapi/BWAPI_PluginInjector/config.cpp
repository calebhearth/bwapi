#include <Windows.h>
#include <string>

#include "config.h"
#include "common.h"

bool RunConfig()
{
  std::string sInstallPath = GetRegString("SOFTWARE\\Blizzard Entertainment\\Starcraft", "InstallPath");

  // Load the config file
  if ( ShellExecute(NULL, "open", (sInstallPath + "\\bwapi-data\\bwapi.ini").c_str(), NULL, NULL, SW_SHOWNORMAL) <= (HINSTANCE)32 )
    return BWAPIError("Unable to open BWAPI config file.");

  return true;
}

