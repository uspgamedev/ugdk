
option(UGDK_INTERNAL_GLEW        "Set to ON to use an internal GLEW instead of the system one." OFF)
option(UGDK_INTERNAL_FREETYPE    "Set to ON to use an internal freetype instead of the system one." OFF)
option(UGDK_INTERNAL_SDL2        "Set to ON to use an internal SDL2 instead of the system one." OFF)
option(UGDK_INTERNAL_SDL2_IMAGE  "Set to ON to use an internal SDL2_image instead of the system one." OFF)
option(UGDK_INTERNAL_SDL2_MIXER  "Set to ON to use an internal SDL2_mixer instead of the system one." OFF)

option(UGDK_FORCE_SUBMODULES     "Set to ON to force the submodules to the commited version." ON)

if(${UGDK_FORCE_SUBMODULES})
    macro(init_submodule SUBMODULE)
        find_host_package(Git REQUIRED)
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init ${SUBMODULE}
                        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
    endmacro()
else()
    macro(init_submodule)
    endmacro(init_submodule)
endif()

if(ANDROID)
    # Always compile all externals for Android.
    set(UGDK_INTERNAL_FREETYPE   ON CACHE BOOL "Set to ON to use an internal freetype instead of the system one." FORCE)
    set(UGDK_INTERNAL_SDL2       ON CACHE BOOL "Set to ON to use an internal SDL2 instead of the system one." FORCE)
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

    check_package(FREETYPE Freetype)
    check_package(SDL2 SDL2)
    check_package(SDL2_IMAGE SDL2_image)
    check_package(SDL2_MIXER SDL2_mixer)
    check_package(GLEW GLEW 1.5)
endif()

########################
## SDL2
########################
if(UGDK_INTERNAL_SDL2)
    init_submodule(sdl2)
    add_subdirectory(sdl2) # Defines 'SDL2'

else(UGDK_INTERNAL_SDL2)
    find_package(OpenGL REQUIRED)

    add_library(SDL2 STATIC IMPORTED GLOBAL)
    set_target_properties(SDL2 PROPERTIES
        IMPORTED_LOCATION "${SDL2_LIBRARY_PRIMARY}"
        IMPORTED_LINK_INTERFACE_LIBRARIES "${SDL2_DEPENDENCIES};${OPENGL_LIBRARIES}"
        INTERFACE_INCLUDE_DIRECTORIES "${SDL2_INCLUDE_DIRS};${OPENGL_INCLUDE_DIR}")
endif(UGDK_INTERNAL_SDL2)

########################
## SDL2_IMAGE
########################
if(UGDK_INTERNAL_SDL2_IMAGE)
    init_submodule(sdl2-image)
    add_subdirectory(sdl2-image) # Defines 'SDL2-image'

else(UGDK_INTERNAL_SDL2_IMAGE)
    add_library(SDL2-image STATIC IMPORTED GLOBAL)
    set_target_properties(SDL2-image PROPERTIES
        IMPORTED_LOCATION "${SDL2_IMAGE_LIBRARIES}"
        INTERFACE_INCLUDE_DIRECTORIES "${SDL2_IMAGE_INCLUDE_DIRS}")
endif(UGDK_INTERNAL_SDL2_IMAGE)

########################
## SDL2_MIXER
########################
if(UGDK_INTERNAL_SDL2_MIXER)
    init_submodule(sdl2-mixer)
    add_subdirectory(sdl2-mixer) # Defines 'SDL2-mixer'

else(UGDK_INTERNAL_SDL2_MIXER)
    add_library(SDL2-mixer STATIC IMPORTED GLOBAL)
    set_target_properties(SDL2-mixer PROPERTIES
        IMPORTED_LOCATION "${SDL2_MIXER_LIBRARIES}"
        INTERFACE_INCLUDE_DIRECTORIES "${SDL2_MIXER_INCLUDE_DIRS}")
endif(UGDK_INTERNAL_SDL2_MIXER)

if(UGDK_INTERNAL_FREETYPE)
    add_subdirectory(freetype) # Defines 'freetype'

else(UGDK_INTERNAL_FREETYPE)
    add_library(freetype STATIC IMPORTED GLOBAL)
    set_target_properties(freetype PROPERTIES
        IMPORTED_LOCATION "${FREETYPE_LIBRARIES}"
        INTERFACE_INCLUDE_DIRECTORIES "${FREETYPE_INCLUDE_DIRS}")
endif(UGDK_INTERNAL_FREETYPE)

if(NOT ANDROID)
    if(UGDK_INTERNAL_GLEW)
        add_subdirectory(glew) # Defines 'glew'
    else(UGDK_INTERNAL_GLEW)
        add_library(glew SHARED IMPORTED GLOBAL)
        set_target_properties(glew PROPERTIES
            IMPORTED_LOCATION "${GLEW_LIBRARIES}"
            INTERFACE_INCLUDE_DIRECTORIES "${GLEW_INCLUDE_DIRS}")
    endif(UGDK_INTERNAL_GLEW)
endif(NOT ANDROID)

add_subdirectory(freetype-gl)
add_subdirectory(libjson)
