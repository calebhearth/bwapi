PROJECT(BWAPI_ChaosLauncherInjector)

SET(SOURCES 
  source/DLLMain.cpp
)

INCLUDE_DIRECTORIES( 
  ../Util/Source
  ../include
  
  # FIXME: This is for the #include "../../svnrev.h" which is created in the build directory
  ${CMAKE_CURRENT_BINARY_DIR}/Source
)

ADD_DEFINITIONS(
  -D_CRT_SECURE_NO_WARNINGS
)

ADD_LIBRARY(BWAPI_ChaosLauncherInjector MODULE ${SOURCES})
SET_TARGET_PROPERTIES(BWAPI_ChaosLauncherInjector PROPERTIES SUFFIX .bwl)
