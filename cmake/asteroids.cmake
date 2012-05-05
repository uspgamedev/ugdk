
IF (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    LIST(APPEND GAME_SRCS src/mac/SDLMain.h src/mac/SDLMain.m)
ENDIF ()
add_executable (asteroids EXCLUDE_FROM_ALL examples/asteroids-script/src/asteroids.cc
                ${GAME_SRCS})

set_target_properties (asteroids PROPERTIES RUNTIME_OUTPUT_DIRECTORY examples/asteroids-script/)
if(WIN32)
    add_custom_command(TARGET asteroids POST_BUILD 
                        COMMAND xcopy src\\generated\\*.py examples\\asteroids-script\\data\\scripts\\ugdk\\ /q /k /y > nul
                        #[WORKING_DIRECTORY dir]
                        COMMENT "Updating Python script modules." 
                        VERBATIM)
endif(WIN32)

if (CMAKE_COMPILER_IS_GNUCXX)
	set (UGDK_TEST_CFLAGS "-g -Wall -ansi -O0 -U_FORTIFY_SOURCE -msse2")
	set_target_properties (asteroids PROPERTIES COMPILE_FLAGS ${UGDK_TEST_CFLAGS})
else ()
	set (UGDK_TEST_CFLAGS "")
endif (CMAKE_COMPILER_IS_GNUCXX)

target_link_libraries(asteroids ${FRAMEWORK_RELEASE})
