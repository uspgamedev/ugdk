
#include <cerrno>

#include <string>

#include <ugdk/base/engine.h>
#include <ugdk/base/configuration.h>

#include <ugdk/math/vector2D.h>

#include <ugdk/script/scriptmanager.h>
#include <ugdk/script/langwrapper.h>
#include <ugdk/script/virtualobj.h>
#include <ugdk/script/lua/luawrapper.h>
#include <ugdk/script/lua/header.h>
#include <ugdk/script/python/pythonwrapper.h>

extern "C" {
extern int luaopen_ugdk_math(lua_State* L);
extern void init_ugdk_math(void);
}

using ugdk::Vector2D;

static ugdk::Engine* engine() {
    return ugdk::Engine::reference();
}

static void InitScripts() {

    using ugdk::script::LangWrapper;
    using ugdk::script::lua::LuaWrapper;
    using ugdk::script::python::PythonWrapper;

    //inicializando lua
    LuaWrapper* lua_wrapper = new LuaWrapper();
    lua_wrapper->RegisterModule("ugdk.math", luaopen_ugdk_math);
    SCRIPT_MANAGER()->Register("Lua", lua_wrapper);

    //inicializando python
    PythonWrapper* py_wrapper = new PythonWrapper();
    printf("Registered Python Module: %d\n",
           (int)py_wrapper->RegisterModule("_ugdk_math", init_ugdk_math));
    SCRIPT_MANAGER()->Register("Python", py_wrapper);
}

static void RunTests() {

}

int main(int argc, char *argv[]) {

    ugdk::Configuration engine_config;
    engine_config.window_title = "Script System Custom Tests";
    engine_config.window_size  = Vector2D(800.0, 600.0);
    engine_config.fullscreen   = false;

    engine_config.base_path = "./";

    struct stat st;
    // Removing the trailing slash.
    int s = stat(engine_config.base_path.substr(0, engine_config.base_path.size() - 1).c_str(), &st);
    if(s < 0 && errno == ENOENT)
        engine_config.base_path = "./";

    engine_config.window_icon = "";

    InitScripts();

    engine()->Initialize(engine_config);

    RunTests();

    // Transfers control to the framework.
    engine()->Run();

    // Releases all loaded textures, to avoid issues when changing resolution.
    engine()->video_manager()->Release();
    engine()->text_manager()->Release();

    engine()->Release();

    return 0;
}
