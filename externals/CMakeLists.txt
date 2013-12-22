
option(UGDK_INTERNAL_GLEW        "Set to ON to use an internal GLEW instead of the system one." OFF)
option(UGDK_INTERNAL_SDL2        "Set to ON to use an internal SDL2 instead of the system one." OFF)
option(UGDK_INTERNAL_SDL2_IMAGE   "Set to ON to use an internal SDL2_image instead of the system one." OFF)
option(UGDK_INTERNAL_SDL2_MIXER   "Set to ON to use an internal SDL2_mixer instead of the system one." OFF)

if(ANDROID)
    # Always compile all externals for Android.
    set(UGDK_INTERNAL_SDL2 ON CACHE BOOL "Set to ON to use an internal SDL2 instead of the system one." FORCE)
    set(UGDK_INTERNAL_SDL2_IMAGE ON CACHE BOOL "Set to ON to use an internal SDL2_image instead of the system one." FORCE)
    set(UGDK_INTERNAL_SDL2_MIXER ON CACHE BOOL "Set to ON to use an internal SDL2_mixer instead of the system one." FORCE)
else()

    macro(check_package OPTION_VAR PKG_NAME)
        find_package(${PKG_NAME} ${ARGN} QUIET)
        if(NOT ${OPTION_VAR}_FOUND)
            message("-- ${PKG_NAME} not found, forcing internal.")
            set(UGDK_INTERNAL_${OPTION_VAR} ON CACHE BOOL "Set to ON to use an internal ${PKG_NAME} instead of the system one." FORCE)
        endif()
    endmacro(check_package)

    check_package(SDL2 SDL2)
    check_package(SDL2_IMAGE SDL2_image)
    check_package(SDL2_MIXER SDL2_mixer)
    check_package(GLEW GLEW 1.5)
endif()

########################
## SDL2
########################
if(UGDK_INTERNAL_SDL2)
    # Build from source!
    if(ANDROID)
        # For android, using SDL's CMakeLists is not necessary (and doesn't work without too much effort)
        include(android-sdl2.cmake)

    else()
        # Set some options for the SDL's CMakeLists.
        set(SDL_SHARED OFF CACHE BOOL "Build a shared version of the library")
        set(DIRECTX    OFF CACHE BOOL "Use DirectX for Windows audio/video" FORCE)
        set(RENDER_D3D OFF CACHE BOOL "Enable the Direct3D render driver" FORCE)
        add_subdirectory(sdl2 sdl2-build)

        target_include_directories(SDL2 PUBLIC "${SDL2_SOURCE_DIR}/include") # TODO: add 'SYSTEM' flag when we have cmake 2.8.12
    endif()

else(UGDK_INTERNAL_SDL2)
    add_library(SDL2 STATIC IMPORTED GLOBAL)
    set_target_properties(SDL2 PROPERTIES
        IMPORTED_LOCATION "${SDL2_LIBRARY_PRIMARY}"
        IMPORTED_LINK_INTERFACE_LIBRARIES "${SDL2_DEPENDENCIES}"
        INTERFACE_INCLUDE_DIRECTORIES "${SDL2_INCLUDE_DIRS}")
endif(UGDK_INTERNAL_SDL2)

########################
## SDL2_IMAGE
########################
if(UGDK_INTERNAL_SDL2_IMAGE)
    add_subdirectory(sdl2-image) # Defines 'SDL2-image'

else(UGDK_INTERNAL_SDL2_IMAGE)
    add_library(SDL2-image SHARED IMPORTED GLOBAL)
    set_target_properties(SDL2-image PROPERTIES
        IMPORTED_LOCATION "${SDL2_IMAGE_LIBRARIES}"
        INTERFACE_INCLUDE_DIRECTORIES "${SDL2_IMAGE_INCLUDE_DIRS}")
endif(UGDK_INTERNAL_SDL2_IMAGE)

########################
## SDL2_MIXER
########################
if(UGDK_INTERNAL_SDL2_MIXER)
    add_subdirectory(sdl2-mixer) # Defines 'SDL2-mixer'

else(UGDK_INTERNAL_SDL2_MIXER)
    add_library(SDL2-mixer SHARED IMPORTED GLOBAL)
    set_target_properties(SDL2-mixer PROPERTIES
        IMPORTED_LOCATION "${SDL2_MIXER_LIBRARIES}"
        INTERFACE_INCLUDE_DIRECTORIES "${SDL2_MIXER_INCLUDE_DIRS}")
endif(UGDK_INTERNAL_SDL2_MIXER)

if(NOT ANDROID)
    # Other libs
    find_package(OpenGL REQUIRED)
    include_directories(${OPENGL_INCLUDE_DIR})

    if(UGDK_INTERNAL_GLEW)
        add_subdirectory(glew) # Defines 'glew'
    else(UGDK_INTERNAL_GLEW)
        add_library(glew SHARED IMPORTED GLOBAL)
        set_target_properties(glew PROPERTIES
            IMPORTED_LOCATION "${GLEW_LIBRARIES}"
            INTERFACE_INCLUDE_DIRECTORIES "${GLEW_INCLUDE_DIRS}")
    endif(UGDK_INTERNAL_GLEW)

    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/freetype-glpp/CMakeLists.txt")
        find_package(Git)
        if(GIT_FOUND)
            execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init
                            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
        else(GIT_FOUND)
            message(FATAL_ERROR "Git submodule 'freetype-glpp' not initialized and git not found.")
        endif(GIT_FOUND)
    endif()

    add_subdirectory(freetype-glpp)
    include_directories("freetype-glpp/include")
    set(FREETYPE_GLPP_LIBRARIES freetype-gl++)
endif(NOT ANDROID)
