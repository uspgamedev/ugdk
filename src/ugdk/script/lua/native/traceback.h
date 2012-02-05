
#ifndef UGDK_SCRIPT_LUA_NATIVE_TRACEBACK_H_
#define UGDK_SCRIPT_LUA_NATIVE_TRACEBACK_H_

#include <ugdk/script/lua/header.h>

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

}

#endif /* UGDK_SCRIPT_LUA_NATIVE_TRACEBACK_H_ */
