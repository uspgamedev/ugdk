
# Search lib64 paths for visual studio x64 builds.
IF(MSVC AND (CMAKE_SIZEOF_VOID_P EQUAL 8))
    set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS ON)
ENDIF()

# SYSTEM include directories don't produce warnings.
if(${CMAKE_VERSION} VERSION_LESS "2.8.12")
    set(CMAKE_INCLUDE_SYSTEM)
else()
    set(CMAKE_INCLUDE_SYSTEM SYSTEM)
endif()