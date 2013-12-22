cmake_minimum_required (VERSION 2.8.11)

project (glew C)

add_library(glew STATIC "src/glew.c")

target_compile_definitions(glew PUBLIC "-DGLEW_STATIC")
target_include_directories(glew PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
