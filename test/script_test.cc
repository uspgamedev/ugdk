#include <Python.h>

#include <cstdlib>

#include "SDL.h"
#include <ugdk/base/engine.h>
#include <ugdk/modules.h>
#include <ugdk/script/scriptmanager.h>
#include <ugdk/script/virtualobj.h>
#include <ugdk/script/languages/lua/luawrapper.h>
#include <ugdk/script/languages/python/pythonwrapper.h>

using ugdk::Engine;

static void InitScripts() {
    using ugdk::script::lua::LuaWrapper;
    using ugdk::script::python::PythonWrapper;

    //inicializando lua
    LuaWrapper* lua_wrapper = new LuaWrapper();
    ugdk::RegisterLuaModules(lua_wrapper);
    SCRIPT_MANAGER()->Register("Lua", lua_wrapper);
    
    PythonWrapper* python_wrapper = new PythonWrapper();
    ugdk::RegisterPythonModules(python_wrapper);
    SCRIPT_MANAGER()->Register("Python", python_wrapper);
}

int main(int argc, char **argv) {
    ugdk::Configuration config;
    config.base_path = "data/";
    InitScripts();
    
    Engine* eng = Engine::reference();
    eng->Initialize(config);

    PyObject *path = PySys_GetObject("path");
    PyList_Append(path, PyString_FromString("../src/generated"));
    
    using ugdk::script::VirtualObj;
    {
        VirtualObj wassup = SCRIPT_MANAGER()->LoadModule("wassup");

        VirtualObj::List args;
        args.push_back(wassup["vecx"]);
        wassup["supimpa"](args);
    }
    SCRIPT_MANAGER()->LoadModule("main");
    
    
    eng->Run();
    eng->Release();
    return 0;
}