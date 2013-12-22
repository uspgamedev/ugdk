
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

    # TODO: macro for simplicity
    macro(check_package OPTION_VAR PKG_NAME)
        find_package(${PKG_NAME} QUIET)
        if(NOT ${OPTION_VAR}_FOUND)
            message("-- ${PKG_NAME} not found, forcing internal.")
            set(UGDK_INTERNAL_${OPTION_VAR} ON CACHE BOOL "Set to ON to use an internal ${PKG_NAME} instead of the system one." FORCE)
        endif()
    endmacro(check_package)

    check_package(SDL2 SDL2)
    check_package(SDL2_IMAGE SDL2_image)
    check_package(SDL2_MIXER SDL2_mixer)
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

        target_link_libraries(     dependency-SDL2 LINK_PUBLIC SDL2-static)
        target_include_directories(dependency-SDL2 INTERFACE "${SDL2_SOURCE_DIR}/include")

        add_library(SDL2 STATIC IMPORTED)
    endif()

else(UGDK_INTERNAL_SDL2)
    add_library(SDL2 UNKNOWN IMPORTED)
    set_target_properties(SDL2 PROPERTIES
        IMPORTED_LOCATION "${SDL2_LIBRARIES}"
        INCLUDE_DIRECTORIES "${SDL2_INCLUDE_DIRS}")
endif(UGDK_INTERNAL_SDL2)

########################
## SDL2_IMAGE
########################
if(UGDK_INTERNAL_SDL2_IMAGE)
    add_subdirectory(sdl2-image) # Defines 'SDL2-image'

else(UGDK_INTERNAL_SDL2_IMAGE)
    add_library(SDL2-image UNKNOWN IMPORTED)
    set_target_properties(SDL2-image PROPERTIES
        IMPORTED_LOCATION "${SDL2_IMAGE_LIBRARIES}"
        INCLUDE_DIRECTORIES "${SDL2_IMAGE_INCLUDE_DIRS}")
endif(UGDK_INTERNAL_SDL2_IMAGE)

########################
## SDL2_MIXER
########################
if(UGDK_INTERNAL_SDL2_MIXER)
    add_subdirectory(sdl2-mixer) #  SDL2-mixer

else(UGDK_INTERNAL_SDL2_MIXER)
    add_library(SDL2-image-mixer UNKNOWN IMPORTED)
    set_target_properties(SDL2-mixer PROPERTIES
        IMPORTED_LOCATION "${SDL2_MIXER_LIBRARIES}"
        INCLUDE_DIRECTORIES "${SDL2_MIXER_INCLUDE_DIRS}")
endif(UGDK_INTERNAL_SDL2_MIXER)

if(NOT ANDROID)
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
        add_subdirectory(glew)
        set(GLEW_INCLUDE_DIRS ${GLEW_INCLUDE_DIR})
        set(GLEW_LIBRARIES ${GLEW_LIBRARY})
    endif(UGDK_INTERNAL_GLEW)
    include_directories(${GLEW_INCLUDE_DIRS})

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
