
#include <cstdio>

#include <ugdk/script/languages/lua/bootstrapgear.h>
#include <ugdk/script/languages/lua/auxlib.h>
#include <ugdk/script/languages/lua/datagear.h>

namespace ugdk {
namespace script {
namespace lua {

/// Public:

bool BootstrapGear::Initialize(const ModuleList& modules) {
    L_ = AuxLib::newstate();
    if (!L_) return false;
    L_.pushcfunction(SafeInitialize);
    L_.pushudata(this);
    L_.pushudata(&modules);
    return (TracedCall(2, 0) == Constant::OK());
}

DataGear* BootstrapGear::NextGear() {
    // too small to call in protected mode.
    L_.newtable();
    int datatable_id = L_.aux().ref(Constant::REGISTRYINDEX());
    return datatable_id == LUA_NOREF ? NULL : new DataGear(L_, datatable_id);
}

void BootstrapGear::Abort() {
    lua_close(L_);
    // too small too.
}

/// Private:

int BootstrapGear::SafeInitialize (lua_State* L) {
    State L_(L);

    L_.settop(2);
    BootstrapGear& btgear = *(static_cast<BootstrapGear*>(L_.touserdata(1)));
    ModuleList& modules = *(static_cast<ModuleList*>(L_.touserdata(2)));
    L_.settop(0);

    btgear.LoadLibs();
    btgear.PreloadModules(modules);

    return 0;
}

void BootstrapGear::LoadLibs () {
    // luaL_checkversion(L.get());  // TODO-lua5.2: add this.
    L_.gc(Constant::gc::STOP(), 0);
    L_.aux().openlibs();
    L_.gc(Constant::gc::RESTART(), 0);
}

void BootstrapGear::PreloadModules (const ModuleList& modules) {
    L_.getglobal(LUA_LOADLIBNAME);      // [pack]
    L_.getfield(-1, "preload");         // [pack,preload]
    ModuleList::const_iterator it = modules.begin();
    for (; it != modules.end(); ++it) {
        L_.pushcfunction(it->init_func());        // [pack,preload,init_func]
        L_.setfield(-2, it->name().c_str());     // [pack,preload]
    }
    L_.pop(2);
}

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */
