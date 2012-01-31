
#ifndef UGDK_SCRIPT_LUA_STATE_H_
#define UGDK_SCRIPT_LUA_STATE_H_

#include <cstdlib>
#include <cstdio>

#include <string>
#include <functional>

#include <ugdk/script/lua/header.h>
#include <ugdk/script/lua/auxlib.h>

namespace ugdk {
namespace script {
namespace lua {

class State {

  public:

    State () :
      L_(AuxLib::newstate()) {
        if (L_.get() == NULL)
          State::errormsg("Could not create Lua state.");
    }

    AuxLib aux() const { return AuxLib(L_); }

    int gettop() const { return lua_gettop(L_.get()); }

    void pushvalue (int index) { lua_pushvalue(L_.get(), index); }
    void pushnil () { lua_pushnil(L_.get()); }

    void push (bool b) { lua_pushboolean(L_.get(), b); }
    void push (lua_Integer integer) { lua_pushinteger(L_.get(), integer); }
    void push (lua_Number number) { lua_pushnumber(L_.get(), number); }
    void push (void *ptr) { lua_pushlightuserdata(L_.get(), ptr); }
    void push (const char* str) { lua_pushstring(L_.get(), str); }
    void push (lua_CFunction func, int n = 0) {
        lua_pushcclosure(L_.get(), func, n);
    }

    void pop (int n) { lua_pop(L_.get(), n); }

    void insert (int index) { lua_insert(L_.get(), index); }
    void remove (int index) { lua_remove(L_.get(), index); }

    bool isnil (int index) const { return lua_isnil(L_.get(), index); }
    bool istable (int index) const { return lua_istable(L_.get(), index); }

    const char* tostring(int n) const { return lua_tostring(L_.get(), n); }
    int type (int n) const { return lua_type(L_.get(), n); }
    
    void call (int nargs, int nres) { lua_call(L_.get(), nargs, nres); }
    const Constant pcall (int nargs, int nres, int errfunc) {
        return Constant(
            std::tr1::bind(lua_pcall, L_.get(), nargs, nres, errfunc)
        );
    }

    int gc (Constant what, int data) {
        return lua_gc(L_.get(), what.value(), data);
    }

    // TODO: make-do for now.
    static void errormsg (std::string msg) {
      fprintf(stderr, "lua: %s\n", msg.c_str());
    }

  private:


    StatePtr L_;

};

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */

#endif /* UGDK_SCRIPT_LUA_STATE_H_ */

