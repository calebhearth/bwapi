#include "Actions.h"

#include "FileReader.h"
#include "ReplayReader.h"

#include <stdio.h>
#include <BWAPI.h>
#include <fstream>
/*
int GetActionSize(BYTE *pBuffer)
{
  int iSize = 1;
  switch ( pBuffer[0] )
  {
  case Actions::Save_Game:
  case Actions::Load_Game:
    iSize += 4;
    while ( pBuffer[iSize] )
      ++iSize;
    break;
  case Actions::Chat_Replay:
    iSize += 81;
  case Actions::Select_Delta_Add:
  case Actions::Select_Delta_Del:
  case Actions::Select_Units:
    iSize += 2 * pBuffer[iSize];
    break;
  case Actions::Placebox:
    iSize += 7;
    break;
  case Actions::Set_Fog:
  case Actions::Group_Units:
  case Actions::Train:
  case Actions::Cancel_Train:
  case Actions::Unit_Morph:
  case Actions::Exit_Transport:
  case Actions::Building_Morph:
    iSize += 2;
    break;
  case Actions::Set_Allies:
  case Actions::Cheat:
  case Actions::Lift_Off:
  case Actions::Ping_Minimap:
    iSize += 4;
    break;
  case Actions::Set_Speed:
  case Actions::Stop:
  case Actions::Return:
  case Actions::Cloak_Off:
  case Actions::Cloak_On:
  case Actions::Tank_Siege:
  case Actions::Tank_Unsiege:
  case Actions::Unload_All:
  case Actions::Hold_Position:
  case Actions::Burrow_Down:
  case Actions::Burrow_Up:
  case Actions::Research:
  case Actions::Upgrade:
  case Actions::Voice_Enable:
  case Actions::Voice_Squelch:
  case Actions::Set_Latency:
  case Actions::Leave_Game:
    iSize += 1;
    break;
  case Actions::Right_Click:
  case Actions::Set_Replay_Speed:
    iSize += 9;
    break;
  case Actions::Target_Click:
    iSize += 10;
    break;
  default:
    break;
  }
  return iSize;
}
*/

