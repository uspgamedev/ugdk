#include <ugdk/config/config.h>

#ifdef UGDK_USING_PYTHON
#include <Python.h>
#endif

#include <cstdlib>

#define MODULE_AUTO_LOAD(ACTION) ACTION(UGDK)

#include "SDL.h"
#include <ugdk/base/engine.h>
#include <ugdk/script.h>
#include <ugdk/script/scriptmanager.h>
#include <ugdk/script/virtualobj.h>

using ugdk::Engine;
using ugdk::script::VirtualObj;

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
    
    ugdk::script::InitScripts();
    
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