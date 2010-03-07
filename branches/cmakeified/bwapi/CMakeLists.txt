CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(BWAPIv2)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
#SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
#SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
#SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

ADD_SUBDIRECTORY(Util)
ADD_SUBDIRECTORY(BWAPILIB)
ADD_SUBDIRECTORY(BWAPI)
ADD_SUBDIRECTORY(BWAPI_ChaosLauncherInjector)

#-------------------------------------------------------------------------------------------------------------
# Includes
INCLUDE(svnrevconfig)
INCLUDE(setupeclipse)

#-------------------------------------------------------------------------------------------------------------
# Custom target to directly install the Chaoslauncher plugin into the Chaoslauncher directory

FIND_PATH(STARCRAFT_ROOT_DIR StarCraft.exe
  PATHS
  "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Blizzard Entertainment\\Starcraft;InstallPath]"
  "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Blizzard Entertainment\\Starcraft;InstallPath]"
  $ENV{ProgramFiles}/Starcraft
  C:/Starcraft
  $ENV{UserProfile}/Starcraft
)

FIND_PATH(CHAOS_DIR Chaoslauncher.exe
  PATHS
  $ENV{ChaosDir}
  ${STARCRAFT_ROOT_DIR}/Chaoslauncher
)
	
GET_TARGET_PROPERTY(BWAPI_OUTPUT BWAPI LOCATION)
GET_TARGET_PROPERTY(BWAPI_ChaosLauncherInjector_OUTPUT BWAPI_ChaosLauncherInjector LOCATION)

ADD_CUSTOM_TARGET(install_clauncher 
  COMMAND ${CMAKE_COMMAND} -E copy "${BWAPI_OUTPUT}" "$ENV{ChaosDir}"
  COMMAND ${CMAKE_COMMAND} -E copy "${BWAPI_ChaosLauncherInjector_OUTPUT}" "$ENV{ChaosDir}"
  DEPENDS BWAPI BWAPI_ChaosLauncherInjector
)
