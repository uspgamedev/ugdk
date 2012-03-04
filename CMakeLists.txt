
# Recquires CMake 2.6 or greater.
cmake_minimum_required (VERSION 2.6)

# General definitions.
project (ugdk CXX C)
set (FRAMEWORK_NAME UGDK)
set (FRAMEWORK_RELEASENAME ugdk)
set (FRAMEWORK_DEBUGNAME ugdk_d)
set (FRAMEWORK_VERSION 0.2.0)

# Some output.
message ("=== ${FRAMEWORK_NAME} version ${FRAMEWORK_VERSION} ===")

# If the user did not define the parameteres, use default values.
if (NOT BUILD_TYPE)
    message ("-- No build type was defined, choosing default value: RELEASE")
    set (BUILD_TYPE RELEASE)
else (NOT BUILD_TYPE)
    message ("-- Build type: ${BUILD_TYPE}")
endif (NOT BUILD_TYPE)

if (NOT LINK_TYPE)
    message ("-- No link type was defined, choosing default value: STATIC")
    set (LINK_TYPE STATIC)
else (NOT LINK_TYPE)
    message ("-- Link type: ${LINK_TYPE}")
endif (NOT LINK_TYPE)

if (${BUILD_TYPE} STREQUAL RELEASE)
    set (FRAMEWORK_BUILDNAME ${FRAMEWORK_RELEASENAME})
elseif (${BUILD_TYPE} STREQUAL DEBUG)
    set (FRAMEWORK_BUILDNAME ${FRAMEWORK_DEBUGNAME})
	set (FRAMEWORK_NAME "${FRAMEWORK_NAME} (DEBUG)")
else ()
    message (FATAL_ERROR ">>> Unknown build type ${BUILD_TYPE}.")
endif ()

if ((NOT (${LINK_TYPE} STREQUAL STATIC)) AND (NOT (${LINK_TYPE} STREQUAL SHARED)))
    message (FATAL_ERROR ">>> Unknown link type ${LINK_TYPE}.")
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
set (MODULE_DIR "${SRC_DIR}/module")
set (GENERATED_DIR "${SRC_DIR}/generated")

#set (CMAKE_CURRENT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${MODULE_DIR}")
#set (CMAKE_CURRENT_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/${WRAPPER_DIR}")

include (${SRC_DIR}/src_list.cmake)
include (${SRC_DIR}/module_list.cmake)

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
    set (CFLAGS "${CFLAGS} -Wall -ansi -O3 -U_FORTIFY_SOURCE -msse2")
    if (${BUILD_TYPE} STREQUAL DEBUG)
        set (CFLAGS "${CFLAGS} -g")
    endif (${BUILD_TYPE} STREQUAL DEBUG)
endif (UNIX OR MINGW)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set (LDFLAGS "${LDFLAGS} -mmacosx-version-min=10.5")
endif ()

# ${FRAMEWORK_SRC} esta' definido?
if (NOT FRAMEWORK_SRC)
    message (FATAL_ERROR "Error: FRAMEWORK_SRC not defined! Please do so in the file src/src_list.cmake!")
endif (NOT FRAMEWORK_SRC)

include_directories ("src")

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

find_package(Lua51 REQUIRED)
include_directories(${LUA_INCLUDE_DIR})
    
find_package(PythonLibs 2.7 REQUIRED)
include_directories (${PYTHON_INCLUDE_DIRS})

find_package(Boost 1.48.0 REQUIRED)
include_directories("${Boost_INCLUDE_DIRS}/boost/tr1/tr1")
include_directories(${Boost_INCLUDE_DIRS})

# Does this work everywhere?
find_package (SWIG REQUIRED)
include (cmake/UseSWIG.cmake)

# ${FRAMEWORK_SRC} esta' definido?
if (NOT MODULE_SRC)
    message (FATAL_ERROR "Error: MODULE_SRC not defined! Please do so in the file src/module_list.cmake!")
endif (NOT MODULE_SRC)

set_source_files_properties (${MODULE_SRC} PROPERTIES CPLUSPLUS ON)
set_source_files_properties (${MODULE_SRC} PROPERTIES SWIG_FLAGS "")

