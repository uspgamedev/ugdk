#include <cstdio>

#include <ugdk/config/config.h>
#include <ugdk/script/module.h>

#include <ugdk/script/languages/lua/luawrapper.h>
#include <ugdk/script/languages/python/pythonwrapper.h>

#define    LUA_INIT_FUNCTION_NAME(name) luaopen_##name
#define PYTHON_INIT_FUNCTION_NAME(name) init_##name

#define    LUA_INIT_FUNCTION_SIGNATURE(name) int LUA_INIT_FUNCTION_NAME(name)(lua_State*)
#define PYTHON_INIT_FUNCTION_SIGNATURE(name) void PYTHON_INIT_FUNCTION_NAME(name)(void)

inline std::string LuaNameConversion(const std::string& name) {
    std::string result = name;
    for(size_t i = 0; i < result.size(); ++i)
        if(result[i] == '_') result[i] = '.';
    return result;
}

#define    LUA_MODULE_NAME(name) LuaNameConversion(#name)
#define PYTHON_MODULE_NAME(name) "_" #name

typedef lua_CFunction LUA_inittype;
typedef ugdk::script::python::PyInitFunction PYTHON_inittype;

extern "C" {
#define MODULE_INIT_DECLARTION(LANG, NAME) extern LANG##_INIT_FUNCTION_SIGNATURE(NAME);
UGDK_LANGUAGES_LIST(MODULE_INIT_DECLARTION)
}

namespace ugdk {

#define MODULE_ITEM(LANG, name)    \
    ::ugdk::script::Module<LANG##_inittype>(LANG##_MODULE_NAME(name), LANG##_INIT_FUNCTION_NAME(name)),

#define DECLARE_MODULES(LANG) \
static const script::Module<LANG##_inittype> LANG##_MODULES[UGDK_MODULES_NUM] = { \
    UGDK_MODULES_LIST(LANG, MODULE_ITEM) \
}

#ifdef UGDK_USING_LUA
DECLARE_MODULES(   LUA);
#endif

#ifdef UGDK_USING_PYTHON
DECLARE_MODULES(PYTHON);
#endif


template <class wrapper_t, class init_func_t>
static void RegisterModules(wrapper_t* wrapper,
                            const script::Module<init_func_t> modules[],
                            const char* lang_name) {
    for (size_t i = 0; i < UGDK_MODULES_NUM; ++i) {
        if(!wrapper->RegisterModule(modules[i])) {
            fprintf(stderr, "[%s] Load module '%s': >>ERROR<<\n", lang_name, modules[i].name().c_str());
        } else {
#ifdef DEBUG
            fprintf(stderr, "[%s] Load module '%s': ok\n", lang_name, modules[i].name().c_str());
#endif
        }
    }
}

void RegisterLuaModules(script::lua::LuaWrapper* wrapper) {
#ifdef UGDK_USING_LUA
    RegisterModules(wrapper, LUA_MODULES, "Lua");
#endif
}

void RegisterPythonModules(script::python::PythonWrapper* wrapper) {
#ifdef UGDK_USING_PYTHON
    RegisterModules(wrapper, PYTHON_MODULES, "Python");
#endif
}

} /* namespace ugdk */

#undef UGDK_MODULES_LIST




