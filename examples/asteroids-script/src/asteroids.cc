
#include <cerrno>
#include <sys/stat.h>

#include <string>

#include <ugdk/base/engine.h>
#include <ugdk/base/configuration.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/textmanager.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/action/entity.h>
#include <ugdk/action/scene.h>
#include <ugdk/modules.h>

#include <ugdk/script/scriptmanager.h>
#include <ugdk/script/langwrapper.h>
#include <ugdk/script/virtualobj.h>
#include <ugdk/script/lua/luawrapper.h>
#include <ugdk/script/lua/header.h>
#include <ugdk/script/python/pythonwrapper.h>

using ugdk::Vector2D;
using ugdk::script::VirtualObj;

class ScriptEntity : public ugdk::Entity {
  public:
    virtual ~ScriptEntity() {}
    ScriptEntity(const VirtualObj& proxy) :
        proxy_(proxy) {}
    void Update(double dt) {
        VirtualObj vdt = VirtualObj(proxy_.wrapper());
        vdt.set_value(dt);
        std::vector<VirtualObj> args;
        args.push_back(proxy_);
        args.push_back(vdt);
        proxy_["Update"](args);
    }
  private:
    VirtualObj proxy_;
};

static ugdk::Engine* engine() {
    return ugdk::Engine::reference();
}

static void InitScripts() {

    using ugdk::script::LangWrapper;
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

static void RunTests() {


    // testando lua
    {
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

        obj["ls"](std::vector<VirtualObj>(1,obj));

        VirtualObj obj2(obj.wrapper());

        obj2.set_value("hahahahaha");
        obj["print"](std::vector<VirtualObj>(1,obj2));

        puts("=== Lua tests are finished. ===");

    }

    //testando python
    printf("Python test starting...\n");
    VirtualObj wassup = SCRIPT_MANAGER()->LoadModule("wassup");
    printf("MARK got wassup\n");
    VirtualObj pyVecx = wassup["vecx"];
    printf("MARK got python vecx\n");
    ugdk::Vector2D* vecx = pyVecx.value<ugdk::Vector2D*>();
    printf("MARK converted vecx to C++ Vector2D object\n");
    printf("X: ( %f , %f )\n", vecx->x, vecx->y);

    wassup["supimpa"](std::vector<VirtualObj>(1,pyVecx));

    printf("Python test finished. \n");
}

int main(int argc, char *argv[]) {

    ugdk::Configuration engine_config;
    engine_config.window_title = "Script System Custom Tests";
    engine_config.window_size  = Vector2D(800.0, 600.0);
    engine_config.fullscreen   = false;

    engine_config.base_path = "./data/";

    struct stat st;
    // Removing the trailing slash.
    int s = stat(engine_config.base_path.substr(0, engine_config.base_path.size() - 1).c_str(), &st);
    if(s < 0 && errno == ENOENT)
        engine_config.base_path = "./";

    engine_config.window_icon = "";

    InitScripts();

    engine()->Initialize(engine_config);

    RunTests();

    ugdk::Scene* scene = new ugdk::Scene();
    ScriptEntity *entity = NULL;
    {
        VirtualObj temp = SCRIPT_MANAGER()->LoadModule("entity");
        entity = new ScriptEntity(temp["entity"]);
    }
    scene->AddEntity(entity);
    engine()->PushScene(scene);
    // Transfers control to the framework.
    engine()->Run();

    // Releases all loaded textures, to avoid issues when changing resolution.
    engine()->video_manager()->Release();
    engine()->text_manager()->Release();

    engine()->Release();

    return 0;
}
