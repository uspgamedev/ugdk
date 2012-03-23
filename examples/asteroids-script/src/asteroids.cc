
#include <cerrno>
#include <sys/stat.h>

#include <string>
#include <list>

#include <ugdk/base/engine.h>
#include <ugdk/base/configuration.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/textmanager.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/action/entity.h>
#include <ugdk/action/scene.h>
#include <ugdk/graphic/node.h>
#include <ugdk/modules.h>

#include <ugdk/graphic/drawable.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>
#include <ugdk/graphic/texture.h>
#include <ugdk/base/texturecontainer.h>
#include <ugdk/base/resourcemanager.h>

#include <ugdk/script/scriptmanager.h>
#include <ugdk/script/langwrapper.h>
#include <ugdk/script/virtualobj.h>
#include <ugdk/script/lua/luawrapper.h>
#include <ugdk/script/lua/header.h>
#include <ugdk/script/python/pythonwrapper.h>

#include "scriptscene.h"

using ugdk::Vector2D;
using ugdk::script::VirtualObj;

/////////////////////////////////////////////////////////////////

static void InitScripts() {
    using ugdk::script::lua::LuaWrapper;
    using ugdk::script::python::PythonWrapper;

    //inicializando lua
    LuaWrapper* lua_wrapper = new LuaWrapper();
    ugdk::RegisterLuaModules(lua_wrapper);
    SCRIPT_MANAGER()->Register("Lua", lua_wrapper);

    //inicializando python
    PythonWrapper* py_wrapper = new PythonWrapper();
    ugdk::RegisterPythonModules(py_wrapper);
    SCRIPT_MANAGER()->Register("Python", py_wrapper);
}

static void SurpriseLuaTest() {

    puts("=== START Surprise Lua tests! ===");
    VirtualObj obj = SCRIPT_MANAGER()->LoadModule("main");

    puts("Checking results...");
    ugdk::Vector2D* vec = obj["v"].value<ugdk::Vector2D*>();
    if (!vec) puts("FAILED.");
    else {
        printf("Result 1: ( %f , %f )\n", vec->x, vec->y);
    }

    string text = obj["str"].value<string>();
    if (!text.size()) puts("FAILED TEXT.");
    else printf("Result 2: %s\n", text.c_str());

    bool boolean = obj["bool"].value<bool>();
    if (!boolean) puts("FAILED BOOLEAN.");
    else printf("Result 3: %d\n", boolean);

    int integer = obj["integer"].value<int>();
    if (!integer) puts("FAILED INTEGER.");
    else printf("Result 4: %d\n", integer);

    double number = obj["number"].value<double>();
    if (!number) puts("FAILED NUMBER.");
    else printf("Result 5: %f\n", number);

    obj["ls"](VirtualObj::List(1,obj));

    puts("Printing list...");
    VirtualObj::List objlist = obj["list"].value<VirtualObj::List>();
    if (!objlist.size()) puts("FAILED List.");
    else for (VirtualObj::List::iterator it = objlist.begin();
              it != objlist.end();
              ++it)
        obj["print"](VirtualObj::List(1,*it));

    puts("Printing table...");
    VirtualObj::Map objmap = obj["map"].value<VirtualObj::Map>();
    if (!objmap.size()) puts("FAILED Map.");
    else for (VirtualObj::Map::iterator it = objmap.begin();
              it != objmap.end();
              ++it)
        obj["print"]((it->first, it->second));

    VirtualObj obj2(obj.wrapper());

    obj2.set_value("hahahahaha");
    obj["print"](VirtualObj::List(1,obj2));

    puts("=== Lua tests are finished. ===");
}


int main(int argc, char *argv[]) {

    ugdk::Configuration engine_config;
    engine_config.window_title = "Asteroids";
    engine_config.window_size  = Vector2D(640.0, 480.0);
    engine_config.fullscreen   = false;
    engine_config.base_path = "./data/";
    struct stat st;
    // Removing the trailing slash.
    int s = stat(engine_config.base_path.substr(0, engine_config.base_path.size() - 1).c_str(), &st);
    if(s < 0 && errno == ENOENT)
        engine_config.base_path = "./";
    engine_config.window_icon = "";

    InitScripts();
    ugdk::Engine::reference()->Initialize(engine_config);

    SurpriseLuaTest();

    VirtualObj config = SCRIPT_MANAGER()->LoadModule("Config");

    Vector2D* resolution = config["resolution"].value<Vector2D*>();

    VIDEO_MANAGER()->ChangeResolution(*resolution, config["fullscreen"].value<bool>());
    

	printf("Size in c++ = (%f, %f)\n", ugdk::Engine::reference()->video_manager()->video_size().x,
										ugdk::Engine::reference()->video_manager()->video_size().y);

    asteroids::ScriptScene* scene = new asteroids::ScriptScene();
    scene->GenerateMap();

    ugdk::Engine::reference()->PushScene(scene);
    // Transfers control to the framework.
    ugdk::Engine::reference()->Run();

    // Releases all loaded textures, to avoid issues when changing resolution.
    ugdk::Engine::reference()->video_manager()->Release();
    ugdk::Engine::reference()->text_manager()->Release();

    ugdk::Engine::reference()->Release();

    return 0;
}
