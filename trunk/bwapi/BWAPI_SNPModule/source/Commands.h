#pragma once
#include <windows.h>

#include "LocalPC.h"
#include "Threads.h"

#define CMD_ADDGAME     0
#define CMD_REMOVEGAME  1
#define CMD_GETLIST     2
#define CMD_STORM       3

void BroadcastAdvertisement(SOCKADDR *to = NULL);
void BroadcastGameListRequest();
void CleanGameList(DWORD dwTimeout);
void UpdateGameList(SOCKADDR_IN *from, char *data, bool remove);
