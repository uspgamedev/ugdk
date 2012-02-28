
add_executable (script_customtest EXCLUDE_FROM_ALL test/src/script/script_customtest.cc)

set_target_properties (script_customtest PROPERTIES RUNTIME_OUTPUT_DIRECTORY test/)

if (CMAKE_COMPILER_IS_GNUCXX)
	set (UGDK_TEST_CFLAGS "-g -Wall -ansi -O0 -U_FORTIFY_SOURCE -msse2")
	set_target_properties (script_customtest PROPERTIES COMPILE_FLAGS ${UGDK_TEST_CFLAGS})
else ()
	set (UGDK_TEST_CFLAGS "")
endif (CMAKE_COMPILER_IS_GNUCXX)

# FIXME
target_link_libraries (script_customtest ${FRAMEWORK_BUILDNAME})


