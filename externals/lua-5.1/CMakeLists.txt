cmake_minimum_required(VERSION 2.8.11)

project(lua C)

set(LUA_SRC
src/lstate.h    src/lobject.h   src/lundump.h   src/lauxlib.c
src/lgc.h       src/loadlib.c   src/lstring.h   src/lapi.h
src/lbaselib.c  src/lvm.c       src/lzio.c      src/lmathlib.c
src/ldo.c       src/ltablib.c   src/lzio.h      src/ldebug.c
src/ldo.h       src/ltable.c    src/ldblib.c    src/lcode.h
src/llex.c      src/ltm.c       src/lgc.c       src/lvm.h
src/lopcodes.c  src/lmem.h      src/luaconf.h   src/print.c
src/linit.c     src/lparser.c   src/lstate.c    src/lopcodes.h
src/ldebug.h    src/ldump.c     src/lstrlib.c   src/llimits.h
src/lfunc.c     src/lundump.c   src/llex.h      src/lcode.c
src/ltm.h       src/luac.c      src/lua.h       src/loslib.c
src/lfunc.h     src/lauxlib.h   src/lstring.c   src/lmem.c
src/lua.c       src/lparser.h   src/lobject.c   src/lapi.c
src/lualib.h    src/ltable.h    src/liolib.c
)

add_library(lua ${LUA_SRC})
