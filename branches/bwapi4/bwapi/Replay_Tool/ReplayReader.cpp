#include "ReplayReader.h"
#include <stdio.h>

ReplayReader::ReplayReader(DWORD dwSize, const char *pszDebugOutput)
  : pActionsBegin( (BYTE*)malloc(dwSize) )
  , pActionsEnd( pActionsBegin == nullptr ? nullptr : pActionsBegin + dwSize  )
  , pCurrent( pActionsBegin )
  , dbgOutFile( pszDebugOutput == nullptr ? nullptr : fopen(pszDebugOutput, "a") )
  , bFrameSize(0)
  , bFrameBytesRead(0)
  , end(false)
  , validFrame(false)
  , dwCurrentFrameTick(0)
  , dwHighestFrameTick(0)
{}

ReplayReader::~ReplayReader()
{
  if ( this->pActionsBegin != nullptr )
  {
    free(this->pActionsBegin);
    this->pActionsBegin = nullptr;
  }
  if ( this->dbgOutFile != nullptr )
  {
    fclose(this->dbgOutFile);
    this->dbgOutFile = nullptr;
  }
}

ReplayReader::operator void *() const
{
  return this->pActionsBegin;
}

ReplayReader::operator bool() const
{
  return this->pActionsBegin != nullptr && this->end == false;
}

size_t ReplayReader::size() const
{
  return this->pActionsEnd - this->pActionsBegin;
}

void ReplayReader::newFrame()
{
  // Read the frame time
  DWORD dwNewFrameTick = this->read<DWORD>();
  // Log an error message if the frame is impossible
  if ( dwNewFrameTick < this->dwCurrentFrameTick )
  {
    this->log("#### ERROR: Frame count has decreased ( %u < %u ) ####\n", dwNewFrameTick, this->dwCurrentFrameTick);
  }
  // store the current frame tick
  this->dwCurrentFrameTick = dwNewFrameTick;
  if ( dwNewFrameTick > this->dwHighestFrameTick )  // log the highest frame tick
    this->dwHighestFrameTick = dwNewFrameTick;

  // Read the frame byte count
  this->bFrameSize = this->read<BYTE>();
  this->bFrameBytesRead = 0;

  // Set as valid frame
  this->validFrame = true;
}

UnitType ReplayReader::readUnitType()
{
  return UnitType( this->read<WORD>() );
}

Order ReplayReader::readOrder()
{
  return Order( this->read<BYTE>() );
}

TechType ReplayReader::readTechType()
{
  return TechType( this->read<BYTE>() );
}
UpgradeType ReplayReader::readUpgradeType()
{
  return UpgradeType( this->read<BYTE>() );
}

std::string ReplayReader::ReadCString()
{
  if ( !*this )
    return "";

  std::string str;

  size_t len = 0;
  char c = '\0';
  char *pStart = (char*)this->pCurrent;
  while ( this->read<char>() != '\0' )
  {}

  str.assign(pStart, (char*)this->pCurrent - pStart);
  return str;
}

bool ReplayReader::isValidFrame() const
{
  return this->validFrame;
}

void ReplayReader::log(const char *pszFormat, ...) const
{
  if ( this->dbgOutFile != nullptr )
  {
    va_list vl;
    va_start(vl,pszFormat);
    vfprintf(this->dbgOutFile, pszFormat, vl);
    va_end(vl);
  }
}

DWORD ReplayReader::highestFrameTick() const
{
  return this->dwHighestFrameTick;
}

