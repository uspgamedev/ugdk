
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

    State (lua_State* L) :
      L_(L) {}

    operator bool() const { return L_; }

    operator lua_State*() const { return L_; }

    AuxLib aux() const { return AuxLib(L_); }

    int gettop() const { return lua_gettop(L_); }
    void settop(int index) { lua_settop(L_, index); }

    void pushvalue (int index) { lua_pushvalue(L_, index); }
    void pushnil () { lua_pushnil(L_); }

    void push (bool b) { lua_pushboolean(L_, b); }
    void push (lua_Integer integer) { lua_pushinteger(L_, integer); }
    void push (lua_Number number) { lua_pushnumber(L_, number); }
    void push (void *ptr) { lua_pushlightuserdata(L_, ptr); }
    void push (const char* str) { lua_pushstring(L_, str); }
    void push (lua_CFunction func, int n = 0) {
        lua_pushcclosure(L_, func, n);
    }
    template <class T>
    void push (T value) { push(value); }

    void pop (int n) { lua_pop(L_, n); }

    void insert (int index) { lua_insert(L_, index); }
    void remove (int index) { lua_remove(L_, index); }

    void newtable () { lua_newtable(L_); }

    void getglobal (const char* name) { lua_getglobal(L_, name); }
    void getfield (int index, const char* k) { lua_getfield(L_, index, k); }
    void setfield (int index, const char* k) { lua_setfield(L_, index, k); }

    void gettable (int index) { lua_gettable(L_, index); }
    void settable (int index) { lua_settable(L_, index); }
    void rawgeti (int index, int n) { lua_rawgeti(L_, index, n); }
    void rawseti (int index, int n) { lua_rawseti(L_, index, n); }

    int setfenv(int index) { return lua_setfenv(L_, index); }
    void getfenv(int index) { lua_getfenv(L_, index); }

    int setmetatable(int index) { return lua_setmetatable(L_, index); }

    bool isnil (int index) const { return lua_isnil(L_, index); }
    bool istable (int index) const { return lua_istable(L_, index); }

    const char* tostring(int n) const { return lua_tostring(L_, n); }
    int type (int n) const { return lua_type(L_, n); }
    
    void call (int nargs, int nres) { lua_call(L_, nargs, nres); }
    const Constant pcall (int nargs, int nres, int errfunc) {
        return Constant(
            std::tr1::bind(lua_pcall, L_, nargs, nres, errfunc)
        );
    }

    int gc (Constant what, int data) {
        return lua_gc(L_, what.value(), data);
    }

    // TODO: make-do for now.
    static void errormsg (std::string msg) {
      fprintf(stderr, "lua: %s\n", msg.c_str());
      fprintf(stdout, "lua: %s\n", msg.c_str());
    }

  private:

    lua_State* const L_;

};

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */

#endif /* UGDK_SCRIPT_LUA_STATE_H_ */

