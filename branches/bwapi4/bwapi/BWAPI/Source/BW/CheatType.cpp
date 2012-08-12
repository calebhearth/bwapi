#pragma once
#include "CheatType.h"
#include <cstring>
namespace BW
{
  CheatFlags::Enum getCheatFlag(const char* name)
  {
    if ( _strcmpi(name, "black sheep wall") == 0 )
      return CheatFlags::BlackSheepWall;
    else if ( _strcmpi(name, "breathe deep") == 0 )
      return CheatFlags::BreatheDeep;
    else if ( _strcmpi(name, "food for thought") == 0 )
      return CheatFlags::FoodForThought;
    else if ( _strcmpi(name, "game over man") == 0 )
      return CheatFlags::GameOverMan;
    else if ( _strcmpi(name, "medieval man") == 0 )
      return CheatFlags::MedievalMan;
    else if ( _strcmpi(name, "modify the phase variance") == 0 )
      return CheatFlags::ModifyThePhaseVariance;
    else if ( _strcmpi(name, "noglues") == 0 )
      return CheatFlags::NoGlues;
    else if ( _strcmpi(name, "operation cwal") == 0 )
      return CheatFlags::OperationCwal;
    else if ( _strcmpi(name, "ophelia") == 0 )
      return CheatFlags::Ophelia;
    else if ( _strcmpi(name, "power overwhelming") == 0 )
      return CheatFlags::PowerOverwelming;
    else if ( _strcmpi(name, "show me the money") == 0 )
      return CheatFlags::ShowMeTheMoney;
    else if ( _strcmpi(name, "something for nothing") == 0 )
      return CheatFlags::SomethingForNothing;
    else if ( _strcmpi(name, "staying alive") == 0 )
      return CheatFlags::StayingAlive;
    else if ( _strcmpi(name, "the gathering") == 0 )
      return CheatFlags::TheGathering;
    else if ( _strcmpi(name, "there is no cow level") == 0 )
      return CheatFlags::ThereIsNoCowLevel;
    else if ( _strcmpi(name, "war aint what it used to be") == 0 )
      return CheatFlags::WarAintWhatItUsedToBe;
    else if ( _strcmpi(name, "whats mine is mine") == 0 )
      return CheatFlags::WhatsMineIsMine;
    return CheatFlags::None;
  }
}
