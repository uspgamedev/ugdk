cmake_minimum_required(VERSION 2.8)
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

add_definitions(-DLOAD_TGA
                -DLOAD_BMP
                -DLOAD_GIF
                -DLOAD_XCF
                -DLOAD_PCX
                -DLOAD_PNM
                -DLOAD_XV)
                
find_package(PNG QUIET)
if(PNG_FOUND)
    include_directories(${PNG_INCLUDE_DIRS})
    set(SDLIMAGE_LIBPNG ${PNG_LIBRARIES})
    add_definitions(-DLOAD_PNG)
else(PNG_FOUND)
    set(PNG_SHARED OFF CACHE BOOL "Build shared lib." FORCE)
    set(PNG_STATIC ON CACHE BOOL "Build static lib." FORCE)
    add_subdirectory(lpng163)
    include_directories(lpng163)
    include_directories(${CMAKE_CURRENT_BINARY_DIR}/lpng163)
    set(SDLIMAGE_LIBPNG libpng_static)
    add_definitions(-DLOAD_PNG)
endif(PNG_FOUND)

include_directories(include)

add_library(SDL2-image ${SDL2-image-sources})
target_link_libraries(SDL2-image ${SDL2_LIBRARY} ${SDLIMAGE_LIBPNG})
SET_TARGET_PROPERTIES(SDL2-image PROPERTIES COMPILE_FLAGS "-fstrict-aliasing -fprefetch-loop-arrays")