
option(UGDK_INTERNAL_GLEW        "Set to ON to use an internal GLEW instead of the system one." OFF)
option(UGDK_INTERNAL_FREETYPE    "Set to ON to use an internal freetype instead of the system one." OFF)
option(UGDK_INTERNAL_SDL2        "Set to ON to use an internal SDL2 instead of the system one." OFF)
option(UGDK_INTERNAL_SDL2_IMAGE  "Set to ON to use an internal SDL2_image instead of the system one." OFF)
option(UGDK_INTERNAL_SDL2_MIXER  "Set to ON to use an internal SDL2_mixer instead of the system one." OFF)
option(UGDK_INTERNAL_OGRE        "Set to ON to use an internal OGRE instead of the system one." OFF)

macro(update_list LISTNAME VALUE)
    set(${LISTNAME} "${${LISTNAME}};${VALUE}" CACHE INTERNAL "")
endmacro()

########################

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

    
    check_package(SDL2 SDL2)
	check_package(SDL2_MIXER SDL2_mixer)
	check_package(FREETYPE Freetype)
    if(UGDK_3D_ENABLED)
        check_package(OGRE OGRE)
    else()
		check_package(SDL2_IMAGE SDL2_image)
		check_package(GLEW GLEW 1.5)
	endif()
endif()

########################
## SDL2
########################
if(UGDK_INTERNAL_SDL2)
    # Build from source!
    if(ANDROID)
        # For android, using SDL's CMakeLists is not necessary (and doesn't work without too much effort)
        include(android-sdl2.cmake)

    elseif()
        if(WIN32)
            find_package(OpenGL REQUIRED)
            list(APPEND EXTRA_LIBS ${OPENGL_LIBRARIES})
        endif()
        # Set some options for the SDL's CMakeLists.
        if(${BUILD_SHARED_LIBS})
            set(SDL_SHARED ON  CACHE BOOL "Build a shared version of the library" FORCE)
            set(SDL_STATIC OFF CACHE BOOL "Build a static version of the library" FORCE)
        else()
            set(SDL_SHARED OFF CACHE BOOL "Build a shared version of the library" FORCE)
            set(SDL_STATIC ON  CACHE BOOL "Build a static version of the library" FORCE)
        endif()
        set(DIRECTX    OFF CACHE BOOL "Use DirectX for Windows audio/video" FORCE)
        set(RENDER_D3D OFF CACHE BOOL "Enable the Direct3D render driver" FORCE)
        if (MSVC)
            set(LIBC   ON CACHE BOOL "Use the system C library" FORCE)
        endif(MSVC)
        add_subdirectory(sdl2 sdl2-build)

        target_include_directories(SDL2 PUBLIC "${SDL2_SOURCE_DIR}/include" "${SDL2_BINARY_DIR}/include") # TODO: add 'SYSTEM' flag when we have cmake 2.8.12
    endif()

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
    add_subdirectory(sdl2-mixer) # Defines 'SDL2-mixer'

else(UGDK_INTERNAL_SDL2_MIXER)
    add_library(SDL2-mixer STATIC IMPORTED GLOBAL)
    set_target_properties(SDL2-mixer PROPERTIES
        IMPORTED_LOCATION "${SDL2_MIXER_LIBRARIES}"
        INTERFACE_INCLUDE_DIRECTORIES "${SDL2_MIXER_INCLUDE_DIRS}")
endif(UGDK_INTERNAL_SDL2_MIXER)

if(NOT ANDROID AND NOT UGDK_3D_ENABLED)
    if(UGDK_INTERNAL_GLEW)
        add_subdirectory(glew) # Defines 'glew'
    else(UGDK_INTERNAL_GLEW)
        add_library(glew SHARED IMPORTED GLOBAL)
        set_target_properties(glew PROPERTIES
            IMPORTED_LOCATION "${GLEW_LIBRARIES}"
            INTERFACE_INCLUDE_DIRECTORIES "${GLEW_INCLUDE_DIRS}")
    endif(UGDK_INTERNAL_GLEW)
endif(NOT ANDROID AND NOT UGDK_3D_ENABLED)

add_subdirectory(libjson)

########################
## freetype
########################
if(UGDK_INTERNAL_FREETYPE)
	add_subdirectory(freetype)

