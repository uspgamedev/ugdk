cmake_minimum_required (VERSION 2.8)

project (example1 CXX C)

add_executable (example1 main.cc)

target_link_libraries(example1 libugdk)