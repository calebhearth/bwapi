#pragma once
#include <Windows.h>

void RegisterThreadName(const char *threadName, DWORD threadId = ~0);
void CheckRegisteredThreads();

void SetThreadName(const char *threadName, DWORD threadId = ~0);
