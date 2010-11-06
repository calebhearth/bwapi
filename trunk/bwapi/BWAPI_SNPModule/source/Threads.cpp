#include <windows.h>

#include "Threads.h"
#include "Connect.h"
#include "Commands.h"

namespace LUDP
{
  bool gbWantExit = false;
  volatile pktq *gpRecvQueue;
  volatile void *gpGameAdvert;

  DWORD WINAPI RecvThread(LPVOID)
  {
    while (1)
    {
      // create receiving sockaddr
      SOCKADDR_IN saFrom;
      int dwSaFromLen = sizeof(SOCKADDR);

      // recvfrom
      char szBuffer[LUDP_PKT_SIZE + sizeof(packet)];
      memset(szBuffer, 0, LUDP_PKT_SIZE + sizeof(packet));
      int rVal = recvfrom(gsRecv, szBuffer, LUDP_PKT_SIZE + sizeof(packet), 0, (SOCKADDR*)&saFrom, &dwSaFromLen);
      if ( gbWantExit )
        return 0;

      switch ( rVal )
      {
      case SOCKET_ERROR:
        Error(WSAGetLastError(), "recvfrom (broadcast) failed");
      case 0: // closed connection
        return 0;
      }
      ++gdwRecvCalls;
      gdwRecvBytes += rVal;

      SMemZero(saFrom.sin_zero, sizeof(saFrom.sin_zero));
      packet *bc  = (packet*)&szBuffer;
      if ( bc->wSize >= sizeof(packet) && bc->wSize == rVal )
      {
        switch ( bc->wType )
        {
        case CMD_ADDGAME: // add/update game
        case CMD_REMOVEGAME: // remove game
          // advertise game/response
          UpdateGameList(&saFrom, szBuffer, bc->wType == CMD_REMOVEGAME);
          break;
        case CMD_GETLIST:
          // request list
          BroadcastAdvertisement(&saFrom);
          break;
        case CMD_STORM:
          {
            pktq *recvPkt = (pktq*)SMAlloc(sizeof(pktq));
            if ( !recvPkt )
              Error(ERROR_NOT_ENOUGH_MEMORY, "Recv Allocation error");

            memcpy(&recvPkt->saFrom, &saFrom, sizeof(SOCKADDR));
            recvPkt->dwLength    = rVal - sizeof(packet);
            memcpy(recvPkt->bData, &szBuffer[sizeof(packet)], recvPkt->dwLength);
            recvPkt->pNext       = NULL;

            EnterCriticalSection(&gCrit);
            if ( gpRecvQueue )
            {
              volatile pktq *i = gpRecvQueue;
              while ( i->pNext )
              {
                i = i->pNext;
              }
              i->pNext = recvPkt;
            }
            else
            {
              gpRecvQueue = recvPkt;
            }
            LeaveCriticalSection(&gCrit);
            SetEvent(ghRecvEvent);
            Log("Should receive something");
            break;
          }
        default:
          Error(ERROR_INVALID_PARAMETER, "Unidentified broadcast type %04X", bc->wType);
          break;
        }
      }
      else
      {
        Error(ERROR_INVALID_PARAMETER, "Bad broadcast packet");
      }
    } // loop
    return 0;
  }
};
