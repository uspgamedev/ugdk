cmake_minimum_required(VERSION 2.8.11)
project(SDL2-image C)

set(SDL2-image-sources IMG.c
                       IMG_bmp.c IMG_gif.c IMG_jpg.c IMG_lbm.c 
                       IMG_pcx.c IMG_png.c IMG_pnm.c IMG_tga.c
                       IMG_tif.c IMG_webp.c IMG_xcf.c IMG_xpm.c 
                       IMG_xv.c IMG_xxx.c
                       include/SDL_image.h miniz.h
                       )
                       
if(MSVC)
    list(APPEND SDL2-image-sources version.rc)
endif(MSVC)

find_package(PNG QUIET)
if(PNG_FOUND)
    include_directories(${PNG_INCLUDE_DIRS})
    set(SDLIMAGE_LIBPNG ${PNG_LIBRARIES})
else(PNG_FOUND)
    if(ANDROID)
        set(PNG_SHARED ON CACHE BOOL "Build shared lib." FORCE)
        set(PNG_STATIC OFF CACHE BOOL "Build static lib." FORCE)
    else()
        set(PNG_SHARED OFF CACHE BOOL "Build shared lib." FORCE)
        set(PNG_STATIC ON CACHE BOOL "Build static lib." FORCE)
    endif()
    add_subdirectory(lpng163)
    include_directories(lpng163)
    include_directories(${CMAKE_CURRENT_BINARY_DIR}/lpng163)
    if(ANDROID)
        set(SDLIMAGE_LIBPNG libpng)
    else()
        set(SDLIMAGE_LIBPNG libpng_static)
    endif()
endif(PNG_FOUND)

add_library(SDL2-image ${SDL2-image-sources})
set_target_properties(SDL2-image PROPERTIES COMPILE_FLAGS "-fstrict-aliasing -fprefetch-loop-arrays")

target_compile_definitions(SDL2-image PRIVATE
    LOAD_TGA
    LOAD_BMP
    LOAD_GIF
    LOAD_XCF
    LOAD_PCX
    LOAD_PNM
    LOAD_XV
    LOAD_PNG
    )
target_link_libraries(     SDL2-image LINK_PUBLIC SDL2 ${SDLIMAGE_LIBPNG})
target_include_directories(SDL2-image PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include") # TODO: add 'SYSTEM' flag when we have cmake 2.8.12

