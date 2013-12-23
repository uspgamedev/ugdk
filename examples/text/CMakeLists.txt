
add_ugdk_executable(example-text text.cc)
target_compile_definitions(example-text PRIVATE EXAMPLE_LOCATION="${CMAKE_CURRENT_SOURCE_DIR}")
