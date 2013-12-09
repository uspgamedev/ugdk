cmake_minimum_required (VERSION 2.8.7)

project (ugdk CXX C)
set (UGDK_VERSION_MAJOR 0)
set (UGDK_VERSION_MINOR 5)
set (UGDK_VERSION_PATCH 0)
set (UGDK_BIGVERSION ${UGDK_VERSION_MAJOR}.${UGDK_VERSION_MINOR})
set (UGDK_VERSION ${UGDK_BIGVERSION}.${UGDK_VERSION_PATCH})
set (GROUP_NAME "USPGameDev Team")
set (HOME_URL "http://uspgamedev.org/")
set (CONTACT_MAIL "contato@uspgamedev.org")
set (UGDK_INSTALL_LOCATION share/ugdk)

message ("=== UGDK version ${UGDK_VERSION} ===")

IF(MSVC AND (CMAKE_SIZEOF_VOID_P EQUAL 8))
    set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS ON)
ENDIF()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")

# BUILD_SHARED_LIBS is a standard CMake variable, but we declare it here to
# make it prominent in the GUI.
option(BUILD_SHARED_LIBS "Build shared libraries (DLLs)." OFF)

option(UGDK_BUILD_TESTS          "Set to ON to enable building tests." ON)
option(UGDK_BUILD_EXAMPLES       "Set to ON to enable building examples." ON)
option(UGDK_CREATE_BINDINGS      "Set to ON to generate bindings for supported script languages." ON)
option(UGDK_LUA_BINDINGS         "Set to ON to enable Lua as one of the languages to create bindings." ON)
option(UGDK_PYTHON_BINDINGS      "Set to ON to enable Python as one of the languages to create bindings." ON)
option(UGDK_ANDROID              "Set to ON to build for Android." OFF)

option(UGDK_INTERNAL_GLEW        "Set to ON to use an internal GLEW instead of the system one." OFF)
option(UGDK_INTERNAL_SDL2        "Set to ON to use an internal SDL2 instead of the system one." OFF)
option(UGDK_INTERNAL_SDL2_IMAGE   "Set to ON to use an internal SDL2_image instead of the system one." OFF)
option(UGDK_INTERNAL_SDL2_MIXER   "Set to ON to use an internal SDL2_mixer instead of the system one." OFF)

if(NOT ${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
    # Don't polute the git repository with the generated files for out-of-source builds
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/.gitignore "*")
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET (CHECKMAC TRUE)
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

set(UGDK_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_DIR})

CONFIGURE_FILE(
    "doxystuff/Doxyfile.in"
    "doxystuff/Doxyfile"
)

include (${SRC_DIR}/src_list.cmake)
include (${SRC_DIR}/module_list.cmake)
include (${SRC_DIR}/lua_src_list.cmake)
include (${SRC_DIR}/py_src_list.cmake)

# LDFLAGS: flags que voce queira enviar ao ligador
set (LDFLAGS "" CACHE STRING "flags enviadas ao linker")
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    LIST(APPEND LDFLAGS "-mmacosx-version-min=10.5")
endif ()

if (NOT UGDK_SRC)
    message (FATAL_ERROR "Error: UGDK_SRC not defined! Please do so in the file src/src_list.cmake!")
endif (NOT UGDK_SRC)

