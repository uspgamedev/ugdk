
set (UGDK_TEST_CFLAGS "-g -Wall -ansi -O0 -U_FORTIFY_SOURCE -msse2")

add_executable (script_customtest EXCLUDE_FROM_ALL test/src/script/script_customtest.cc)

set_target_properties (script_customtest PROPERTIES RUNTIME_OUTPUT_DIRECTORY test/)
set_target_properties (script_customtest PROPERTIES COMPILE_FLAGS ${UGDK_TEST_CFLAGS})

# FIXME
target_link_libraries (script_customtest ugdk_d)


