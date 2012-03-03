
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
#include <ugdk/graphic/node.h>
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
        args.push_back(vdt);
        proxy_["Update"](args);
    }

	ugdk::graphic::Node* get_node() {
		return proxy_["Node"].value<ugdk::graphic::Node*>();
	}
private:
    VirtualObj proxy_;
};

class ScriptEntityStack {
public:
	virtual ~ScriptEntityStack() {}
	ScriptEntityStack(const VirtualObj& proxy) :
		proxy_(proxy) {}

	int size() {
		std::vector<VirtualObj> args;
		return proxy_["__len__"](args).value<int>();
	}

	ScriptEntity* pop() {
		std::vector<VirtualObj> args;
		return new ScriptEntity(proxy_["pop"](args));
	}
private:
    VirtualObj proxy_;
};

class ScriptScene : public ugdk::Scene {
public:
	ScriptScene() : ugdk::Scene() {
		map_generator_ = SCRIPT_MANAGER()->LoadModule("MapGenerator");
	}

	void GenerateMap() {
		std::vector<VirtualObj> args;
		ScriptEntityStack objects = ScriptEntityStack( map_generator_["Generate"](args) );

		ScriptEntity* ent;
		while ( objects.size() > 0 ) {
			ent = objects.pop();
			this->AddEntity(ent);
			this->content_node()->AddChild(ent->get_node());
		}
	}

private:
	VirtualObj map_generator_;
};

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



int main(int argc, char *argv[]) {

    ugdk::Configuration engine_config;
    engine_config.window_title = "Asteroids";
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

    ugdk::Engine::reference()->Initialize(engine_config);

    ScriptScene* scene = new ScriptScene();
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
