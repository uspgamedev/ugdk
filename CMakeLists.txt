cmake_minimum_required (VERSION 3.0.0)
# 2.8.11 for target_include_directories

include(cmake/Compatibility.cmake)

########################################

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

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")

# BUILD_SHARED_LIBS is a standard CMake variable, but we declare it here to
# make it prominent in the GUI.
option(BUILD_SHARED_LIBS "Build shared libraries (DLLs)." OFF)

option(UGDK_3D_ENABLED           "Set to ON to enable the 3D module - Ogre rendering + Bullet physics." OFF)
option(UGDK_BUILD_TESTS          "Set to ON to enable building tests." ON)
option(UGDK_CREATE_BINDINGS      "Set to ON to generate bindings for supported script languages." ON)
option(UGDK_LUA_BINDINGS         "Set to ON to enable Lua as one of the languages to create bindings." ON)
option(UGDK_PYTHON_BINDINGS      "Set to ON to enable Python as one of the languages to create bindings." ON)

message(STATUS "3D Enabled: ${UGDK_3D_ENABLED}")

if (UGDK_3D_ENABLED)
    set(UGDK_GRAPHIC_MODULE 3d)
else()
    set(UGDK_GRAPHIC_MODULE 2d)
endif()
set(UGDK_LIBRARY_NAME ugdk${UGDK_BIGVERSION}-${UGDK_GRAPHIC_MODULE})

########################################

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

#################

macro(load_source_for_module NAME)
    include(modules/${NAME}/generated_src_list.cmake)

    list(APPEND UGDK_SRC ${GENERATED_SRC})
    list(APPEND UGDK_HEADERS ${GENERATED_HEADERS})

    unset(GENERATED_SRC)
    unset(GENERATED_HEADERS)
endmacro(load_source_for_module)

set(UGDK_SRC)
set(UGDK_HEADERS)

load_source_for_module(ugdk-core)
load_source_for_module(ugdk-${UGDK_GRAPHIC_MODULE})

# Create the targets for all the dependencies.
add_subdirectory(externals)

