#include <windows.h>
#include <BWAPI.h>

#include <list>
#include <deque>
#include "Location.h"

using namespace BWAPI;

void UpdateScripts();
bool LoadAIBinary(const char *pszFileName);
int GetStandardUnitCount(BWAPI::UnitType type, bool bCompleted = true, Player *player = NULL);

int GetTerranRushScore(Player *player);
int GetZergGroundRushScore(Player *player);
int GetZergAirRushScore(Player *player);
int GetProtossAirRushScore(Player *player);

void AICreateThread(WORD wStartBlock, Position location, void *pTown = NULL);
void AICreateThread(const char *pszScriptID, Position location);

#define AI_THREAD_BROODWAR        0x01
#define AI_THREAD_KILLABLE        0x02
#define AI_THREAD_WANT_KILLED     0x04

class aithread
{
public:
  // ctors
  aithread();
  aithread(WORD wStartBlock, BWAPI::Position location, void *town = NULL);
  
  // dtor
  ~aithread();
  
  // Read data
  template <class _T>
  _T read()
  {
    _T rval = (_T&)pbAIScriptBinary[this->dwScriptOffset];
    this->dwScriptOffset += sizeof(_T);
    this->dwBytesRead    += sizeof(_T);
    return rval;
  }

  // Execute script
  void  execute();

  // Flags
  DWORD getFlags();
  void  setFlags(DWORD dwFlags);
  void  clearFlags(DWORD dwFlags);

  // 
  DWORD sleep();
  void  killThread();
  void  showDebug(int x, int y);
  void  saveDebug(const char prefix, int iOpcode, const char *pszFormat = NULL, ...);
  void  noretry();
  void  retry();

private:
  DWORD             dwScriptOffset;   // the offset in the AIScript file
  DWORD             dwSleepTime;      // the number of frames remaining before the thread continues execution
  DWORD             dwPlayerID;       // the player index that owns the thread

  Location          location;         // the location boundry that the script is being executed in
  //RECT              locationBounds;   // the location boundry of which the script is executed in
  //POINT             locationCenter;   // the center of the location of which the script is executed in
  //BWAPI::Position   locationCenter;
  void              *pTown;           // town information
  DWORD             dwFlags;          // the flags for the thread behaviour (contains baseID*8, flags are &7)
  BYTE              bTotBuildCount[UnitTypes::Enum::MAX]; // not included

  int               threadId;
  DWORD             dwBytesRead;
  std::deque<char*> debugQueue;

  bool retryBlock;
};

extern BYTE *pbAIScriptBinary;
extern std::list<aithread> aiThreadList;

