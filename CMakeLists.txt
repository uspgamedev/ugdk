cmake_minimum_required (VERSION 2.8.11)
# 2.8.11 for target_include_directories

set( UGDK_ANDROID_DIR "${CMAKE_SOURCE_DIR}/android" )
set( LIBRARY_OUTPUT_PATH_ROOT "${UGDK_ANDROID_DIR}" CACHE PATH "root for library output, set this to change where android libs are installed to" FORCE)

project (ugdk CXX C)
set (UGDK_VERSION_MAJOR 0)
set (UGDK_VERSION_MINOR 5)
set (UGDK_VERSION_PATCH 0)
set (UGDK_BIGVERSION ${UGDK_VERSION_MAJOR}.${UGDK_VERSION_MINOR})
set (UGDK_VERSION ${UGDK_BIGVERSION}.${UGDK_VERSION_PATCH})
set (GROUP_NAME "USPGameDev Team")
set (HOME_URL "http://uspgamedev.org/")
set (CONTACT_MAIL "contato@uspgamedev.org")
set (UGDK_INSTALL_LOCATION_RELATIVE share/ugdk)

message ("=== UGDK version ${UGDK_VERSION} ===")

IF(MSVC AND (CMAKE_SIZEOF_VOID_P EQUAL 8))
    set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS ON)
ENDIF()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")

# BUILD_SHARED_LIBS is a standard CMake variable, but we declare it here to
# make it prominent in the GUI.
option(BUILD_SHARED_LIBS "Build shared libraries (DLLs)." OFF)

option(UGDK_BUILD_TESTS          "Set to ON to enable building tests." ON)
option(UGDK_CREATE_BINDINGS      "Set to ON to generate bindings for supported script languages." ON)
option(UGDK_LUA_BINDINGS         "Set to ON to enable Lua as one of the languages to create bindings." ON)
option(UGDK_PYTHON_BINDINGS      "Set to ON to enable Python as one of the languages to create bindings." ON)

