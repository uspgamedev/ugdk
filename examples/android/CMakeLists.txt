
add_library (example-android SHARED 
    example-android.cc
    ${CMAKE_SOURCE_DIR}/externals/sdl2/src/main/android/SDL_android_main.c
)
set_target_properties (example-android PROPERTIES OUTPUT_NAME main)

target_link_libraries(example-android libugdk)
