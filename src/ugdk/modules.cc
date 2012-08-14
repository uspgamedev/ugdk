
#include <cstdio>

#include <ugdk/script/module.h>

#include <ugdk/script/languages/lua/luawrapper.h>
#include <ugdk/script/languages/python/pythonwrapper.h>

#define UGDK_MODULES_NUM 9

#define UGDK_MODULES_LIST(ACTION) \
    ACTION(action) \
    ACTION(audio) \
    ACTION(base) \
    ACTION(drawable) \
    ACTION(graphic) \
    ACTION(input) \
    ACTION(math) \
    ACTION(time) \
    ACTION(util)

/// WHAT WIZARDY IS THIS!?

extern "C" {
#define UGDKLUA_DECLARE_INIT(name) extern int luaopen_ugdk_##name(lua_State*);

    UGDK_MODULES_LIST(UGDKLUA_DECLARE_INIT)

#undef UGDKLUA_DECLARE_INIT
#define UGDKPYTHON_DECLARE_INIT(name) extern void init_ugdk_##name(void);

    UGDK_MODULES_LIST(UGDKPYTHON_DECLARE_INIT)


#undef UGDKPYTHON_DECLARE_INIT
}

namespace ugdk {

using script::Module;
using script::python::PyInitFunction;

#define UGDKLUA_LIST_ITEM(name) \
    script::Module<lua_CFunction>("ugdk."#name, luaopen_ugdk_##name),

    static const Module<lua_CFunction> LUA_MODULES[UGDK_MODULES_NUM] = {
        UGDK_MODULES_LIST(UGDKLUA_LIST_ITEM)
    };

#undef UGDKLUA_LIST_ITEM

#define UGDKPYTHON_LIST_ITEM(name) \
    Module<PyInitFunction>("_ugdk_"#name, init_ugdk_##name),

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




