#pragma once
#include <string>

#define BWAPI_TYPESET(setname,name,...) static const name setname ## struc[] = { __VA_ARGS__ };                    \
                            static const name::set setname(setname ## struc, countof(setname ## struc));

#ifndef countof
#define countof(x) ((unsigned int)( sizeof(x)/sizeof((x)[0]) ))
#endif

namespace BWAPI
{
  void fixName(std::string *name);
  const char *underscoresToSpaces(const char *pszTypename);
}
