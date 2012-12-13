# Requires CMake 2.8 or newer.
cmake_minimum_required (VERSION 2.8)

project (ugdk CXX C)
set (FRAMEWORK_VERSION_MAJOR 0)
set (FRAMEWORK_VERSION_MINOR 5)
set (FRAMEWORK_VERSION_PATCH 0)
set (FRAMEWORK_BIGVERSION ${FRAMEWORK_VERSION_MAJOR}.${FRAMEWORK_VERSION_MINOR})
set (FRAMEWORK_VERSION ${FRAMEWORK_BIGVERSION}.${FRAMEWORK_VERSION_PATCH})
set (GROUP_NAME "USPGameDev Team")
set (HOME_URL "http://uspgamedev.org/")
set (CONTACT_MAIL "admin@uspgamedev.org")
set (UGDK_INSTALL_LOCATION share/ugdk)

message ("=== UGDK version ${FRAMEWORK_VERSION} ===")

# BUILD_SHARED_LIBS is a standard CMake variable, but we declare it here to
# make it prominent in the GUI.
option(BUILD_SHARED_LIBS "Build shared libraries (DLLs)." OFF)

option(UGDK_BUILD_TESTS "Set to ON to enable building tests." ON)
option(UGDK_CREATE_BINDINGS "Set to ON to generate bindings for supported script languages." ON)
option(UGDK_LUA_BINDINGS "Set to ON to enable Lua as one of the languages to create bindings." ON)
option(UGDK_PYTHON_BINDINGS "Set to ON to enable Python as one of the languages to create bindings." ON)

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

set(FRAMEWORK_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_DIR})

CONFIGURE_FILE(
    "doxystuff/Doxyfile.in"
    "doxystuff/Doxyfile"
)

#set (CMAKE_CURRENT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${MODULE_DIR}")
#set (CMAKE_CURRENT_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/${WRAPPER_DIR}")

include (${SRC_DIR}/src_list.cmake)
include (${SRC_DIR}/module_list.cmake)
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
    add_definitions(-Wall -ansi -U_FORTIFY_SOURCE -msse2 -std=c++0x)
elseif (MSVC) 
    add_definitions(/D_CRT_SECURE_NO_WARNINGS /D_VARIADIC_MAX=10 /W4 /wd4100 /wd4127 /wd4201 /wd4211 /wd4250 /wd4706 /fp:fast /MP)
endif ()
if(WIN32)
    add_definitions(/DWIN32)
endif()

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

##########################################
# Package Configuration

