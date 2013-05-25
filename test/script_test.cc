#include <ugdk/common/config.h>

#ifdef UGDK_PYTHON_ENABLED
#include <Python.h>
#endif

#include <cstdlib>

#include "SDL.h"
#include <ugdk/system/engine.h>
#include <ugdk/script.h>
#include <ugdk/script/scriptmanager.h>
#include <ugdk/script/virtualobj.h>

using ugdk::Engine;
using ugdk::script::VirtualObj;

static bool LuaTests() {
    VirtualObj main = SCRIPT_MANAGER()->LoadModule("main");
    if(!main) return false;
    VirtualObj val = main["integer"];

#ifdef UGDK_USING_VARIADIC
    auto f = main["soma"].AsFunction<int, int, int>();
    assert(f(13, 25) == 38);
    assert(main["soma"].Call(3, val).value<int>() == 45);
    return main["soma"].Call(3, 8).value<int>() == 11;
#endif
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
    
    ugdk::script::InitScripts();
    
    Engine* eng = Engine::reference();
    eng->Initialize(config);

#ifdef UGDK_LUA_ENABLED
    if(!LuaTests()) puts("LUA FAILED!");
#endif

#ifdef UGDK_PYTHON_ENABLED
    PyObject *path = PySys_GetObject("path");
    PyList_Append(path, PyString_FromString("../src/generated"));
    if(!PythonTests()) puts("PYTHON FAILED!");
#endif
    
    eng->Run();
    eng->Release();
    return 0;
}