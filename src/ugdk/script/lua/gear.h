
#ifndef UGDK_SCRIPT_LUA_GEAR_H_
#define UGDK_SCRIPT_LUA_GEAR_H_

#include <vector>

#include <ugdk/script/type.h>
#include <ugdk/script/lua/header.h>
#include <ugdk/script/lua/state.h>

namespace ugdk {
namespace script {
namespace lua {

class Gear {

  public:

    Gear(lua_State* L, DataID datatable_id) :
        L_(L),
        datatable_id_(datatable_id),
        base_index_(lua_gettop(L)) {}
    ~Gear() { L_.settop(base_index_); }

    State* operator->() { return &L_; }

    void LoadLibs ();

    void PreloadModule(const std::vector<Module>& modules);

    void CreateDatatable();

    // [-0,+1]
    bool GetData (DataID id);

    // [-1,+0]
    bool SetData (DataID id);

    // [-1,+0]
    void* UnwrapData (const VirtualType& type);

    void WrapData(void *data, const VirtualType& type);

    // [-0,+1]
    const Constant DoFile (const char* filename);

    // [-0,+?]
    const Constant LoadModule (const char* name, lua_CFunction loader);

  private:

    State   L_;
    DataID  datatable_id_;
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
