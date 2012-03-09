
IF (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET(GAME_SRCS ${GAME_SRCS} src/mac/SDLMain.h src/mac/SDLMain.m)
ENDIF ()
add_executable (asteroids EXCLUDE_FROM_ALL examples/asteroids-script/src/asteroids.cc
					   examples/asteroids-script/src/scriptscene.cc
					   examples/asteroids-script/src/scriptentity.cc
					   examples/asteroids-script/src/scriptentitystack.cc
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
					   ${LUA_LIBRARIES}
					   ${PYTHON_FINAL_LIB})
