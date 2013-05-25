# Requires CMake 2.8 or newer.
cmake_minimum_required (VERSION 2.8)

project (glew C)

include_directories("include")
add_library(glew STATIC "src/glew.c")

set(GLEW_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" CACHE PATH "" FORCE)
set(GLEW_LIBRARY glew CACHE PATH ""  FORCE)

MARK_AS_ADVANCED(GLEW_INCLUDE_DIR GLEW_LIBRARY)