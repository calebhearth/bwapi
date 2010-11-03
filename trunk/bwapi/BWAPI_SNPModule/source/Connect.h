#pragma once
#include <windows.h>
#include <Winsock.h>

extern SOCKET   gsRecv;
extern SOCKADDR gaddrRecv;

extern SOCKET   gsBCRecv;
extern SOCKADDR gaddrBCRecv;

extern SOCKET   gsSend;

extern SOCKET   gsBroadcast;
extern SOCKADDR gaddrBroadcast;

extern DWORD gdwSendCalls;
extern DWORD gdwSendBytes;
extern DWORD gdwRecvCalls;
extern DWORD gdwRecvBytes;

SOCKET   MakeUDPSocket();
SOCKADDR *InitAddr(SOCKADDR *addr, const char *ip = "127.0.0.1", WORD wPort = 6112);

bool InitializeSockets();
void DestroySockets();

