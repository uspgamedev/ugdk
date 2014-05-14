cmake_minimum_required(VERSION 2.8.11)

project(libjson CXX)

SET(LIBJSON_SOURCE
    src/internalJSONNode.cpp
    src/internalJSONNode.h
    src/JSONAllocator.cpp
    src/JSONAllocator.h
    src/JSONChildren.cpp
    src/JSONChildren.h
    src/JSONDebug.cpp
    src/JSONDebug.h
    src/JSONDefs/GNU_C.h
    src/JSONDefs/Strings_Defs.h
    src/JSONDefs/Unknown_C.h
    src/JSONDefs/Visual_C.h
    src/JSONDefs.h
    src/JSONGlobals.h
    src/JSONIterators.cpp
    src/JSONMemory.cpp
    src/JSONMemory.h
    src/JSONMemoryPool.h
    src/JSONNode.cpp
    src/JSONNode.h
    src/JSONNode_Mutex.cpp
    src/JSONOptions.h
    src/JSONPreparse.cpp
    src/JSONPreparse.h
    src/JSONSharedString.h
    src/JSONSingleton.h
    src/JSONStats.h
    src/JSONStream.cpp
    src/JSONStream.h
    src/JSONValidator.cpp
    src/JSONValidator.h
    src/JSONWorker.cpp
    src/JSONWorker.h
    src/JSONWriter.cpp
    src/JSON_Base64.h
    src/NumberToString.h
)

add_library(libjson ${LIBJSON_SOURCE})
target_include_directories(libjson PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
