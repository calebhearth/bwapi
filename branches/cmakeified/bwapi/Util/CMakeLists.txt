PROJECT(Util)

SET(SOURCES 
  Source/Util/Exceptions.cpp
  Source/Util/FileLogger.cpp
  Source/Util/Logger.cpp
  Source/Util/Strings.cpp
)

ADD_DEFINITIONS(
  -D_CRT_SECURE_NO_WARNINGS
)

ADD_LIBRARY(Util STATIC ${SOURCES})