set(GLM_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/externals/glm-0.9.4.2")

include(cmake/ugdk.cmake)

if(NOT ANDROID)
    find_package(SDL2 QUIET)
    if(NOT SDL2_FOUND)
        message("-- SDL2 not found, forcing internal.")
        set(UGDK_INTERNAL_SDL2 ON CACHE BOOL "Set to ON to use an internal SDL2 instead of the system one." FORCE)
    endif(NOT SDL2_FOUND)

    find_package(SDL2_image QUIET)
    if(NOT SDL2_IMAGE_FOUND)
        message("-- SDL2_image not found, forcing internal.")
        set(UGDK_INTERNAL_SDL2_IMAGE ON CACHE BOOL "Set to ON to use an internal SDL2_image instead of the system one." FORCE)
    endif(NOT SDL2_IMAGE_FOUND)

    find_package(SDL2_mixer QUIET)
    if(NOT SDL2_MIXER_FOUND)
        message("-- SDL2_mixer not found, forcing internal.")
        set(UGDK_INTERNAL_SDL2_MIXER ON CACHE BOOL "Set to ON to use an internal SDL2_mixer instead of the system one." FORCE)
    endif(NOT SDL2_MIXER_FOUND)

    ########################
    ## SDL2
    ########################
    if(UGDK_INTERNAL_SDL2)
        set(SDL_SHARED OFF CACHE BOOL "Build a shared version of the library")
        set(DIRECTX    OFF CACHE BOOL "Use DirectX for Windows audio/video" FORCE)
        set(RENDER_D3D OFF CACHE BOOL "Enable the Direct3D render driver" FORCE)
        add_subdirectory(externals/sdl2 externals/sdl2-build)

        set(UGDK_DEPENDENCIES_SDL2_LIBRARIES    SDL2-static)
        set(UGDK_DEPENDENCIES_SDL2_INCLUDE_DIRS ${SDL2_SOURCE_DIR}/include)
    else(UGDK_INTERNAL_SDL2)
        set(UGDK_DEPENDENCIES_SDL2_LIBRARIES    ${SDL2_LIBRARIES})
        set(UGDK_DEPENDENCIES_SDL2_INCLUDE_DIRS ${SDL2_INCLUDE_DIRS})
    endif(UGDK_INTERNAL_SDL2)
    include_directories(SYSTEM ${UGDK_DEPENDENCIES_SDL2_INCLUDE_DIRS})

    ########################
    ## SDL2_IMAGE
    ########################
    if(UGDK_INTERNAL_SDL2_IMAGE)
        add_subdirectory(externals/sdl2-image)
        set(UGDK_DEPENDENCIES_SDL2_IMAGE_LIBRARIES    SDL2-image)
        set(UGDK_DEPENDENCIES_SDL2_IMAGE_INCLUDE_DIRS ${SDL2-image_SOURCE_DIR}/include)

    else(UGDK_INTERNAL_SDL2_IMAGE)
        set(UGDK_DEPENDENCIES_SDL2_IMAGE_LIBRARIES    ${SDL2_IMAGE_LIBRARIES})
        set(UGDK_DEPENDENCIES_SDL2_IMAGE_INCLUDE_DIRS ${SDL2_IMAGE_INCLUDE_DIRS})
    endif(UGDK_INTERNAL_SDL2_IMAGE)
    include_directories(SYSTEM ${UGDK_DEPENDENCIES_SDL2_IMAGE_INCLUDE_DIRS})

    ########################
    ## SDL2_MIXER
    ########################
    if(UGDK_INTERNAL_SDL2_MIXER)
        add_subdirectory(externals/sdl2-mixer)
        set(UGDK_DEPENDENCIES_SDL2_MIXER_LIBRARIES    SDL2-mixer)
        set(UGDK_DEPENDENCIES_SDL2_MIXER_INCLUDE_DIRS ${SDL2-mixer_SOURCE_DIR}/include)

    else(UGDK_INTERNAL_SDL2_MIXER)
        set(UGDK_DEPENDENCIES_SDL2_MIXER_LIBRARIES    ${SDL2_MIXER_LIBRARIES})
        set(UGDK_DEPENDENCIES_SDL2_MIXER_INCLUDE_DIRS ${SDL2_MIXER_INCLUDE_DIRS})
    endif(UGDK_INTERNAL_SDL2_MIXER)
    include_directories(SYSTEM ${UGDK_DEPENDENCIES_SDL2_MIXER_INCLUDE_DIRS})

    # We set the UGDK_EXTERNALS_SDL_INCLUDE_DIRS variable for users building UGDK with add_subdirectory
    set(UGDK_EXTERNALS_SDL_INCLUDE_DIRS ${UGDK_DEPENDENCIES_SDL2_INCLUDE_DIRS};${UGDK_DEPENDENCIES_SDL2_MIXER_INCLUDE_DIRS})

    # Other libs
    find_package(OpenGL REQUIRED)
    include_directories(${OPENGL_INCLUDE_DIR})

    find_package(GLEW 1.5 QUIET)
    if(NOT GLEW_FOUND)
        message("-- GLEW not found, forcing internal.")
        set(UGDK_INTERNAL_GLEW ON CACHE BOOL "Set to ON to use the internal GLEW." FORCE)
    endif(NOT GLEW_FOUND)

    if(UGDK_INTERNAL_GLEW)
        add_definitions(-DGLEW_STATIC)
        add_subdirectory(externals/glew)
        set(GLEW_INCLUDE_DIRS ${GLEW_INCLUDE_DIR})
        set(GLEW_LIBRARIES ${GLEW_LIBRARY})
    endif(UGDK_INTERNAL_GLEW)
    include_directories(${GLEW_INCLUDE_DIRS})

    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/externals/freetype-glpp/CMakeLists.txt")
        find_package(Git)
        if(GIT_FOUND)
            execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init
                            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
        else(GIT_FOUND)
            message(FATAL_ERROR "Git submodule 'externals/freetype-glpp' not initialized and git not found.")
        endif(GIT_FOUND)
    endif()

    add_subdirectory(externals/freetype-glpp)
    include_directories("externals/freetype-glpp/include")
    set(FREETYPE_GLPP_LIBRARIES freetype-gl++)
else()
    include_directories(SYSTEM externals/sdl2/include)
    include_directories(SYSTEM externals/sdl2-image/include)
    include_directories(SYSTEM externals/sdl2-mixer/include)
endif(NOT ANDROID)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/${SRC_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR}/${SRC_DIR})

