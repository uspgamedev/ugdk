cmake_minimum_required(VERSION 2.8.11)

project(libjson CXX)

SET(LIBJSON_SOURCE
libjson.h
JSONOptions.h
_internal/Dependencies/libbase64++/libbase64++.h
_internal/Dependencies/mempool++/mempool.h
_internal/Source/internalJSONNode.cpp
_internal/Source/internalJSONNode.h
_internal/Source/JSONAllocator.cpp
_internal/Source/JSONAllocator.h
_internal/Source/JSONChildren.cpp
_internal/Source/JSONChildren.h
_internal/Source/JSONDebug.cpp
_internal/Source/JSONDebug.h
_internal/Source/JSONDefs/GNU_C.h
_internal/Source/JSONDefs/Strings_Defs.h
_internal/Source/JSONDefs/Unknown_C.h
_internal/Source/JSONDefs/Visual_C.h
_internal/Source/JSONDefs.h
_internal/Source/JSONGlobals.h
_internal/Source/JSONIterators.cpp
_internal/Source/JSONMemory.cpp
_internal/Source/JSONMemory.h
_internal/Source/JSONMemoryPool.h
_internal/Source/JSONNode.cpp
_internal/Source/JSONNode.h
_internal/Source/JSONNode_Mutex.cpp
_internal/Source/JSONPreparse.cpp
_internal/Source/JSONPreparse.h
_internal/Source/JSONSharedString.h
_internal/Source/JSONSingleton.h
_internal/Source/JSONStats.h
_internal/Source/JSONStream.cpp
_internal/Source/JSONStream.h
_internal/Source/JSONValidator.cpp
_internal/Source/JSONValidator.h
_internal/Source/JSONWorker.cpp
_internal/Source/JSONWorker.h
_internal/Source/JSONWriter.cpp
_internal/Source/JSON_Base64.h
_internal/Source/libjson.cpp
_internal/Source/NumberToString.h)

add_library(libjson ${LIBJSON_SOURCE})
target_include_directories(libjson PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>")
set_target_properties (libjson PROPERTIES 
    OUTPUT_NAME json
    FOLDER "Dependencies"
)

install(TARGETS libjson
    EXPORT UGDK_Install
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)