set (CMAKE_CURRENT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${MODULE_DIR}")
set (CMAKE_CURRENT_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/${GENERATED_DIR}")

set (GENERATED_SRC "")
swig_add_module (ugdk_lua lua ${MODULE_SRC} ${FRAMEWORK_SRC})
set (GENERATED_SRC ${GENERATED_SRC} ${swig_generated_sources})
swig_add_module (ugdk_python python ${MODULE_SRC} ${FRAMEWORK_SRC})
set (TEMP_ONLY_CXX)
foreach (it ${swig_generated_sources})
  if (${it} MATCHES ".cc$")
    set (TEMP_ONLY_CXX ${TEMP_ONLY_CXX} ${it})
  endif ()
endforeach (it)
set (GENERATED_SRC ${GENERATED_SRC} ${TEMP_ONLY_CXX})
unset (TEMP_ONLY_CXX)

# pre-processor: #define's
if (MSVC)
  foreach (d ${DEFS})
    set (CFLAGS_EXTRA "${CFLAGS_EXTRA} /D${d}")
  endforeach (d)
  
  set (CFLAGS_EXTRA "${CFLAGS_EXTRA} /D_CRT_SECURE_NO_WARNINGS /DBOOST_ALL_NO_LIB /W4 /wd4100 /wd4127 /wd4201 /wd4211 /wd4706 /arch:SSE2 /fp:fast")
  
else (MSVC)
  foreach (d ${DEFS})
    set (CFLAGS_EXTRA "${CFLAGS_EXTRA} -D${d}")
  endforeach (d)
endif  (MSVC)

set (PYTHON_FINAL_LIB ${PYTHON_LIBRARIES})
if (${BUILD_TYPE} STREQUAL DEBUG)
    set (CFLAGS_EXTRA "${CFLAGS_EXTRA} -DDEBUG")
	
	# Check if we have the debug library to link with on debug. If not, we'll just use the nondebug one.
	if (PYTHON_DEBUG_LIBRARY)
		set (PYTHON_FINAL_LIB ${PYTHON_DEBUG_LIBRARY})
	endif()
endif (${BUILD_TYPE} STREQUAL DEBUG)

include_directories ("externals/opengl")

if (${LINK_TYPE} STREQUAL STATIC)
    add_library (${FRAMEWORK_BUILDNAME} STATIC ${FRAMEWORK_SRC} ${GENERATED_SRC})
elseif (${LINK_TYPE} STREQUAL SHARED)
    add_library (${FRAMEWORK_BUILDNAME} SHARED ${FRAMEWORK_SRC} ${GENERATED_SRC})
endif (${LINK_TYPE} STREQUAL STATIC)

include (cmake/asteroids.cmake)

if (CMAKE_COMPILER_IS_GNUCXX)
	set (EXTRA_LIBRARIES m ${EXTRA_LIBRARIES})
endif (CMAKE_COMPILER_IS_GNUCXX)
if (MINGW)
	set (EXTRA_LIBRARIES mingw32 ${EXTRA_LIBRARIES})
endif (MINGW)
if (WIN32)
	set (EXTRA_LIBRARIES Ws2_32 ${EXTRA_LIBRARIES})
endif (WIN32)

set_target_properties (${FRAMEWORK_BUILDNAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})
set_target_properties (${FRAMEWORK_BUILDNAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${LIB_DIR})
if (WIN32)
	set_target_properties (${FRAMEWORK_BUILDNAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${LIB_DIR})
endif (WIN32)

target_link_libraries (${FRAMEWORK_BUILDNAME}
					   ${EXTRA_LIBRARIES}
					   ${SDL_LIBRARY}
					   ${SDLMIXER_LIBRARY}
					   ${SDLTTF_LIBRARY}
					   ${SDLIMAGE_LIBRARY}
					   ${OPENGL_LIBRARIES}
					   ${LUA_LIBRARIES}
					   ${PYTHON_FINAL_LIB})

set_target_properties (${FRAMEWORK_BUILDNAME} PROPERTIES LINK_FLAGS "${LDFLAGS}")
set_target_properties (${FRAMEWORK_BUILDNAME} PROPERTIES COMPILE_FLAGS "${CFLAGS} ${CFLAGS_EXTRA}")
set_target_properties (${FRAMEWORK_BUILDNAME} PROPERTIES PROJECT_NAME "${FRAMEWORK_NAME}")

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



