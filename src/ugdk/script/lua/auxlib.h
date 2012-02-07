
#ifndef UGDK_SCRIPT_LUA_AUXLIB_H_
#define UGDK_SCRIPT_LUA_AUXLIB_H_

#include <functional>

#include <ugdk/script/lua/header.h>

namespace ugdk {
namespace script {
namespace lua {

class State;

class AuxLib {

  public:

    explicit AuxLib(lua_State* L) : L_(L) {}

    static lua_State* newstate() {
        return luaL_newstate();
    }

    void openlibs() { luaL_openlibs(L_); }

    const Constant loadfile(const char* filename) {
        return Constant(
            std::tr1::bind(luaL_loadfile, L_, filename)
        );
    }

    int checkintteger(int index) { return luaL_checkinteger(L_, index); }

    DataID ref(int index) { return luaL_ref(L_, index); }

    void unref(int index, int n) { luaL_unref(L_, index, n); }

  private:

    lua_State* const L_;

};

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */


#endif /* UGDK_SCRIPT_LUA_AUXLIB_H_ */