include_directories(SYSTEM ${GLM_LOCATION})

##########################################
# Package Configuration

set (CPACK_GENERATOR DEB)
set (CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_CURRENT_SOURCE_DIR}/README.txt)
set (CPACK_PACKAGE_NAME libugdk)
set (CPACK_PACKAGE_VENDOR ${GROUP_NAME})
set (CPACK_PACKAGE_CONTACT "${GROUP_NAME} <${CONTACT_MAIL}>")
set (CPACK_PACKAGE_INSTALL_REGISTRY_KEY "ugdk")
set (CPACK_PACKAGE_VERSION_MAJOR ${UGDK_VERSION_MAJOR})
set (CPACK_PACKAGE_VERSION_MINOR ${UGDK_VERSION_MINOR})
set (CPACK_PACKAGE_VERSION_PATCH ${UGDK_VERSION_PATCH})
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "USP Game Development Kit, a framework for developing 2d games.")
set (CPACK_DEBIAN_PACKAGE_NAME "libugdk${UGDK_BIGVERSION}")
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

# Bit hacky way so all the output files are placed on the ${LIB_DIR} folder.
# Code found on http://stackoverflow.com/questions/7747857/in-cmake-how-do-i-work-around-the-debug-and-release-directories-visual-studio-2

# First for the generic no-config case (e.g. with mingw)
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "" )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIB_DIR} )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR} )
# Second, for multi-config builds (e.g. msvc)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "" )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${LIB_DIR} )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${LIB_DIR} )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )



set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
if (UNIX OR MINGW)
    add_definitions(-ansi -U_FORTIFY_SOURCE -std=c++0x -DUGDK_USING_VARIADIC)
    add_definitions(-Wall
        # TODO: would like these but they produce overwhelming amounts of warnings
        #-Wshadow                       # GLM causes many warnings with this
        #-Wextra                        # implies -Wunused-parameter
        -Wmissing-field-initializers
        -Wswitch-default
        #-Wconversion                   # GLM causes many warnings with this
        #-Wzero-as-null-pointer-constant
        -Wtype-limits
        -Wsign-compare
        -Wignored-qualifiers
        -Wuninitialized
        -Winit-self)
        
    set_source_files_properties(${GENERATED_SRC}
        PROPERTIES 
            COMPILE_FLAGS
                "-fpermissive -Wno-unused-value")
            
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_definitions(
        -Wno-self-assign
    )
