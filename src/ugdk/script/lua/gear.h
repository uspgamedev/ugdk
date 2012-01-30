
#ifndef UGDK_SCRIPT_LUA_GEAR_H_
#define UGDK_SCRIPT_LUA_GEAR_H_

#include <ugdk/script/lua/state.h>

namespace ugdk {
namespace script {
namespace lua {

class Gear {

  public:

    Gear() {}
    ~Gear() {}

    void LoadLibs ();

    // [-0,+1]
    const Constant DoFile (const char* filename);

    // [-0,+?]
    const Constant LoadModule (const char* name, lua_CFunction loader);

  private:

    const Constant Report (const Constant& c);

    const Constant TracedCall (int nargs, int nres);

    State lua_;

};

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */

#endif /* UGDK_SCRIPT_LUA_GEAR_H_ */
