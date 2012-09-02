#include <ugdk/config/config.h>

#ifdef UGDK_USING_PYTHON
#include <Python.h>
#endif

#include <cstdlib>

#include "SDL.h"
#include <ugdk/base/engine.h>
#include <ugdk/modules.h>
#include <ugdk/script/scriptmanager.h>
#include <ugdk/script/virtualobj.h>
#include <ugdk/script/languages/lua/luawrapper.h>
#include <ugdk/script/languages/python/pythonwrapper.h>

using ugdk::Engine;
using ugdk::script::VirtualObj;

static void InitScripts() {
    using ugdk::script::LangWrapper;

#ifdef UGDK_USING_LUA
    LangWrapper* lua_wrapper = new ugdk::script::lua::LuaWrapper();
    ugdk::RegisterLuaModules(lua_wrapper);
    SCRIPT_MANAGER()->Register(lua_wrapper);
#endif
    
#ifdef UGDK_USING_PYTHON
    LangWrapper* python_wrapper = new ugdk::script::python::PythonWrapper();
    ugdk::RegisterPythonModules(python_wrapper);
    SCRIPT_MANAGER()->Register(python_wrapper);
#endif
}

static bool LuaTests() {
    return SCRIPT_MANAGER()->LoadModule("main").valid();
}

static bool PythonTests() {
    VirtualObj wassup = SCRIPT_MANAGER()->LoadModule("wassup");
    if(!wassup) return false;
    if(!wassup["supimpa"]) return false;

    VirtualObj::List args;
    args.push_back(wassup["vecx"]);
    return wassup["supimpa"](args).valid();
}

int main(int argc, char **argv) {
    ugdk::Configuration config;
    config.base_path = "data/";
    
    InitScripts();
    
    Engine* eng = Engine::reference();
    eng->Initialize(config);

#ifdef UGDK_USING_LUA
    if(!LuaTests()) puts("LUA FAILED!");
#endif

#ifdef UGDK_USING_PYTHON
    PyObject *path = PySys_GetObject("path");
    PyList_Append(path, PyString_FromString("../src/generated"));
    if(!PythonTests()) puts("PYTHON FAILED!");
#endif
    
    eng->Run();
    eng->Release();
    return 0;
}