set (CPACK_GENERATOR DEB)
set (CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_CURRENT_SOURCE_DIR}/README.txt)
set (CPACK_PACKAGE_NAME libugdk)
set (CPACK_PACKAGE_VENDOR ${GROUP_NAME})
set (CPACK_PACKAGE_CONTACT "${GROUP_NAME} <${CONTACT_MAIL}>")
set (CPACK_PACKAGE_INSTALL_REGISTRY_KEY "ugdk")
set (CPACK_PACKAGE_VERSION_MAJOR ${FRAMEWORK_VERSION_MAJOR})
set (CPACK_PACKAGE_VERSION_MINOR ${FRAMEWORK_VERSION_MINOR})
set (CPACK_PACKAGE_VERSION_PATCH ${FRAMEWORK_VERSION_PATCH})
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "USP Game Development Kit, a framework for developing 2d games.")
set (CPACK_DEBIAN_PACKAGE_NAME "libugdk${FRAMEWORK_BIGVERSION}")
set (CPACK_DEBIAN_PACKAGE_HOMEPAGE ${HOME_URL})
set (CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.2), libgcc1 (>= 1:4.1.1), libstdc++6 (>= 4.4.0), libsdl1.2-dev, libsdl-image1.2, libsdl-mixer1.2-dev, libsdl-ttf2.0-0, lua5.1, python (>=2.6)")
set (DEBIAN_PACKAGE_BUILDS_DEPENDS "libc6 (>= 2.2), libgcc1 (>= 1:4.1.1), libstdc++6 (>= 4.4.0), libsdl1.2-dev, libsdl-image1.2-dev, libsdl-mixer1.2-dev, libsdl-ttf2.0-dev, swig2.0, lua5.1-dev, python-dev (>= 2.6), cmake")
set (CPACK_NSIS_CONTACT ${CONTACT_MAIL})
set (CPACK_NSIS_DISPLAY_NAME "USP Game Development Kit")
set (CPACK_NSIS_PACKAGE_HOMEPAGE ${HOME_URL})
set (CPACK_RPM_PACKAGE_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
set (CPACK_RPM_PACKAGE_URL ${HOME_URL})

include (CPack)

#
##########################################
set(UGDK_LANGUAGES_LIST "")
if (UGDK_CREATE_BINDINGS)
    if (UGDK_LUA_BINDINGS)
        list(APPEND UGDK_LANGUAGES_LIST Lua)
    endif()
    if (UGDK_PYTHON_BINDINGS)
        list(APPEND UGDK_LANGUAGES_LIST Python)
    endif()
    ugdk_setup_package(SWIG)
endif (UGDK_CREATE_BINDINGS)

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

add_library (libugdk ${UGDK_SRC} ${GENERATED_SRC})

if (CMAKE_COMPILER_IS_GNUCXX)
    set (EXTRA_LIBRARIES m ${EXTRA_LIBRARIES})
endif (CMAKE_COMPILER_IS_GNUCXX)
if (MINGW)
    set (EXTRA_LIBRARIES mingw32 ${EXTRA_LIBRARIES})
endif (MINGW)
if (WIN32)
    set (EXTRA_LIBRARIES Ws2_32 ${EXTRA_LIBRARIES})
endif (WIN32)


# We could use LINK_PUBLIC here, but it's a new feature in cmake 2.8.7, so we'd rather not require the newest version.

target_link_libraries (libugdk
                       ${EXTRA_LIBRARIES}
                       ${SDL_LIBRARY}
                       ${SDLMIXER_LIBRARY}
                       ${SDLTTF_LIBRARY}
                       ${SDLIMAGE_LIBRARY}
                       ${OPENGL_LIBRARIES}
                       ${UGDK_LIBRARIES})

target_link_libraries (libugdk LINK_INTERFACE_LIBRARIES 
                       ${EXTRA_LIBRARIES}
                       ${SDL_LIBRARY}
                       ${SDLMIXER_LIBRARY}
                       ${SDLTTF_LIBRARY}
                       ${SDLIMAGE_LIBRARY}
                       ${OPENGL_LIBRARIES}
                       ${UGDK_LIBRARIES})
                      
set_target_properties (libugdk PROPERTIES 
    LINK_FLAGS "${LDFLAGS}"
    PROJECT_NAME UGDK
    OUTPUT_NAME ugdk${FRAMEWORK_BIGVERSION}
    OUTPUT_NAME_DEBUG ugdk${FRAMEWORK_BIGVERSION}-dbg
    SOVERSION ${FRAMEWORK_VERSION_PATCH}
    VERSION ${FRAMEWORK_VERSION}
)

if(UGDK_BUILD_TESTS)
    add_subdirectory (test)
endif(UGDK_BUILD_TESTS)

if (UNIX AND PYTHONLIBS_FOUND)
    set (UGDK_INSTALL_LOCATION_DEFINE "#define UGDK_INSTALL_LOCATION \"/usr/${UGDK_INSTALL_LOCATION}\"")
endif (UNIX AND PYTHONLIBS_FOUND)

set(UGDK_EXTERNAL_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src;${SDL_INCLUDE_DIR};${SDLTTF_INCLUDE_DIR} CACHE STRING "UGDK include directories" FORCE)

CONFIGURE_FILE(
    "${SRC_DIR}/ugdk/config/config.h.in"
    "${SRC_DIR}/ugdk/config/config.h"
)

install(TARGETS libugdk
    DESTINATION lib
#    CONFIGURATIONS Release
)

install(DIRECTORY src/ugdk src/pyramidworks DESTINATION include/ugdk${FRAMEWORK_BIGVERSION} FILES_MATCHING PATTERN "*.h" PATTERN "*.th" PATTERN "*.tcc")

if (PYTHONLIBS_FOUND AND PYTHON_INCLUDE_DIR AND EXISTS "${PYTHON_INCLUDE_DIR}/patchlevel.h")
    install(DIRECTORY src/generated/ 
            DESTINATION ${UGDK_INSTALL_LOCATION}/${FRAMEWORK_BIGVERSION}/python
            FILES_MATCHING PATTERN "*.py")
endif (PYTHONLIBS_FOUND AND PYTHON_INCLUDE_DIR AND EXISTS "${PYTHON_INCLUDE_DIR}/patchlevel.h")

