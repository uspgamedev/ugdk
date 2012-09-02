#include <cstdio>

#include <ugdk/config/config.h>
#include <ugdk/script/module.h>

#include <ugdk/script/languages/lua/luawrapper.h>
#include <ugdk/script/languages/python/pythonwrapper.h>

extern "C" {
#define MODULE_INIT_DECLARTION(LANG, NAME) extern LANG##_INIT_FUNCTION_SIGNATURE(NAME);

#define DECLARE_MODULE_INITS(LANG) UGDK_MODULES_LIST(LANG, MODULE_INIT_DECLARTION)

UGDK_LANGUAGES_LIST(DECLARE_MODULE_INITS)
}

namespace ugdk {

#define MODULE_ITEM(LANG, name) \
    ::ugdk::script::Module< ::ugdk::script::LANG##_inittype >(LANG##_MODULE_NAME(name), LANG##_INIT_FUNCTION_NAME(name)),

#define DECLARE_MODULES(LANG) \
static const script::Module< ::ugdk::script::LANG##_inittype > UGDK_##LANG##_MODULES[UGDK_MODULES_NUM] = { \
    UGDK_MODULES_LIST(LANG, MODULE_ITEM) \
};

UGDK_LANGUAGES_LIST(DECLARE_MODULES)

template <class init_func_t>
static void RegisterModules(::ugdk::script::LangWrapper* wrapper_base,
                            const ::ugdk::script::Module<init_func_t> modules[]) {

    ::ugdk::script::InheritableLangWrapper<init_func_t>* wrapper = 
        dynamic_cast<::ugdk::script::InheritableLangWrapper<init_func_t>*>(wrapper_base);
    for (size_t i = 0; i < UGDK_MODULES_NUM; ++i) {
        if(!wrapper->RegisterModule(modules[i])) {
            fprintf(stderr, "[%s] Load module '%s': >>ERROR<<\n", wrapper->lang_name().c_str(), modules[i].name().c_str());
        } else {
#ifdef DEBUG
            fprintf(stderr, "[%s] Load module '%s': ok\n", wrapper->lang_name().c_str(), modules[i].name().c_str());
#endif
        }
    }
}

void RegisterLuaModules(script::lua::LuaWrapper* wrapper) {
#ifdef UGDK_USING_LUA
    RegisterModules(wrapper, UGDK_LUA_MODULES);
#endif
}

void RegisterPythonModules(script::python::PythonWrapper* wrapper) {
#ifdef UGDK_USING_PYTHON
    RegisterModules(wrapper, UGDK_PYTHON_MODULES);
#endif
}

} /* namespace ugdk */

#undef UGDK_MODULES_LIST




