
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

    explicit AuxLib(const StatePtr& L) : L_(L) {}

    static StatePtr newstate() {
        StatePtr L(luaL_newstate(), lua_close);
        return L;
    }

    void openlibs() { luaL_openlibs(L_.get()); }

    const Constant loadfile(const char* filename) {
        return Constant(
            std::tr1::bind(luaL_loadfile, L_.get(), filename)
        );
    }

  private:

    StatePtr L_;

};

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */


#endif /* UGDK_SCRIPT_LUA_AUXLIB_H_ */
