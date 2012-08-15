#include <cstdio>

#include <ugdk/script/module.h>

#include <ugdk/script/languages/lua/luawrapper.h>
#include <ugdk/script/languages/python/pythonwrapper.h>

#define UGDK_MODULES_NUM 11
#define UGDK_MODULES_LIST(LANG, ACTION) \
    ACTION(LANG, pyramidworks_collision) \
    ACTION(LANG, pyramidworks_geometry) \
    ACTION(LANG, ugdk_action) \
    ACTION(LANG, ugdk_audio) \
    ACTION(LANG, ugdk_base) \
    ACTION(LANG, ugdk_drawable) \
    ACTION(LANG, ugdk_graphic) \
    ACTION(LANG, ugdk_input) \
    ACTION(LANG, ugdk_math) \
    ACTION(LANG, ugdk_time) \
    ACTION(LANG, ugdk_util)

#define UGDK_LANGUAGES_LIST(ACTION) \
    UGDK_MODULES_LIST(LUA, ACTION) \
    UGDK_MODULES_LIST(PYTHON, ACTION)

/// WHAT WIZARDY IS THIS!?

#define    LUA_INIT_FUNCTION_NAME(name) luaopen_##name
#define PYTHON_INIT_FUNCTION_NAME(name) init_##name

#define    LUA_INIT_FUNCTION_SIGNATURE(name) int LUA_INIT_FUNCTION_NAME(name)(lua_State*)
#define PYTHON_INIT_FUNCTION_SIGNATURE(name) void PYTHON_INIT_FUNCTION_NAME(name)(void)

#define    LUA_MODULE_NAME(name) #name
#define PYTHON_MODULE_NAME(name) "_" #name

typedef lua_CFunction LUA_inittype;
typedef ugdk::script::python::PyInitFunction PYTHON_inittype;

extern "C" {
#define MODULE_INIT_DECLARTION(LANG, NAME) extern LANG##_INIT_FUNCTION_SIGNATURE(NAME);

UGDK_MODULES_LIST(   LUA, MODULE_INIT_DECLARTION)
UGDK_MODULES_LIST(PYTHON, MODULE_INIT_DECLARTION)
}

namespace ugdk {

#define MODULE_ITEM(LANG, name)    \
    ::ugdk::script::Module<LANG##_inittype>(LANG##_MODULE_NAME(name), LANG##_INIT_FUNCTION_NAME(name)),

#define DECLARE_MODULES(LANG) \
static const script::Module<LANG##_inittype> LANG##_MODULES[UGDK_MODULES_NUM] = { \
    UGDK_MODULES_LIST(LANG, MODULE_ITEM) \
}

DECLARE_MODULES(   LUA);
DECLARE_MODULES(PYTHON);


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
    RegisterModules(wrapper, LUA_MODULES, "Lua");
}

void RegisterPythonModules(script::python::PythonWrapper* wrapper) {
    RegisterModules(wrapper, PYTHON_MODULES, "Python");
}

} /* namespace ugdk */

#undef UGDK_MODULES_LIST




