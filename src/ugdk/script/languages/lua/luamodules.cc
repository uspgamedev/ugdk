#include <ugdk/script/languages/lua/modules.h>

#include <vector>

#include <ugdk/script/module.h>
#include <ugdk/script/languages/lua/luawrapper.h>

namespace ugdk {
namespace script {
namespace lua {

typedef std::vector< Module<inittype> > LuaModuleList;

static LuaModuleList& get_module_list() {
    static LuaModuleList lua_modules;
    return lua_modules;
}

void AddModule(const Module<inittype> & module) {
    get_module_list().push_back(module);
}

void RegisterModules(LuaWrapper* wrapper) {
    LuaModuleList& lua_modules = get_module_list();
    for(LuaModuleList::const_iterator it = lua_modules.begin(); it != lua_modules.end(); ++it) {
        if(!wrapper->RegisterModule(*it)) {
            fprintf(stderr, "[%s] Load module '%s': >>ERROR<<\n", wrapper->lang_name().c_str(), it->name().c_str());
        } else {
#ifdef DEBUG
            fprintf(stderr, "[%s] Load module '%s': ok\n", wrapper->lang_name().c_str(), it->name().c_str());
#endif
        }
    }
}

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */
