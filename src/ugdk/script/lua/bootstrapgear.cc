
#include <cstdio>

#include <ugdk/script/lua/bootstrapgear.h>
#include <ugdk/script/lua/auxlib.h>

namespace ugdk {
namespace script {
namespace lua {

/// Public

void BootstrapGear::LoadLibs () {
    // luaL_checkversion(L.get());  // TODO-lua5.2: add this.
    L_.gc(Constant::gc::STOP(), 0);
    L_.aux().openlibs();
    L_.gc(Constant::gc::RESTART(), 0);
}

using std::vector;

void BootstrapGear::PreloadModule(const vector<Module>& modules) {
    L_.getglobal(LUA_LOADLIBNAME);             // [pack]
    L_.getfield(-1, "preload");                // [pack,preload]
    vector<Module>::const_iterator it = modules.begin();
    for (; it != modules.end(); ++it) {
        L_.push(it->init_func_);      // [pack,preload,init_func]
        L_.setfield(-2, it->name_.c_str());    // [pack,preload]
    }
    L_.pop(2);                                 // []
}

int BootstrapGear::GenerateDatatable() {
    //L_.push(datatable_id_);
    L_.newtable();
    return L_.aux().ref(Constant::REGISTRYINDEX());
    //L_.settable(Constant::REGISTRYINDEX());
}

void BootstrapGear::DestroyDatatable(DataID datatable_id) {
    L_.aux().unref(Constant::REGISTRYINDEX(), datatable_id);
}

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */
