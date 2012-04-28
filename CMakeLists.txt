
# Recquires CMake 2.6 or newer.
cmake_minimum_required (VERSION 2.6)

project (ugdk CXX C)
set (FRAMEWORK_NAME    UGDK)
set (FRAMEWORK_RELEASE ugdk)
set (FRAMEWORK_DEBUG   ugdk_d)
set (FRAMEWORK_VERSION 0.3.0)

message ("=== ${FRAMEWORK_NAME} version ${FRAMEWORK_VERSION} ===")

# If the user did not define the parameteres, use default values.
if (NOT LINK_TYPE)
    message ("-- No link type was defined, choosing default value: STATIC")
    set (LINK_TYPE STATIC)
else (NOT LINK_TYPE)
    message ("-- Link type: ${LINK_TYPE}")
endif (NOT LINK_TYPE)

if ((NOT (${LINK_TYPE} STREQUAL STATIC)) AND (NOT (${LINK_TYPE} STREQUAL SHARED)))
    message (FATAL_ERROR ">>> Unknown link type ${LINK_TYPE}.")
endif ()

if (${LINK_TYPE} STREQUAL SHARED)
	set(BUILD_SHARED_LIBS True)
endif ()

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET (CHECKMAC "#define ISMAC")
endif ()

IF (UNIVERSAL)
    # Necessary for a Mac Universal Binary
    SET (CMAKE_OSX_ARCHITECTURES ppc;i386;x86_64)
    SET (CMAKE_OSX_SYSROOT /Developer/SDKs/MacOSX10.5.sdk)
    SET (MACOSX_DEPLOYMENT_TARGET 10.5)
ENDIF ()

# Important build locations.
set (SRC_DIR src)
set (LIB_DIR lib)

include (${SRC_DIR}/src_list.cmake)
include (${SRC_DIR}/ugdk_module_list.cmake)
include (${SRC_DIR}/pyramidworks_module_list.cmake)
include (${SRC_DIR}/lua_src_list.cmake)
include (${SRC_DIR}/py_src_list.cmake)

# LDFLAGS: flags que voce queira enviar ao ligador
set (LDFLAGS "" CACHE STRING "flags enviadas ao linker")
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	LIST(APPEND LDFLAGS "-mmacosx-version-min=10.5")
endif ()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
if (UNIX OR MINGW)
    # se for o g++ para *nix ou o MinGW para Windows:
    add_definitions(-Wall -ansi -U_FORTIFY_SOURCE -msse2)
elseif (MSVC) 
    add_definitions(/D_CRT_SECURE_NO_WARNINGS /DBOOST_ALL_NO_LIB /W4 /wd4100 /wd4127 /wd4201 /wd4211 /wd4250 /wd4706 /arch:SSE2 /fp:fast /MP)
endif ()

# ${UGDK_SRC} está definido?
if (NOT UGDK_SRC)
    message (FATAL_ERROR "Error: UGDK_SRC not defined! Please do so in the file src/src_list.cmake!")
endif (NOT UGDK_SRC)

include_directories ("src")
include (cmake/ugdk.cmake)

# Encontrando as bibliotecas necessarias:
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

find_package(SDL REQUIRED)
include_directories(${SDL_INCLUDE_DIR})

find_package(SDL_mixer REQUIRED)
include_directories(${SDLMIXER_INCLUDE_DIR})

find_package(SDL_ttf REQUIRED)
include_directories(${SDLTTF_INCLUDE_DIR})

find_package(SDL_image REQUIRED)
include_directories(${SDLIMAGE_INCLUDE_DIR})

find_package(Boost 1.48.0 REQUIRED)
include_directories("${Boost_INCLUDE_DIRS}/boost/tr1/tr1")
include_directories(${Boost_INCLUDE_DIRS})

ugdk_setup_package (SWIG)
if (SWIG_FOUND)
  ugdk_setup_package (Lua51)
  ugdk_setup_package (PythonLibs)
endif (SWIG_FOUND)

include_directories ("src/kits")
include_directories ("externals/opengl")

# Bit hacky way so all the output files are placed on the ${LIB_DIR} folder.
# Code found on http://stackoverflow.com/questions/7747857/in-cmake-how-do-i-work-around-the-debug-and-release-directories-visual-studio-2

# First for the generic no-config case (e.g. with mingw)
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${LIB_DIR} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIB_DIR} )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR} )
# Second, for multi-config builds (e.g. msvc)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${LIB_DIR} )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${LIB_DIR} )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${LIB_DIR} )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )


add_library (${FRAMEWORK_RELEASE} ${UGDK_SRC} ${GENERATED_SRC})

