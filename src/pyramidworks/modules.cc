
#include <cstdio>

#include <ugdk/script/module.h>

#include <ugdk/script/languages/lua/luawrapper.h>
#include <ugdk/script/languages/python/pythonwrapper.h>

#define PYRAMIDWORKS_MODULES_NUM 2

#define PYRAMIDWORKS_MODULES_LIST(ACTION) \
    ACTION(collision) \
    ACTION(geometry)

/// WHAT WIZARDY IS THIS!?

extern "C" {
#define PYRAMIDWORKSLUA_DECLARE_INIT(name) extern int luaopen_pyramidworks_##name(lua_State*);

    PYRAMIDWORKS_MODULES_LIST(PYRAMIDWORKSLUA_DECLARE_INIT)

#undef PYRAMIDWORKSLUA_DECLARE_INIT
#define PYRAMIDWORKSPYTHON_DECLARE_INIT(name) extern void init_pyramidworks_##name(void);

    PYRAMIDWORKS_MODULES_LIST(PYRAMIDWORKSPYTHON_DECLARE_INIT)


#undef PYRAMIDWORKSPYTHON_DECLARE_INIT
}

namespace pyramidworks {

using ugdk::script::Module;
using ugdk::script::python::PyInitFunction;

#define PYRAMIDWORKSLUA_LIST_ITEM(name) \
    ugdk::script::Module<lua_CFunction>("pyramidworks."#name, luaopen_pyramidworks_##name),

    static const Module<lua_CFunction> LUA_MODULES[PYRAMIDWORKS_MODULES_NUM] = {
        PYRAMIDWORKS_MODULES_LIST(PYRAMIDWORKSLUA_LIST_ITEM)
    };

#undef PYRAMIDWORKSLUA_LIST_ITEM

#define PYRAMIDWORKSPYTHON_LIST_ITEM(name) \
    Module<PyInitFunction>("_pyramidworks_"#name, init_pyramidworks_##name),

    static const Module<PyInitFunction> PYTHON_MODULES[PYRAMIDWORKS_MODULES_NUM] = {
        PYRAMIDWORKS_MODULES_LIST(PYRAMIDWORKSPYTHON_LIST_ITEM)
    };

#undef PYRAMIDWORKSPYTHON_LIST_ITEM

template <class wrapper_t, class init_func_t>
static void RegisterModules(wrapper_t* wrapper,
                            const Module<init_func_t> modules[],
                            const char* lang_name) {
    for (size_t i = 0; i < PYRAMIDWORKS_MODULES_NUM; ++i) {
        if(!wrapper->RegisterModule(modules[i])) {
            fprintf(stderr, "[%s] Load module '%s': >>ERROR<<\n", lang_name, modules[i].name().c_str());
        } else {
#ifdef DEBUG
            fprintf(stderr, "[%s] Load module '%s': ok\n", lang_name, modules[i].name().c_str());
#endif
        }
    }
}

void RegisterLuaModules(ugdk::script::lua::LuaWrapper* wrapper) {
    RegisterModules(wrapper, LUA_MODULES, "Lua");
}

void RegisterPythonModules(ugdk::script::python::PythonWrapper* wrapper) {
    RegisterModules(wrapper, PYTHON_MODULES, "Python");
}

} /* namespace pyramidworks */

#undef PYRAMIDWORKS_MODULES_LIST




