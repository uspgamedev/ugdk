
#ifndef UGDK_SCRIPT_LUA_GEAR_H_
#define UGDK_SCRIPT_LUA_GEAR_H_

#include <ugdk/script/lua/header.h>
#include <ugdk/script/lua/state.h>

namespace ugdk {
namespace script {
namespace lua {

class Gear : public Identifiable {

  public:

    Gear() : base_index_(0) {}
    ~Gear() {}

    bool ValidState() { return lua_; }

    void LoadLibs ();

    bool PushData (DataID id);

    // [-0,+1]
    const Constant DoFile (const char* filename);

    // [-0,+?]
    const Constant LoadModule (const char* name, lua_CFunction loader);

  private:

    State   lua_;
    int     base_index_;

    int Local(int index) const { return (base_index_ + index); }

    /// [-0,+(0|1),-]
    bool PushDataTable();

    const Constant Report (const Constant& c);

    /// Makes a traced call to a function.
    /** Behaves exactly like lua_pcall, with the usual traceback function from
     ** Lua.
     **
     ** [-(nargs+1),+(nres|1),e]
     */
    const Constant TracedCall (int nargs, int nres);

};

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */

#endif /* UGDK_SCRIPT_LUA_GEAR_H_ */
