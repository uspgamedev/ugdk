
#include <cstdio>

#include <ugdk/script/module.h>

#include <ugdk/script/languages/lua/luawrapper.h>
#include <ugdk/script/languages/python/pythonwrapper.h>

#define UGDK_MODULES_NUM 11

#define UGDK_MODULES_LIST(ACTION) \
    ACTION(pyramidworks_collision) \
    ACTION(pyramidworks_geometry) \
    ACTION(ugdk_action) \
    ACTION(ugdk_audio) \
    ACTION(ugdk_base) \
    ACTION(ugdk_drawable) \
    ACTION(ugdk_graphic) \
    ACTION(ugdk_input) \
    ACTION(ugdk_math) \
    ACTION(ugdk_time) \
    ACTION(ugdk_util)

/// WHAT WIZARDY IS THIS!?

#define    LUA_INIT_FUNCTION_NAME(name) luaopen_##name
#define PYTHON_INIT_FUNCTION_NAME(name) init_##name

#define    LUA_INIT_FUNCTION_SIGNATURE(name) int LUA_INIT_FUNCTION_NAME(name)(lua_State*)
#define PYTHON_INIT_FUNCTION_SIGNATURE(name) void PYTHON_INIT_FUNCTION_NAME(name)(void)

extern "C" {

#define UGDKLUA_DECLARE_INIT(name)    extern    LUA_INIT_FUNCTION_SIGNATURE(name);
#define UGDKPYTHON_DECLARE_INIT(name) extern PYTHON_INIT_FUNCTION_SIGNATURE(name);

UGDK_MODULES_LIST(UGDKLUA_DECLARE_INIT)
UGDK_MODULES_LIST(UGDKPYTHON_DECLARE_INIT)

#undef UGDKLUA_DECLARE_INIT
#undef UGDKPYTHON_DECLARE_INIT
}

namespace ugdk {

using script::Module;
using script::python::PyInitFunction;

typedef lua_CFunction LUA_inittype;
typedef PyInitFunction PYTHON_inittype;

#define UGDKLUA_LIST_ITEM(name) \
    Module<LUA_inittype>(#name, LUA_INIT_FUNCTION_NAME(name)),

    static const Module<LUA_inittype> LUA_MODULES[UGDK_MODULES_NUM] = {
        UGDK_MODULES_LIST(UGDKLUA_LIST_ITEM)
    };

#undef UGDKLUA_LIST_ITEM
    //Module<PYTHON_inittype>("ugdk."#name, PYTHON_INIT_FUNCTION_NAME(name)),

#define UGDKPYTHON_LIST_ITEM(name) \
    Module<PyInitFunction>(#name, init_##name),

    static const Module<PyInitFunction> PYTHON_MODULES[UGDK_MODULES_NUM] = {
        UGDK_MODULES_LIST(UGDKPYTHON_LIST_ITEM)
    };

#undef UGDKPYTHON_LIST_ITEM

template <class wrapper_t, class init_func_t>
static void RegisterModules(wrapper_t* wrapper,
                            const Module<init_func_t> modules[],
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




