#include <ugdk/script/languages/python/modules.h>

#include <vector>
#include <cstdio>

#include <ugdk/script/module.h>
#include <ugdk/script/languages/python/pythonwrapper.h>

namespace ugdk {
namespace script {
namespace python {

typedef std::vector< Module<inittype> > PythonModuleList;

static PythonModuleList& get_module_list() {
    static PythonModuleList lua_modules;
    return lua_modules;
}

void AddModule(const Module<inittype> & module) {
    get_module_list().push_back(module);
}

void RegisterModules(PythonWrapper* wrapper) {
    PythonModuleList& lua_modules = get_module_list();
    for(PythonModuleList::const_iterator it = lua_modules.begin(); it != lua_modules.end(); ++it) {
        if(!wrapper->RegisterModule(*it)) {
            fprintf(stderr, "[%s] Load module '%s': >>ERROR<<\n", wrapper->lang_name().c_str(), it->name().c_str());
        } else {
#ifdef DEBUG
            fprintf(stderr, "[%s] Load module '%s': ok\n", wrapper->lang_name().c_str(), it->name().c_str());
#endif
        }
    }
}

} /* namespace python */
} /* namespace script */
} /* namespace ugdk */
