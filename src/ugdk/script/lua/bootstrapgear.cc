
#include <cstdio>

#include <ugdk/script/lua/bootstrapgear.h>
#include <ugdk/script/lua/auxlib.h>

namespace ugdk {
namespace script {
namespace lua {

/// Public:

bool BootstrapGear::Initialize(const std::vector<Module>& modules) {
    L_.push(SafeInitialize);
    L_.push(this);
    L_.push(&modules);
    return (TracedCall(2, 0) == Constant::OK());
}

int BootstrapGear::GenerateDatatable() {
    // too small to call in protected mode.
    L_.newtable();
    return L_.aux().ref(Constant::REGISTRYINDEX());
}

void BootstrapGear::DestroyDatatable(DataID datatable_id) {
    // too small too.
    L_.aux().unref(Constant::REGISTRYINDEX(), datatable_id);
}

/// Private:

typedef const std::vector<Module> ModuleList;

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

void BootstrapGear::PreloadModules (const std::vector<Module>& modules) {
    L_.getglobal(LUA_LOADLIBNAME);      // [pack]
    L_.getfield(-1, "preload");         // [pack,preload]
    ModuleList::const_iterator it = modules.begin();
    for (; it != modules.end(); ++it) {
        printf("Preloading module \"%s\"\n", it->name_.c_str());
        L_.push(it->init_func_);        // [pack,preload,init_func]
        L_.setfield(-2, it->name_.c_str());     // [pack,preload]
    }
    L_.pop(2);
}

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */
