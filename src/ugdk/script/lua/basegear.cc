
#include <cstdio>

#include <ugdk/script/lua/basegear.h>
#include <ugdk/script/lua/auxlib.h>

#include <ugdk/script/lua/native/traceback.h>

namespace ugdk {
namespace script {
namespace lua {

static int traceback (lua_State *L);

/// Protected:

const Constant BaseGear::Report (const Constant& c) {
  if (c != Constant::OK() && !L_.isnil(-1)) {
    const char *msg = L_.tostring(-1);
    if (msg == NULL) msg = "(error object is not a string)";
    LuaMsg("%s\n", msg);
    L_.pop(1);
    /* force a complete garbage collection in case of errors */
    L_.gc(Constant::gc::COLLECT(), 0);
  }
  return c;
}

const Constant BaseGear::TracedCall (int nargs, int nres) {
  int base = L_.gettop() - nargs;
  L_.pushcfunction(traceback);
  L_.insert(base);
  const Constant result = L_.pcall(nargs, nres, base);
  L_.remove(base);
  return Report(result);
}

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

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */
