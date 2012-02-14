#pragma once
#include <string>

#ifndef countof
#define countof(x) ((unsigned int)( sizeof(x)/sizeof((x)[0]) ))
#endif

namespace BWAPI
{
	void fixName(std::string *name);
	const char *underscoresToSpaces(const char *pszTypename);
}
