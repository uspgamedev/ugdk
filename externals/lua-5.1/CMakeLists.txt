cmake_minimum_required(VERSION 2.8.11)

project(Lua C)

SET(LUA_CORE
    ${Lua_SOURCE_DIR}/src/lapi.c
    ${Lua_SOURCE_DIR}/src/lcode.c
    ${Lua_SOURCE_DIR}/src/ldebug.c
    ${Lua_SOURCE_DIR}/src/ldo.c
    ${Lua_SOURCE_DIR}/src/ldump.c
    ${Lua_SOURCE_DIR}/src/lfunc.c
    ${Lua_SOURCE_DIR}/src/lgc.c
    ${Lua_SOURCE_DIR}/src/llex.c
    ${Lua_SOURCE_DIR}/src/lmem.c
    ${Lua_SOURCE_DIR}/src/lobject.c
    ${Lua_SOURCE_DIR}/src/lopcodes.c
    ${Lua_SOURCE_DIR}/src/lparser.c
    ${Lua_SOURCE_DIR}/src/lstate.c
    ${Lua_SOURCE_DIR}/src/lstring.c
    ${Lua_SOURCE_DIR}/src/ltable.c 
    ${Lua_SOURCE_DIR}/src/ltm.c 
    ${Lua_SOURCE_DIR}/src/lundump.c
    ${Lua_SOURCE_DIR}/src/lvm.c
    ${Lua_SOURCE_DIR}/src/lzio.c
)

SET(LUA_STANDARD_LIBRARY
    ${Lua_SOURCE_DIR}/src/lauxlib.c
    ${Lua_SOURCE_DIR}/src/lbaselib.c
    ${Lua_SOURCE_DIR}/src/ldblib.c
    ${Lua_SOURCE_DIR}/src/liolib.c
    ${Lua_SOURCE_DIR}/src/lmathlib.c
    ${Lua_SOURCE_DIR}/src/loslib.c
    ${Lua_SOURCE_DIR}/src/ltablib.c
    ${Lua_SOURCE_DIR}/src/lstrlib.c
    ${Lua_SOURCE_DIR}/src/loadlib.c
    ${Lua_SOURCE_DIR}/src/linit.c
)

SET(PUBLIC_HEADERS
    ${Lua_SOURCE_DIR}/src/lua.h 
    ${Lua_SOURCE_DIR}/src/lualib.h
    ${Lua_SOURCE_DIR}/src/lauxlib.h
    ${Lua_SOURCE_DIR}/src/luaconf.h
)
SET(RESOURCE_FILES
    ${Lua_SOURCE_DIR}/etc/strict.lua
    ${Lua_SOURCE_DIR}/etc/lua.ico
)

add_library(lua ${LUA_CORE} ${LUA_STANDARD_LIBRARY} ${PUBLIC_HEADERS})
if(CMAKE_COMPILER_IS_GNUCC)
    target_link_libraries(lua LINK_PUBLIC m)
endif()
target_include_directories(lua PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
