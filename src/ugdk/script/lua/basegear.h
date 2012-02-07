
#ifndef UGDK_SCRIPT_LUA_BASEGEAR_H_
#define UGDK_SCRIPT_LUA_BASEGEAR_H_

#include <vector>

#include <ugdk/script/lua/header.h>
#include <ugdk/script/lua/state.h>

namespace ugdk {
namespace script {
namespace lua {

class BaseGear {

  public:

    State* operator->() { return &L_; }

  protected:

    State L_;

    BaseGear(lua_State* L) :
        L_(L) {}

    ~BaseGear() {}

    State& L() { return L_; }

    const Constant Report (const Constant& c);

    /// Makes a traced call to a function.
    /** Behaves exactly like lua_pcall, with the usual traceback function from
     ** Lua.
     **
     ** [-(nargs+1),+(nres|0),e]
     */
    const Constant TracedCall (int nargs, int nres);

};

#define GETARGPTR(L,i,T,name) \
    T* name = static_cast<T*>((L).touserdata(i))

#define GETARG(L,i,T,name) \
    T& name = *(static_cast<T*>((L).touserdata(i)))

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */

#endif /* UGDK_SCRIPT_LUA_BASEGEAR_H_ */
