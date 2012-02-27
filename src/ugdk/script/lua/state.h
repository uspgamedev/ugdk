
#ifndef UGDK_SCRIPT_LUA_STATE_H_
#define UGDK_SCRIPT_LUA_STATE_H_

//#include <ugdk/portable/tr1.h>

#include <cstdlib>
#include <cstdio>

#include <functional>

#include <ugdk/script/lua/header.h>
#include <ugdk/script/lua/defs.h>
#include <ugdk/script/lua/primitive.h>
#include <ugdk/script/lua/auxlib.h>

namespace ugdk {
namespace script {
namespace lua {

class State {

  public:

    State (lua_State* L) :
      L_(L),
      auxlib_(L) {}

    operator bool() const { return !!(L_); }

    operator lua_State*() const { return L_; }

    void close() { lua_close(L_); L_ = NULL; }

    AuxLib& aux() { return auxlib_; }

    int gettop() const { return lua_gettop(L_); }
    void settop(int index) { lua_settop(L_, index); }

    void pushvalue (int index) { lua_pushvalue(L_, index); }
    void pushnil () { lua_pushnil(L_); }
    void pushboolean (bool b) { lua_pushboolean(L_, b); }
    void pushinteger (lua_Integer integer) { lua_pushinteger(L_, integer); }
    void pushnumber (lua_Number number) { lua_pushnumber(L_, number); }
    void pushudata (UData ptr) { lua_pushlightuserdata(L_, ptr); }
    void pushstring (const char* str) { lua_pushstring(L_, str); }
    void pushcfunction (lua_CFunction func, int n = 0) {
        lua_pushcclosure(L_, func, n);
    }
    template <class T>
    void pushudata (T* value) { pushudata(AsUData(value)); }
    template <class T>
    void pushprimitive(T value) { lua_push<T>::primitive(L_, value); }

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

    int getmetatable(int index) { return lua_getmetatable(L_, index); }
    int setmetatable(int index) { return lua_setmetatable(L_, index); }

    template <class T>
    bool isprimitive(int index) const {
        return lua_is<T>::primitive(L_, index);
    }
    bool isnil (int index) const { return !!(lua_isnil(L_, index)); }
    bool isstring (int index) const { return !!(lua_isstring(L_, index)); }
    bool istable (int index) const { return !!(lua_istable(L_, index)); }


    template <class T>
    T toprimitive(int n) const { return lua_to<T>::primitive(L_, n); }
    bool toboolean(int n) const { return !!(lua_toboolean(L_, n)); }
    lua_Integer tointeger(int n) const { return lua_tointeger(L_, n); }
    const char* tostring(int n) const { return lua_tostring(L_, n); }
    void* touserdata(int n) const { return lua_touserdata(L_, n); }

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

  private:

    lua_State*  L_;
    AuxLib      auxlib_;

};

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */

#endif /* UGDK_SCRIPT_LUA_STATE_H_ */