endif ()

if (MSVC) 
    add_definitions(/D_CRT_SECURE_NO_WARNINGS /D_VARIADIC_MAX=10 /W4 /wd4100 /wd4127 /wd4201 /wd4211 /wd4250 /wd4706 /fp:fast /MP)
    if (NOT MSVC_VERSION LESS 1800)
        add_definitions(/DUGDK_USING_VARIADIC)
    endif()
endif ()

# =============================
add_library (libugdk ${UGDK_SRC} ${GENERATED_SRC})

if (MINGW)
    set (EXTRA_LIBRARIES mingw32 ${EXTRA_LIBRARIES})
endif (MINGW)
if (WIN32)
    # utf8<->ucs4 conversion uses a networking function
    set (EXTRA_LIBRARIES Ws2_32 ${EXTRA_LIBRARIES}) 
endif (WIN32)


target_link_libraries (libugdk LINK_PUBLIC
                        ${EXTRA_LIBRARIES}
                        # SDL-mixer must come before SDL itself, or else we may have linking errors.
                        ${UGDK_DEPENDENCIES_SDL2_MIXER_LIBRARIES}
                        ${UGDK_DEPENDENCIES_SDL2_IMAGE_LIBRARIES}
                        ${UGDK_DEPENDENCIES_SDL2_LIBRARIES}
                        ${GLEW_LIBRARIES}
                        ${OPENGL_LIBRARIES}
                        ${FREETYPE_GLPP_LIBRARIES}
                        ${UGDK_LIBRARIES})
                      
set_target_properties (libugdk PROPERTIES 
    LINK_FLAGS "${LDFLAGS}"
    PROJECT_NAME UGDK
    OUTPUT_NAME ugdk${UGDK_BIGVERSION}
    OUTPUT_NAME_DEBUG ugdk${UGDK_BIGVERSION}-dbg
    SOVERSION ${UGDK_VERSION_PATCH}
    VERSION ${UGDK_VERSION}
)

if(UGDK_BUILD_TESTS)
    add_subdirectory (test)
endif(UGDK_BUILD_TESTS)

if (UNIX AND PYTHONLIBS_FOUND)
    set (UGDK_INSTALL_LOCATION_DEFINE "#define UGDK_INSTALL_LOCATION \"/usr/${UGDK_INSTALL_LOCATION}\"")
endif (UNIX AND PYTHONLIBS_FOUND)

set(UGDK_EXTERNAL_INCLUDE_DIRS 
    ${PROJECT_SOURCE_DIR}/${SRC_DIR};
    ${CMAKE_CURRENT_BINARY_DIR}/${SRC_DIR};
    ${UGDK_EXTERNALS_SDL_INCLUDE_DIRS};
    ${GLM_LOCATION}
    CACHE STRING "UGDK include directories" FORCE)

CONFIGURE_FILE(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${SRC_DIR}/ugdk/system/config.h"
)

install(TARGETS libugdk
    DESTINATION lib
#    CONFIGURATIONS Release
)

install(DIRECTORY src/ugdk src/pyramidworks DESTINATION include/ugdk${UGDK_BIGVERSION} FILES_MATCHING PATTERN "*.h" PATTERN "*.th" PATTERN "*.tcc")

if (PYTHONLIBS_FOUND AND PYTHON_INCLUDE_DIR AND EXISTS "${PYTHON_INCLUDE_DIR}/patchlevel.h")
    install(DIRECTORY src/generated/ 
            DESTINATION ${UGDK_INSTALL_LOCATION}/${UGDK_BIGVERSION}/python
            FILES_MATCHING PATTERN "*.py")
endif (PYTHONLIBS_FOUND AND PYTHON_INCLUDE_DIR AND EXISTS "${PYTHON_INCLUDE_DIR}/patchlevel.h")

# Examples
if(UGDK_BUILD_EXAMPLES)
    add_subdirectory(examples EXCLUDE_FROM_ALL)
endif(UGDK_BUILD_EXAMPLES)

