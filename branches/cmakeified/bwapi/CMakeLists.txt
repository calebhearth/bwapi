CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(BWAPIv2)

#SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
#SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
#SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

ADD_SUBDIRECTORY(Util)
ADD_SUBDIRECTORY(BWAPILIB)
ADD_SUBDIRECTORY(BWAPI)
ADD_SUBDIRECTORY(BWAPI_ChaosLauncherInjector)

#-------------------------------------------------------------------------------------------------------------
# Configure svnrev.h to reflect the svn revision number
# TODO: Create a CMake target to do this

FIND_PROGRAM(SVN_EXECUTABLE svn
  DOC "subversion command line client")

MACRO(Subversion_GET_REVISION dir variable)
  EXECUTE_PROCESS(COMMAND ${SVN_EXECUTABLE} info ${dir}
    OUTPUT_VARIABLE ${variable}
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  STRING(REGEX REPLACE "^(.*\n)?Revision: ([^\n]+).*"
    "\\2" ${variable} "${${variable}}")
ENDMACRO()

MACRO(Subversion_GET_LOCAL_MODS dir variable)
  EXECUTE_PROCESS(COMMAND ${SVN_EXECUTABLE} status -q ${dir}
    OUTPUT_VARIABLE ${variable}
    OUTPUT_STRIP_TRAILING_WHITESPACE)
ENDMACRO()

Subversion_GET_REVISION(${PROJECT_SOURCE_DIR} SVN_REV)
Subversion_GET_LOCAL_MODS(${PROJECT_SOURCE_DIR} SVN_LOCAL_MODS)

SET(SVN_REV_STR "${SVN_REV}")
IF(NOT ${SVN_LOCAL_MODS} STREQUAL "")
  SET(SVN_REV_STR "${SVN_REV_STR}_M") 
ENDIF()

CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/svnrev.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/svnrev.h"
  IMMEDIATE @ONLY)

#-------------------------------------------------------------------------------------------------------------
# Custom target to directly install the Chaoslauncher plugin into the Chaoslauncher directory

FIND_PATH(STARCRAFT_ROOT_DIR StarCraft.exe
  PATHS
  $ENV{ProgramFiles}/Starcraft
  C:/Starcraft
  $ENV{UserProfile}/Starcraft
  # TODO: This works on Win7 x64, what about the other Windows versions?
  "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Blizzard Entertainment\\Starcraft;InstallPath]"
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

#-------------------------------------------------------------------------------------------------------------
# Hide the annoying CMAKE_ECLIPSE_EXECUTABLE-NOTFOUND variable
MARK_AS_ADVANCED(FORCE CMAKE_ECLIPSE_EXECUTABLE)

#-------------------------------------------------------------------------------------------------------------
# Setup the systems include directories for Visual Studio so that Eclipse can find the headers  
# Analogous to what is done in CMakeFindEclipseCDT4.cmake

# The environment from the vcvars.bat file has to be setup

SET(_dirs
  #$ENV{VCINSTALLDIR}\\include
  #$ENV{WindowsSdkDir}Include
  $ENV{INCLUDE}
)

# TODO: set the VC++ predefined macros for use in Eclipse IDE
SET(_defines "")

IF (NOT CMAKE_ECLIPSE_C_SYSTEM_INCLUDE_DIRS)
  IF ("${CMAKE_C_COMPILER_ID}" STREQUAL MSVC)
    SET(CMAKE_ECLIPSE_C_SYSTEM_INCLUDE_DIRS "${_dirs}" CACHE INTERNAL "C compiler system include directories")
    SET(CMAKE_ECLIPSE_C_SYSTEM_DEFINED_MACROS "${_defines}" CACHE INTERNAL "C compiler system defined macros")
  ENDIF ()
ENDIF ()

IF (NOT CMAKE_ECLIPSE_CXX_SYSTEM_INCLUDE_DIRS)
  IF ("${CMAKE_C_COMPILER_ID}" STREQUAL MSVC)
    SET(CMAKE_ECLIPSE_CXX_SYSTEM_INCLUDE_DIRS "${_dirs}" CACHE INTERNAL "CXX compiler system include directories")
    SET(CMAKE_ECLIPSE_CXX_SYSTEM_DEFINED_MACROS "${_defines}" CACHE INTERNAL "CXX compiler system defined macros")
  ENDIF ()
ENDIF ()
