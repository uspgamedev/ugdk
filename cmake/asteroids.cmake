
IF (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET(GAME_SRCS ${GAME_SRCS} src/mac/SDLMain.h src/mac/SDLMain.m)
ENDIF ()
add_executable (asteroids EXCLUDE_FROM_ALL examples/asteroids-script/src/asteroids.cc
                ${GAME_SRCS})

set_target_properties (asteroids PROPERTIES RUNTIME_OUTPUT_DIRECTORY examples/asteroids-script/)

if (CMAKE_COMPILER_IS_GNUCXX)
	set (UGDK_TEST_CFLAGS "-g -Wall -ansi -O0 -U_FORTIFY_SOURCE -msse2")
	set_target_properties (asteroids PROPERTIES COMPILE_FLAGS ${UGDK_TEST_CFLAGS})
else ()
	set (UGDK_TEST_CFLAGS "")
endif (CMAKE_COMPILER_IS_GNUCXX)

# FIXME
target_link_libraries (asteroids
  ${FRAMEWORK_BUILDNAME}
  ${EXTRA_LIBRARIES}
  ${SDL_LIBRARY}
  ${SDLMIXER_LIBRARY}
  ${SDLTTF_LIBRARY}
  ${SDLIMAGE_LIBRARY}
  ${OPENGL_LIBRARIES}
  ${UGDK_LIBRARIES})
  #${LUA_LIBRARIES}
  #${PYTHON_FINAL_LIB})