elseif(NOT UGDK_3D_ENABLED)
	add_library(freetype STATIC IMPORTED GLOBAL)
	set_target_properties(freetype PROPERTIES
        IMPORTED_LOCATION "${FREETYPE_LIBRARY_REL}"
        IMPORTED_LOCATION_RELEASE "${FREETYPE_LIBRARY_REL}"
        IMPORTED_LOCATION_DEBUG "${FREETYPE_LIBRARY_DBG}"
		INTERFACE_INCLUDE_DIRECTORIES "${FREETYPE_INCLUDE_DIRS}")
endif(UGDK_INTERNAL_FREETYPE)

########################
## 3D Module
########################
if(UGDK_3D_ENABLED)

    ########################
    ## OGRE
    ########################
    if(UGDK_INTERNAL_OGRE)
        set (OGRE_STATIC TRUE CACHE BOOL "Forcing static build" FORCE)
        set (OGRE_ARCHIVE_OUTPUT ${LIB_DIR})
        set (OGRE_LIBRARY_OUTPUT ${LIB_DIR})
        set (OGRE_RUNTIME_OUTPUT ${LIB_DIR})
        set (OGRE_INCLUDE_DIRS "" CACHE INTERNAL "Ogre external include directories")
        set (OGRE_LIBRARIES "" CACHE INTERNAL "Compiled Ogre libraries")
        if(UGDK_INTERNAL_FREETYPE)
            set (FREETYPE_FT2BUILD_INCLUDE_DIR ${freetype_SOURCE_DIR}/include CACHE PATH "" FORCE)
            set (FREETYPE_LIBRARY freetype CACHE STRING "" FORCE)
            set (FREETYPE_LIBRARY_DBG freetype CACHE STRING "" FORCE)
            set (FREETYPE_LIBRARY_REL freetype CACHE STRING "" FORCE)
            set (FREETYPE_FOUND ON CACHE BOOL "" FORCE)
            set (FREETYPE_INCLUDE_DIR ${freetype_SOURCE_DIR}/include CACHE PATH "" FORCE)
            set (FREETYPE_INCLUDE_DIR_freetype2 ${freetype_SOURCE_DIR}/include CACHE PATH "" FORCE)
            set (FREETYPE_INCLUDE_DIR_ft2build ${freetype_SOURCE_DIR}/include CACHE PATH "" FORCE)
        endif(UGDK_INTERNAL_FREETYPE)
       
        add_subdirectory(Ogre)
        set(UGDK_OGRE_LIBRARIES ${OGRE_LIBRARIES} PARENT_SCOPE)
        set(UGDK_OGRE_INCLUDE_DIRS ${OGRE_INCLUDE_DIRS} PARENT_SCOPE)
    else()
        set(UGDK_OGRE_LIBRARIES ${OGRE_LIBRARIES})
        set(UGDK_OGRE_INCLUDE_DIRS ${OGRE_INCLUDE_DIRS})
        
        set(OGRE_COMPONENTS Plugin_BSPSceneManager;Plugin_CgProgramManager;Plugin_OctreeSceneManager;
                            Plugin_OctreeZone;Plugin_ParticleFX;Plugin_PCZSceneManager;RenderSystem_GL;
                            RenderSystem_GL3Plus;RenderSystem_GLES;RenderSystem_GLES2;RenderSystem_Direct3D9;
                            RenderSystem_Direct3D11;Paging;Terrain;Volume;Overlay)
        foreach(it ${OGRE_COMPONENTS})
            if(OGRE_${it}_FOUND)
                list(APPEND UGDK_OGRE_LIBRARIES ${OGRE_${it}_LIBRARIES})
                list(APPEND UGDK_OGRE_INCLUDE_DIRS ${OGRE_${it}_INCLUDE_DIRS})
            endif()
        endforeach()
        set(UGDK_OGRE_LIBRARIES ${UGDK_OGRE_LIBRARIES} PARENT_SCOPE)
        set(UGDK_OGRE_INCLUDE_DIRS ${UGDK_OGRE_INCLUDE_DIRS} PARENT_SCOPE)
    endif()
    

    ########################
    ## BULLET PHYSICS
    ########################
    set (BULLET_INCLUDE_DIRS "" CACHE INTERNAL "Bullet external include directories")
    set (BULLET_LIBRARIES "" CACHE INTERNAL "Compiled Bullet libraries")
    add_subdirectory(BulletPhysics)

    ########################
    ## BtOgre
    ########################
    add_subdirectory(btogre) #generates target BtOgre
else(UGDK_3D_ENABLED)

	########################
	## freetype-gl
	########################
	add_subdirectory(freetype-gl)
endif(UGDK_3D_ENABLED)

