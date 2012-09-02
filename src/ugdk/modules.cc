#include <cstdio>
#include <vector>
#include <typeinfo>

#include <ugdk/config/config.h>
#include <ugdk/script/module.h>

#include <ugdk/script/languages/lua/luawrapper.h>
#include <ugdk/script/languages/python/pythonwrapper.h>
#include <ugdk/script/languages/lua/modules.h>

#define MODULE_INIT_DECLARTION(LANG, NAME) extern LANG##_INIT_FUNCTION_SIGNATURE(NAME);
#define DECLARE_MODULE_INITS(LANG) \
extern "C" { \
    UGDK_MODULES_LIST(LANG, MODULE_INIT_DECLARTION) \
}
#define MODULE_ITEM(LANG, name) \
    result.push_back(::ugdk::script::Module< ::ugdk::script::LANG##_inittype >(LANG##_MODULE_NAME(name), LANG##_INIT_FUNCTION_NAME(name)));

#define DECLARE_MODULES(LANG) \
static std::vector< ::ugdk::script::Module< ::ugdk::script::LANG##_inittype > > build_UGDK_##LANG##_MODULES() { \
    std::vector< ::ugdk::script::Module< ::ugdk::script::LANG##_inittype > > result; \
    UGDK_MODULES_LIST(LANG, MODULE_ITEM) \
    return result; \
} \
static const std::vector< ::ugdk::script::Module< ::ugdk::script::LANG##_inittype > > \
    UGDK_##LANG##_MODULES = build_UGDK_##LANG##_MODULES();

UGDK_LANGUAGES_LIST(DECLARE_MODULE_INITS)
UGDK_LANGUAGES_LIST(DECLARE_MODULES)

namespace ugdk {

static int AddLuaScripts(const std::vector< ::ugdk::script::Module< ::ugdk::script::LUA_inittype > >& modules ) {
    std::vector< ::ugdk::script::Module< ::ugdk::script::LUA_inittype > >::const_iterator it;
    for(it = modules.begin(); it != modules.end(); ++it) {
        ugdk::script::lua::AddModule(*it);
    }
    return 0;
}

int LUA_SCRIPTS_ADDED = AddLuaScripts(UGDK_LUA_MODULES);

template <class init_func_t>
static void RegisterModules(::ugdk::script::LangWrapper* wrapper_base,
                            const std::vector< ::ugdk::script::Module<init_func_t> >& modules) {
    
    if(wrapper_base == NULL) return;
    ::ugdk::script::InheritableLangWrapper<init_func_t>* wrapper = 
        dynamic_cast< ::ugdk::script::InheritableLangWrapper<init_func_t>* >(wrapper_base);
    if(wrapper == NULL) {
        fprintf(stderr, "Wrapper of language '%s' is incompatible with modules of type '%s'\n", 
            wrapper_base->lang_name().c_str(), typeid(init_func_t).name());
        return;
    }
    typename std::vector< ::ugdk::script::Module<init_func_t> >::const_iterator it;
    for(it = modules.begin(); it != modules.end(); ++it) {
        if(!wrapper->RegisterModule(*it)) {
            fprintf(stderr, "[%s] Load module '%s': >>ERROR<<\n", wrapper->lang_name().c_str(), it->name().c_str());
        } else {
#ifdef DEBUG
            fprintf(stderr, "[%s] Load module '%s': ok\n", wrapper->lang_name().c_str(), it->name().c_str());
#endif
        }
    }
}

void RegisterLuaModules(script::LangWrapper* wrapper) {
#ifdef UGDK_USING_LUA
    RegisterModules(wrapper, UGDK_LUA_MODULES);
#endif
}

void RegisterPythonModules(script::LangWrapper* wrapper) {
#ifdef UGDK_USING_PYTHON
    RegisterModules(wrapper, UGDK_PYTHON_MODULES);
#endif
}

} /* namespace ugdk */
