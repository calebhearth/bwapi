#pragma once
#include <Windows.h>
#include <cstdio>
#include <BWAPI.h>

using namespace BWAPI;

class ReplayReader
{
public:
  ReplayReader(DWORD dwSize, const char *pszDebugOutput = nullptr);
  ~ReplayReader();

  // Used to manipulate raw data
  operator void*() const;

  // Check if data was allocated correctly
  operator bool() const;

  // Read a chunk of data
  template <typename _T>
  _T read()
  {
    if ( this->pCurrent + sizeof(_T) < pActionsEnd )
    {
      _T *rval = (_T*)this->pCurrent;
      this->pCurrent += sizeof(_T);
      this->bFrameBytesRead += sizeof(_T);

      if ( this->bFrameBytesRead >= this->bFrameSize )
        this->validFrame = false;

      return *rval;
    }
    this->end = true;
    return 0;
  };

  UnitType    readUnitType();
  Order       readOrder();
  TechType    readTechType();
  UpgradeType readUpgradeType();
  std::string ReadCString();

  // Get the memory block size
  size_t size() const;

  // Begin reading a new frame
  void newFrame();

  // Write to the debug file
  void log(const char *pszFormat, ...) const;

  // Check if the current frame is still valid
  bool isValidFrame() const;

  // Retrieve highest frame count
  DWORD highestFrameTick() const;

private:
  // Memory location pointers
  BYTE *pActionsBegin;
  BYTE *pActionsEnd;
  BYTE *pCurrent;

  // Debug file
  FILE *dbgOutFile;

  // Frame bytes
  BYTE bFrameSize;
  BYTE bFrameBytesRead;

  // Frame time
  DWORD dwCurrentFrameTick;
  DWORD dwHighestFrameTick;

  // Flags
  bool end;
  bool validFrame;
};

