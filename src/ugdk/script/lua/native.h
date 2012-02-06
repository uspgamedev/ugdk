
#ifndef UGDK_SCRIPT_LUA_NATIVE_H_
#define UGDK_SCRIPT_LUA_NATIVE_H_

#include <ugdk/script/lua/header.h>

using ugdk::script::lua::Constant;

extern "C" {

static int traceback (lua_State *L) {
  lua_getglobal(L, "debug");
  if (!lua_istable(L, -1)) {
    lua_pop(L, 1);
    return 1;
  }
  lua_getfield(L, -1, "traceback");
  if (!lua_isfunction(L, -1)) {
    lua_pop(L, 2);
    return 1;
  }
  lua_pushvalue(L, 1);  /* pass error message */
  lua_pushinteger(L, 2);  /* skip this function and traceback */
  lua_call(L, 2, 1);  /* call debug.traceback */
  return 1;
}

static const int sentinel_ = 0;
#define sentinel ((void*)&sentinel_)

static int require (lua_State *L) {

  // clean extra args
  lua_settop(L, 2);
  // get first arg
  const char* name = luaL_checkstring(L, 1);
  // get second arg
  luaL_checktype(L, 2, LUA_TFUNCTION);
  lua_CFunction loader = lua_tocfunction(L, 2);
  lua_getfield(L, Constant::REGISTRYINDEX(), "_LOADED");
  // check if the module has already been loaded.
  lua_getfield(L, 3, name);
  if (lua_toboolean(L, -1)) {
    if (lua_touserdata(L, -1) == sentinel)
      return luaL_error(L, "loop or previuous error loading module ", name);
    return 1;
  }
  // nope
  lua_pop(L, 1);
  // so use the loader
  // but first, set up a sentinel to avoid OROBOROS
  lua_pushlightuserdata(L, sentinel);
  lua_setfield(L, 3, name);
  // call loader
  lua_pushcfunction(L, loader);
  lua_pushstring(L, name);
  lua_call(L, 1, 1);
  // check result
  if (!lua_isnil(L, -1))
    lua_setfield(L, 3, name);
  lua_getfield(L, 3, name);
  // if no result, put true
  if (lua_touserdata(L, -1) == sentinel) {
    lua_pushboolean(L, true);
    lua_pushvalue(L, -1); // repeat
    lua_setfield(L, 3, name);
  }
  return 1;
}

#undef sentinel

}

#endif /* UGDK_SCRIPT_LUA_NATIVE_H_ */