void ParseActions(ReplayReader &rr, const char *pszFilename)
{
  rr.log("---------------------------------------------------------------> %s\n", pszFilename);
  while ( rr == true )
  {
    // Begin reading a new replay frame
    rr.newFrame();
    while ( rr == true && rr.isValidFrame() )
    {
      // Get the player ID and opcode for the current command being executed
      BYTE bPlayerID = rr.read<BYTE>();
      BYTE bCommand  = rr.read<BYTE>();

      rr.log("(P%d) %s: ", bPlayerID, bCommand < Actions::Max ? Actions::pszActionNames[bCommand] : "INVALID");
      switch ( bCommand )
      {
      case Actions::Save_Game:
      case Actions::Load_Game:
        {
          DWORD dwSaveInfo = rr.read<DWORD>();
          rr.log("0x%08X, %s", dwSaveInfo, rr.ReadCString().c_str());
        }
        break;
      case Actions::Chat_Replay:
        {
          BYTE bChatID = rr.read<BYTE>();
          char szMessage[80];
          for ( int i = 0; i < sizeof(szMessage); ++i )
            szMessage[i] = rr.read<char>();
          rr.log("%u, %s", bChatID, szMessage);
        }
        break;
      case Actions::Select_Delta_Add:
      case Actions::Select_Delta_Del:
      case Actions::Select_Units:
        {
          WORD wUnits[12] = { 0 };
          BYTE bUnitCount = rr.read<BYTE>();

          if ( bUnitCount > 12 )
            bUnitCount = 12;
          rr.log("%u", bUnitCount);
          for ( BYTE i = 0; i < bUnitCount; ++i )
          {
            wUnits[i] = rr.read<WORD>();
            rr.log(", %u:%02X", wUnits[i] & 0x7FF, wUnits[i] >> 12);
          }
        }
        break;
      case Actions::Placebox:
        {
          BWAPI::Order orderType( rr.readOrder() );
          short x = rr.read<short>();
          short y = rr.read<short>();
          BWAPI::UnitType unitType( rr.readUnitType() );
          rr.log("%s, (%d, %d), %s", orderType.c_str(), x, y, unitType.c_str());
        }
        break;
      case Actions::Set_Fog:
          rr.log("%04X", rr.read<WORD>() );
        break;
      case Actions::Group_Units:
        {
          BYTE bGroupType = rr.read<BYTE>();
          BYTE bGroupNum  = rr.read<BYTE>();

          rr.log("%s, %u", bGroupType < 3 ? Actions::pszGroupType[bGroupType] : "INVALID", bGroupNum );
        }
        break;
      case Actions::Train:
      case Actions::Unit_Morph:
      case Actions::Building_Morph:
          rr.log("%s", rr.readUnitType().c_str());
        break;
      case Actions::Cancel_Train:
      case Actions::Exit_Transport:
        {
          WORD wUnitID = rr.read<WORD>();
          rr.log("(%u, 0x%02X)", wUnitID & 0x7FF, wUnitID >> 12);
        }
        break;
      case Actions::Set_Allies:
        rr.log("0x%08X", rr.read<DWORD>() );
        break;
      case Actions::Cheat:
        {
          DWORD dwCheatFlags = rr.read<DWORD>();
          rr.log("0x%08X", dwCheatFlags);
        }
        break;
      case Actions::Lift_Off:
      case Actions::Ping_Minimap:
        {
          short x = rr.read<short>();
          short y = rr.read<short>();
          rr.log("(%d, %d)", x, y);
        }
        break;
      case Actions::Set_Speed:
        {
          BYTE bSpeed = rr.read<BYTE>();
          rr.log("%s", bSpeed < 7 ? Actions::pszGameSpeed[bSpeed] : "INVALID");
        }
        break;
      case Actions::Stop:
      case Actions::Return:
      case Actions::Cloak_Off:
      case Actions::Cloak_On:
      case Actions::Tank_Siege:
      case Actions::Tank_Unsiege:
      case Actions::Unload_All:
      case Actions::Hold_Position:
      case Actions::Burrow_Down:
      case Actions::Burrow_Up:
        rr.log("%s", rr.read<BYTE>() ? "Queued" : "");
        break;
      case Actions::Research:
        rr.log("%s", rr.readTechType().c_str());
        break;
      case Actions::Upgrade:
        rr.log("%s", rr.readUpgradeType().c_str());
        break;
      case Actions::Voice_Enable:   // Unused
      case Actions::Voice_Squelch:  // Unused
        rr.log("%u", rr.read<BYTE>());  // squelched player id
        break;
      case Actions::Set_Latency:
        {
          BYTE bLatency = rr.read<BYTE>();
          rr.log("%s", bLatency < 3 ? Actions::pszLatency[bLatency] : "INVALID");
        }
        break;
      case Actions::Leave_Game:
        {
          BYTE bLeaveType = rr.read<BYTE>();
          rr.log("%s", bLeaveType < 6 ? Actions::pszLeaveType[bLeaveType] : "UNKNOWN");
        }
        break;
      case Actions::Right_Click:
        {
          short x = rr.read<short>();
          short y = rr.read<short>();
          WORD wTargetID = rr.read<WORD>();
          BWAPI::UnitType unitType( rr.readUnitType() );
          BYTE bHow = rr.read<BYTE>();
          rr.log("(%d, %d), %u:%02X, %s%s", x, y, wTargetID & 0x7FF, wTargetID >> 12, unitType.c_str(), bHow ? ", Queued" : "");
        }
        break;
      case Actions::Set_Replay_Speed:
        {
          bool paused = rr.read<bool>();
          DWORD dwSpeed = rr.read<DWORD>();
          DWORD dwMultiplier = rr.read<DWORD>();
          rr.log("%s%s, ×%u", paused ? "PAUSED, " : "", dwSpeed < 7 ? Actions::pszGameSpeed[dwSpeed] : "INVALID", dwMultiplier);
        }
        break;
      case Actions::Target_Click:
        {
          short x = rr.read<short>();
          short y = rr.read<short>();
          WORD wTargetID = rr.read<WORD>();
          BWAPI::UnitType unitType( rr.readUnitType() );
          BWAPI::Order orderType( rr.readOrder() );
          BYTE bHow = rr.read<BYTE>();
          rr.log("(%d, %d), %u:%02X, %s, %s%s", x, y, wTargetID & 0x7FF, wTargetID >> 12, unitType.c_str(), orderType.c_str(), bHow ? ", Queued" : "");
        }
        break;
      default:
        break;
      } // switch
      rr.log("\n");
    } // per-frame loop

  } // per-replay loop

}
