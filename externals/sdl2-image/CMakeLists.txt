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

include_directories(include)

add_library(SDL2-image ${SDL2-image-sources})
target_link_libraries(SDL2-image ${SDL2_LIBRARY})