
# Recquires CMake 2.6 or greater.
cmake_minimum_required (VERSION 2.6)

# General definitions.
project (ugdk CXX)
set (FRAMEWORK_NAME UGDK)
set (FRAMEWORK_RELEASENAME ugdk)
set (FRAMEWORK_DEBUGNAME ugdk_d)
set (FRAMEWORK_VERSION 1.0.0)

# If the user did not define the parameteres, use default values.
if (NOT BUILD_TYPE)
    set (BUILD_TYPE RELEASE)
endif (NOT BUILD_TYPE)
if (NOT LINK_TYPE)
    set (LINK_TYPE STATIC)
endif (NOT LINK_TYPE)

if (${BUILD_TYPE} STREQUAL RELEASE)
    set (FRAMEWORK_BUILDNAME ${FRAMEWORK_RELEASENAME})
elseif (${BUILD_TYPE} STREQUAL DEBUG)
    set (FRAMEWORK_BUILDNAME ${FRAMEWORK_DEBUGNAME})
else ()
    message (FATAL_ERROR "Unknown build type ${BUILD_TYPE}.")
endif ()


# Important build locations.
set (SRC_DIR src)
set (LIB_DIR lib)

# Some output.
message ("${FRAMEWORK_NAME} version ${FRAMEWORK_VERSION}")

include (${SRC_DIR}/src_list.cmake)

# #define's que voce queira incluir na compilacao
# separe por espacos
set (DEFS "" CACHE STRING "#define's que voce queira incluir na compilacao; separe por espacos")
if (WIN32)
    set (DEFS "${DEFS} WIN32")
endif (WIN32)

# CFLAGS: flags que voce queira enviar ao compilador
# LDFLAGS: flags que voce queira enviar ao ligador
set (CFLAGS "" CACHE STRING "flags enviadas ao compilador")
set (LDFLAGS "" CACHE STRING "flags enviadas ao linker")
if (UNIX OR MINGW)
    # se for o g++ para *nix ou o MinGW para Windows:
    set (CFLAGS "${CFLAGS} -Wall -ansi -O3 -U_FORTIFY_SOURCE")
    if (BUILD_TYPE EQUAL debug)
        set (CFLAGS "${CFLAGS} -g")
    endif (BUILD_TYPE EQUAL debug)
endif (UNIX OR MINGW)

# ${FRAMEWORK_SRC} esta' definido?
if (NOT FRAMEWORK_SRC)
    message (FATAL_ERROR "Error: FRAMEWORK_SRC not defined! Please do so in the file src/src_list.cmake!")
endif (NOT FRAMEWORK_SRC)

# Encontrando as bibliotecas necessarias:
if (WIN32)
	find_library (LGL NAMES OpenGL32 PATH "${CMAKE_LIBRARY_PATH}")
else (WIN32)
	find_library (LGL NAMES GL PATH "${CMAKE_LIBRARY_PATH}")
endif (WIN32)
if (NOT LGL)
    message (FATAL_ERROR "Fatal error: libGL not found!")
endif (NOT LGL)

find_library (LSDL NAMES SDL PATH "${CMAKE_LIBRARY_PATH}")
if (NOT LSDL)
    message (FATAL_ERROR "Fatal error: libSDL not found!")
endif  (NOT LSDL)

find_library (LSDLIMAGE NAMES SDL_image PATH "${CMAKE_LIBRARY_PATH}")
if (NOT LSDLIMAGE)
    message (FATAL_ERROR "Fatal error: libSDL_image not found!")
endif  (NOT LSDLIMAGE)

find_library (LSDLMIXER NAMES SDL_mixer PATH "${CMAKE_LIBRARY_PATH}")
if (NOT LSDLMIXER)
    message (FATAL_ERROR "Fatal error: libSDL_mixer not found!")
endif  (NOT LSDLMIXER)

find_library (LSDLTTF NAMES SDL_ttf PATH "${CMAKE_LIBRARY_PATH}")
if (NOT LSDLTTF)
    message (FATAL_ERROR "Fatal error: libSDL_ttf not found!")
