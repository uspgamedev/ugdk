
add_definitions(-DEXAMPLE_LOCATION="${CMAKE_CURRENT_SOURCE_DIR}")

add_executable (example-text example-text.cc)
target_link_libraries(example-text libugdk)