if (CMAKE_COMPILER_IS_GNUCXX)
	set (EXTRA_LIBRARIES m ${EXTRA_LIBRARIES})
endif (CMAKE_COMPILER_IS_GNUCXX)
if (MINGW)
	set (EXTRA_LIBRARIES mingw32 ${EXTRA_LIBRARIES})
endif (MINGW)
if (WIN32)
	set (EXTRA_LIBRARIES Ws2_32 ${EXTRA_LIBRARIES})
endif (WIN32)

target_link_libraries (${FRAMEWORK_RELEASE}	LINK_INTERFACE_LIBRARIES 
					   ${EXTRA_LIBRARIES}
					   ${SDL_LIBRARY}
					   ${SDLMIXER_LIBRARY}
					   ${SDLTTF_LIBRARY}
					   ${SDLIMAGE_LIBRARY}
					   ${OPENGL_LIBRARIES}
                       optmized ${UGDK_LIBRARIES}
                       debug    ${UGDK_LIBRARIES_DEBUG})
                      
set_target_properties (${FRAMEWORK_RELEASE} PROPERTIES LINK_FLAGS "${LDFLAGS}")
set_target_properties (${FRAMEWORK_RELEASE} PROPERTIES PROJECT_NAME "${FRAMEWORK_NAME}")
set_target_properties (${FRAMEWORK_RELEASE} PROPERTIES OUTPUT_NAME_DEBUG "${FRAMEWORK_DEBUG}")

if (PYTHONLIBS_FOUND)
  include (cmake/asteroids.cmake)
endif (PYTHONLIBS_FOUND)

# Installing on *nix
#if (UNIX)
#   set (FRAMEWORK_INSTALL_DIR "/usr/lib/${FRAMEWORK_BUILDNAME}")
# 
#   install (CODE "message (\"Installing ${GAME_NAME} ${GAME_VERSION}... Make sure you have root privileges.\")")
# 
#   if (LINK_TYPE EQUAL STATIC)
#       install (TARGETS ${FRAMEWORK_BUILDNAME} LIBRARY DESTINATION ${FRAMEWORK_INSTALL_DIR})
#   elseif (LINK_TYPE EQUAL SHARED)
#       install (TARGETS ${FRAMEWORK_BUILDNAME} ARCHIVE DESTINATION ${FRAMEWORK_INSTALL_DIR})
#   else ()
#      message (FATAL_ERROR "")
#
  #install ( FILES license.txt readme.html DESTINATION ${GAME_INSTALL_DIR})
  #install ( DIRECTORY config images levels licenses musics quests samples screenshots themes languages DESTINATION ${GAME_INSTALL_DIR} PATTERN ".svn" EXCLUDE)
  # install ( DIRECTORY data DESTINATION ${GAME_INSTALL_DIR} PATTERN ".svn" EXCLUDE)
  #
  # install ( CODE "message (\"Creating files at ${GAME_FINAL_DIR}...\")")
  #install ( CODE "EXECUTE_PROCESS(COMMAND \"cmake\" \"-E\" \"copy\" \"${GAME_INSTALL_DIR}/${GAME_UNIXNAME}\" \"${GAME_FINAL_DIR}/${GAME_UNIXNAME}\")")
  #install ( CODE "EXECUTE_PROCESS(COMMAND \"mkdir\" \"$ENV{HOME}/.horus_eye\")")
  #install ( CODE "EXECUTE_PROCESS(COMMAND \"chmod\" \"777\" \"$ENV{HOME}/.horus_eye\")")
  #install ( CODE "set (USER_GROUP \"\")")
  #install ( CODE "EXECUTE_PROCESS(COMMAND \"id\" \"-g\" OUTPUT_VARIABLE USER_GROUP)")
  #install ( CODE "EXECUTE_PROCESS(COMMAND \"chown\" \"$ENV{USER}:${USER_GROUP}\" \"$ENV{HOME}/.horus_eye\")")
  #install ( CODE "EXECUTE_PROCESS(COMMAND \"echo\" \"-n\" \"${GAME_INSTALL_DIR}/\" OUTPUT_FILE \"$ENV{HOME}/.horus_eye/rootpath.txt\")")

  #install ( CODE "message (\"Done! Please run ${GAME_UNIXNAME} to start ${GAME_NAME}.\")")
  #endif  (UNIX)

  #set (USER_HOME "$ENV{HOME}")

CONFIGURE_FILE(
    "${SRC_DIR}/ugdk/config/config.h.in"
    "${SRC_DIR}/ugdk/config/config.h"
)



