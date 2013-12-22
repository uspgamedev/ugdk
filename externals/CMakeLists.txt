set(SDL2_SOURCE_DIR ${PROJECT_SOURCE_DIR}/externals/sdl2)

file(GLOB SDL2_SOURCE_FILES
  ${SDL2_SOURCE_DIR}/src/*.c
  
  ${SDL2_SOURCE_DIR}/src/audio/*.c
  ${SDL2_SOURCE_DIR}/src/audio/android/*.c
  ${SDL2_SOURCE_DIR}/src/audio/dummy/*.c
  
  ${SDL2_SOURCE_DIR}/src/atomic/SDL_atomic.c
  ${SDL2_SOURCE_DIR}/src/atomic/SDL_spinlock.c # Was .c.arm, but this doesn't exist...

  ${SDL2_SOURCE_DIR}/src/core/android/*.c

  ${SDL2_SOURCE_DIR}/src/cpuinfo/*.c
  ${SDL2_SOURCE_DIR}/src/events/*.c
  ${SDL2_SOURCE_DIR}/src/file/*.c
  
  ${SDL2_SOURCE_DIR}/src/haptic/*.c
  ${SDL2_SOURCE_DIR}/src/haptic/dummy/*.c
  ${SDL2_SOURCE_DIR}/src/joystick/*.c
  ${SDL2_SOURCE_DIR}/src/joystick/android/*.c
  ${SDL2_SOURCE_DIR}/src/loadso/dlopen/*.c
  ${SDL2_SOURCE_DIR}/src/power/*.c
  ${SDL2_SOURCE_DIR}/src/power/android/*.c
  ${SDL2_SOURCE_DIR}/src/filesystem/dummy/*.c
  
  ${SDL2_SOURCE_DIR}/src/render/*.c
  ${SDL2_SOURCE_DIR}/src/render/*/*.c
  ${SDL2_SOURCE_DIR}/src/stdlib/*.c
  ${SDL2_SOURCE_DIR}/src/thread/*.c
  ${SDL2_SOURCE_DIR}/src/thread/pthread/*.c
  ${SDL2_SOURCE_DIR}/src/timer/*.c
  ${SDL2_SOURCE_DIR}/src/timer/unix/*.c
  ${SDL2_SOURCE_DIR}/src/video/*.c
  ${SDL2_SOURCE_DIR}/src/video/android/*.c
  ${SDL2_SOURCE_DIR}/src/test/*.c)
  
add_library(SDL2 SHARED ${SDL2_SOURCE_FILES})

SET_TARGET_PROPERTIES(SDL2 PROPERTIES 
    COMPILE_FLAGS "-DGL_GLEXT_PROTOTYPES -DSDL_VIDEO_OPENGL_ES2 -std=c11 -Wno-switch-default"
    INCLUDE_DIRECTORIES "${SDL2_SOURCE_DIR}/include"    
)
target_link_libraries(SDL2 -ldl -lGLESv1_CM -lGLESv2 -llog -landroid -lm)