# We can't create a target for a header-only library, so we have to do this...
# TODO: CMake 3.0 will add INTERFACE to add_library (http://www.kdab.com/modern-cmake-with-qt-and-boost/)
set(GLM_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/externals/glm")
#include_directories(SYSTEM ${GLM_LOCATION}) ## FIXME

# CPack configuration.
include(cmake/Package.cmake)

#########################################################
# Bindings configuration
set(UGDK_LIBRARIES)
set(UGDK_LIBRARIES_DEBUG)
set(UGDK_LANGUAGES_LIST)

if (UGDK_CREATE_BINDINGS)
    load_source_for_module(ugdk-script)
    
    set(UGDK_MODULE_SRC)
    include(src/swig_module_list.cmake)
    include(src/swig_${UGDK_GRAPHIC_MODULE}_module_list.cmake)
    list(APPEND UGDK_MODULE_SRC ${UGDK_MODULE_${UGDK_GRAPHIC_MODULE}_SRC})
    
    if (UGDK_LUA_BINDINGS)
        list(APPEND UGDK_LANGUAGES_LIST Lua)
        include(cmake/languages/Lua.cmake)
    endif()
    if (UGDK_PYTHON_BINDINGS)
        include(cmake/languages/Python.cmake)
        list(APPEND UGDK_LANGUAGES_LIST Python)
    endif()
    
    include(cmake/CreateBindings.cmake)
    create_modules(UGDK_LANGUAGES_LIST UGDK_MODULE_SRC "UGDK" GENERATED_SRC)
    set(UGDK_SWIG_ENABLED True)
    
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

if (UNIX OR MINGW)
    add_definitions(-ansi -U_FORTIFY_SOURCE -std=c++11)
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
    add_definitions(/D_CRT_SECURE_NO_WARNINGS /D_VARIADIC_MAX=10 /W4 /wd4100 /wd4127 /wd4201 /wd4211 /wd4250 /wd4706 /wd4503 /fp:fast /MP)
endif ()

# =============================
add_library(libugdk ${UGDK_SRC} ${UGDK_HEADERS} ${GENERATED_SRC})

# Generated config
target_include_directories(libugdk PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/include")

target_include_directories(libugdk 
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/modules/ugdk-core/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/modules/ugdk-core/src")

target_include_directories(libugdk
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/modules/ugdk-${UGDK_GRAPHIC_MODULE}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/modules/ugdk-${UGDK_GRAPHIC_MODULE}/src")

if (UGDK_CREATE_BINDINGS)
    target_include_directories(libugdk 
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/modules/ugdk-script/include"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/modules/ugdk-script/src"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src" # for SWIG
        )
endif()   
    
# Add GLM dependency
target_include_directories(libugdk ${CMAKE_INCLUDE_SYSTEM} PUBLIC ${GLM_LOCATION})
    
if (WIN32)
    # utf8<->ucs4 conversion uses a networking function
    target_link_libraries(libugdk LINK_PUBLIC Ws2_32)
endif (WIN32)

# Libraries for the scripts.
target_link_libraries(libugdk LINK_PUBLIC ${UGDK_LIBRARIES})

# SDL-mixer must come before SDL itself, or else we may have linking errors.
if(UGDK_3D_ENABLED)
  target_link_libraries(libugdk LINK_PUBLIC SDL2-mixer SDL2 libjson ${BULLET_LIBRARIES} ${UGDK_OGRE_LIBRARIES} BtOgre)
  target_include_directories(libugdk ${CMAKE_INCLUDE_SYSTEM} PUBLIC ${UGDK_OGRE_INCLUDE_DIRS})
else()
  target_link_libraries(libugdk LINK_PUBLIC SDL2-mixer SDL2-image SDL2 libjson freetype-gl)
    if(NOT ANDROID)
        target_link_libraries(libugdk LINK_PUBLIC glew)
    endif()
endif()

set_target_properties(libugdk PROPERTIES 
    PROJECT_NAME UGDK
    OUTPUT_NAME ${UGDK_LIBRARY_NAME}
    OUTPUT_NAME_DEBUG ${UGDK_LIBRARY_NAME}-dbg
)

if(ANDROID)
    target_compile_definitions(libugdk PUBLIC 
        UGDK_USING_GLES
    )
endif(ANDROID)

include(cmake/ExecutableFunctions.cmake)


if (UNIX AND PYTHONLIBS_FOUND)
    set(UGDK_INSTALL_LOCATION "${CMAKE_INSTALL_PREFIX}/${UGDK_INSTALL_LOCATION_RELATIVE}")
endif (UNIX AND PYTHONLIBS_FOUND)

set (UGDK_OGRE_STATIC ${OGRE_STATIC})

configure_file (
    "cmake/config.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/include/ugdk/system/config.h"
) 

install(TARGETS libugdk
    DESTINATION lib
#    CONFIGURATIONS Release
)

install(DIRECTORY modules/ugdk-core/include/
        DESTINATION include/${UGDK_LIBRARY_NAME} 
        FILES_MATCHING PATTERN "*.h" PATTERN "*.th" PATTERN "*.tcc")
        
install(DIRECTORY modules/ugdk-${UGDK_GRAPHIC_MODULE}/include/
        DESTINATION include/${UGDK_LIBRARY_NAME} 
        FILES_MATCHING PATTERN "*.h" PATTERN "*.th" PATTERN "*.tcc")
        
if (UGDK_CREATE_BINDINGS)
    install(DIRECTORY modules/ugdk-script/include/
            DESTINATION include/${UGDK_LIBRARY_NAME} 
            FILES_MATCHING PATTERN "*.h" PATTERN "*.th" PATTERN "*.tcc")
endif()   
        

if (PYTHONLIBS_FOUND AND PYTHON_INCLUDE_DIR AND EXISTS "${PYTHON_INCLUDE_DIR}/patchlevel.h")
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/src/generated/ 
            DESTINATION ${UGDK_INSTALL_LOCATION_RELATIVE}/${UGDK_BIGVERSION}/python
            FILES_MATCHING PATTERN "*.py")
endif (PYTHONLIBS_FOUND AND PYTHON_INCLUDE_DIR AND EXISTS "${PYTHON_INCLUDE_DIR}/patchlevel.h")

# Export variables for any project using UGDK as subdirectory
set(UGDK_HEADERS ${UGDK_HEADERS} PARENT_SCOPE)

# Tests and examples
if(UGDK_BUILD_TESTS)
    add_subdirectory(test)
endif(UGDK_BUILD_TESTS)