endif  (NOT LSDLTTF)

# pre-processor: #define's
if (MSVC)
  foreach (d ${DEFS})
    set (CFLAGS_EXTRA "${CFLAGS_EXTRA} /D${d}")
  endforeach (d)
else (MSVC)
  foreach (d ${DEFS})
    set (CFLAGS_EXTRA "${CFLAGS_EXTRA} -D${d}")
  endforeach (d)
endif  (MSVC)

if (${BUILD_TYPE} EQUAL DEBUG)
    set (CFLAGS_EXTRA "${CFLAGS_EXTRA} -DDEBUG")
endif (${BUILD_TYPE} EQUAL DEBUG)

# *nix library
if (UNIX)
    if (${LINK_TYPE} STREQUAL STATIC)
        add_library (${FRAMEWORK_BUILDNAME} STATIC ${FRAMEWORK_SRC})
    elseif (${LINK_TYPE} STREQUAL SHARED)
        add_library (${FRAMEWORK_BUILDNAME} SHARED ${FRAMEWORK_SRC})
    endif (${LINK_TYPE} STREQUAL STATIC)
    set_target_properties (${FRAMEWORK_BUILDNAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})
    set_target_properties (${FRAMEWORK_BUILDNAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${LIB_DIR})
    set_target_properties (${FRAMEWORK_BUILDNAME} PROPERTIES LINK_FLAGS "${LDFLAGS}")
    target_link_libraries (${FRAMEWORK_BUILDNAME} m SDL SDL_image SDL_mixer SDL_ttf GL GLU)
    set_target_properties (${FRAMEWORK_BUILDNAME} PROPERTIES COMPILE_FLAGS "${CFLAGS} ${CFLAGS_EXTRA}")
    if (${BUILD_TYPE} STREQUAL RELEASE)
        set_target_properties (${FRAMEWORK_BUILDNAME} PROPERTIES PROJECT_NAME "${FRAMEWORK_NAME}")
    elseif (${BUILD_TYPE} STREQUAL DEBUG)
        set_target_properties (${FRAMEWORK_BUILDNAME} PROPERTIES PROJECT_NAME "${FRAMEWORK_NAME} (DEBUG)")
    endif ()
# Windows library
elseif (WIN32)
    if (${LINK_TYPE} STREQUAL STATIC)
        add_library (${FRAMEWORK_BUILDNAME} STATIC ${FRAMEWORK_SRC})
    elseif (${LINK_TYPE} STREQUAL SHARED)
        add_library (${FRAMEWORK_BUILDNAME} SHARED ${FRAMEWORK_SRC})
    endif (${LINK_TYPE} STREQUAL STATIC)
    set_target_properties (${FRAMEWORK_BUILDNAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})
    set_target_properties (${FRAMEWORK_BUILDNAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${LIB_DIR})
	set_target_properties (${FRAMEWORK_BUILDNAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${LIB_DIR})
    set_target_properties (${FRAMEWORK_BUILDNAME} PROPERTIES LINK_FLAGS "${LDFLAGS}")
    set_target_properties (${FRAMEWORK_BUILDNAME} PROPERTIES COMPILE_FLAGS "${CFLAGS} ${CFLAGS_EXTRA}")
    target_link_libraries (${FRAMEWORK_BUILDNAME} m mingw32 SDLmain SDL SDL_image SDL_mixer SDL_ttf OpenGL32 glu32)
    if (${BUILD_TYPE} STREQUAL RELEASE)
        set_target_properties (${FRAMEWORK_BUILDNAME} PROPERTIES PROJECT_NAME "${FRAMEWORK_NAME}")
    elseif (${BUILD_TYPE} STREQUAL DEBUG)
        set_target_properties (${FRAMEWORK_BUILDNAME} PROPERTIES PROJECT_NAME "${FRAMEWORK_NAME} (DEBUG)")
    endif ()
endif  (UNIX)

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
    "${SRC_DIR}/config/config.h.in"
    "${SRC_DIR}/config/config.h"
)