if(NOT ${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
    # Don't polute the git repository with the generated files for out-of-source builds
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/.gitignore "*")
endif()

if(ANDROID)
    if(${ANDROID_NATIVE_API_LEVEL} LESS 9)
        message(FATAL_ERROR "ANDROID_NATIVE_API_LEVEL must be at least 9")
    endif()
else()
    macro(find_host_package)
        find_package(${ARGN})
    endmacro(find_host_package)
endif()

CONFIGURE_FILE(
    "doxystuff/Doxyfile.in"
    "doxystuff/Doxyfile"
)

set (UGDK_LINK_FLAGS)
include (cmake/MacOptions.cmake)

include (src/src_list.cmake)
include (src/module_list.cmake)
include (src/lua_src_list.cmake)
include (src/py_src_list.cmake)

if (NOT UGDK_SRC)
    message (FATAL_ERROR "Error: UGDK_SRC not defined! Please do so in the file src/src_list.cmake!")
endif (NOT UGDK_SRC)

# Create the targets for all the dependencies.
add_subdirectory(externals)

# We can't create a target for a header-only library, so we have to do this...
# TODO: CMake 3.0 will add INTERFACE to add_library (http://www.kdab.com/modern-cmake-with-qt-and-boost/)
set(GLM_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/externals/glm-0.9.5.2")
#include_directories(SYSTEM ${GLM_LOCATION}) ## FIXME

# CPack configuration.
include(cmake/Package.cmake)

#########################################################
# Bindings configuration
set(UGDK_LIBRARIES       "")
set(UGDK_LIBRARIES_DEBUG "")
set(UGDK_LANGUAGES_LIST  "")
if (UGDK_CREATE_BINDINGS)
    if (UGDK_LUA_BINDINGS)
        list(APPEND UGDK_LANGUAGES_LIST Lua)
    endif()
    if (UGDK_PYTHON_BINDINGS)
        list(APPEND UGDK_LANGUAGES_LIST Python)
    endif()
    include(cmake/CreateBindings.cmake)
endif (UGDK_CREATE_BINDINGS)
#########################################################

if(NOT ANDROID)
    set (LIB_DIR lib)
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
endif(NOT ANDROID)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
if (UNIX OR MINGW)
    add_definitions(-ansi -U_FORTIFY_SOURCE -std=c++0x)
    #add_definitions(-msse2)
    add_definitions(-Wall
        # TODO: would like these but they produce overwhelming amounts of warnings
        #-Wshadow                       # GLM causes many warnings with this
        #-Wextra                        # implies -Wunused-parameter
        -Wmissing-field-initializers
        #-Wswitch-default               # Currently ignored.
        #-Wconversion                   # GLM causes many warnings with this
        #-Wzero-as-null-pointer-constant
        -Wtype-limits
        -Wsign-compare
        -Wignored-qualifiers
        -Wuninitialized
        -Winit-self

        # And we ignore these warnings...
        -Wno-switch-default # SDL includes causes these. Awaiting CMake 2.8.12 to ignore these properly. TODO
    )
        
    set_source_files_properties(${GENERATED_SRC}
        PROPERTIES 
            COMPILE_FLAGS
                "-fpermissive -Wno-unused-value")
            
endif()

# Disable some warnings for Clang.
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_definitions(
        -Wno-self-assign
    )
    if(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS "3.4")
        add_definitions(
            -Wno-deprecated-register
        )
    endif()
endif ()

if (MSVC) 
    add_definitions(/D_CRT_SECURE_NO_WARNINGS /D_VARIADIC_MAX=10 /W4 /wd4100 /wd4127 /wd4201 /wd4211 /wd4250 /wd4706 /fp:fast /MP)
endif ()

# =============================
add_library (libugdk ${UGDK_SRC} ${GENERATED_SRC})

if(${CMAKE_VERSION} VERSION_LESS "2.8.12")
    target_include_directories(libugdk PUBLIC ${GLM_LOCATION})
else()
    target_include_directories(libugdk SYSTEM PUBLIC ${GLM_LOCATION})
endif()

target_include_directories(libugdk
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src" # TODO: should be /include
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/src" # TODO: should be /include
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

if (MINGW)
    target_link_libraries(libugdk LINK_PUBLIC mingw32) # TODO: do we really need this?
endif (MINGW)

if (WIN32)
    # utf8<->ucs4 conversion uses a networking function
    target_link_libraries(libugdk LINK_PUBLIC Ws2_32)
endif (WIN32)

# Libraries for the scripts.
target_link_libraries(libugdk LINK_PUBLIC ${UGDK_LIBRARIES})

if(NOT ANDROID) # Glew is not necessary for android builds.
    target_link_libraries(libugdk LINK_PUBLIC glew)
endif()

# SDL-mixer must come before SDL itself, or else we may have linking errors.
target_link_libraries(libugdk LINK_PUBLIC SDL2-mixer SDL2-image SDL2 freetype-gl libjson)

set_target_properties(libugdk PROPERTIES 
    LINK_FLAGS "${LDFLAGS}"
    PROJECT_NAME UGDK
    OUTPUT_NAME ugdk${UGDK_BIGVERSION}
    OUTPUT_NAME_DEBUG ugdk${UGDK_BIGVERSION}-dbg
)

if(ANDROID)
    target_compile_definitions(libugdk PUBLIC 
        UGDK_USING_GLES
    )
endif(ANDROID)


if(ANDROID)
    macro(add_ugdk_executable NAME)
        add_library(${NAME} SHARED
            ${ARGN}
            ${ugdk_SOURCE_DIR}/src/android/main.cc
        )
        target_link_libraries(${NAME} libugdk)
    endmacro(add_ugdk_executable)
    macro(add_ugdk_android_project NAME BUNDLE_NAME)
        set(UGDK_ANDROIDPROJECT_NAME ${NAME})
        set(UGDK_ANDROIDPROJECT_BUNDLE_NAME ${BUNDLE_NAME})
        set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/android-project/libs/armeabi-v7a" 
            CACHE PATH "root for library output, set this to change where android libs are installed to" FORCE)
        
        file(GLOB_RECURSE ANDROID_TEMPLATE_SOURCE
            RELATIVE ${ugdk_SOURCE_DIR}/android/template
            "android/template/*")
        file(COPY ${ugdk_SOURCE_DIR}/android/skeleton/
            DESTINATION ${CMAKE_BINARY_DIR}/android-project)
            
        foreach(it ${ANDROID_TEMPLATE_SOURCE})
            configure_file(${ugdk_SOURCE_DIR}/android/template/${it}
                           ${CMAKE_BINARY_DIR}/android-project/${it})
        endforeach()
    endmacro(add_ugdk_android_project)
else(ANDROID)
    macro(add_ugdk_executable NAME)
        add_executable(${NAME} ${ARGN})
        target_link_libraries(${NAME} libugdk)
    endmacro(add_ugdk_executable)
    macro(add_ugdk_android_project NAME BUNDLE_NAME)
    endmacro(add_ugdk_android_project)
endif(ANDROID)

if (UNIX AND PYTHONLIBS_FOUND)
    set(UGDK_INSTALL_LOCATION "${CMAKE_INSTALL_PREFIX}/${UGDK_INSTALL_LOCATION_RELATIVE}")
endif (UNIX AND PYTHONLIBS_FOUND)

CONFIGURE_FILE(
    "cmake/config.h.in"
    "src/ugdk/system/config.h"
) 

install(TARGETS libugdk
    DESTINATION lib
#    CONFIGURATIONS Release
)

install(DIRECTORY src/ugdk src/pyramidworks # TODO: change to only 'include/'
        DESTINATION include/ugdk${UGDK_BIGVERSION} 
        FILES_MATCHING PATTERN "*.h" PATTERN "*.th" PATTERN "*.tcc")

if (PYTHONLIBS_FOUND AND PYTHON_INCLUDE_DIR AND EXISTS "${PYTHON_INCLUDE_DIR}/patchlevel.h")
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/src/generated/ 
            DESTINATION ${UGDK_INSTALL_LOCATION_RELATIVE}/${UGDK_BIGVERSION}/python
            FILES_MATCHING PATTERN "*.py")
endif (PYTHONLIBS_FOUND AND PYTHON_INCLUDE_DIR AND EXISTS "${PYTHON_INCLUDE_DIR}/patchlevel.h")

# Tests and examples
if(UGDK_BUILD_TESTS)
    add_subdirectory(test)
endif(UGDK_BUILD_TESTS)

