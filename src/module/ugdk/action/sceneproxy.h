
#ifndef MODULE_PROXY_SCENE_H_
#define MODULE_PROXY_SCENE_H_

#include <cstdio>

#include <ugdk/script/virtualobj.h>
#include <ugdk/action/scene.h>
#include <ugdk/action/task.h>
#include <ugdk/script/baseproxy.h>

namespace ugdk {
namespace action {

class ScriptUpdateTask : public Task {
public:
    ScriptUpdateTask(const ugdk::script::VirtualObj& scene_proxy) : scene_proxy_(scene_proxy) {}
    ~ScriptUpdateTask() {}
    
    void operator()(double dt) {
        ugdk::script::VirtualObj vdt = ugdk::script::VirtualObj(scene_proxy_.wrapper());
        vdt.set_value(dt);
        std::list<ugdk::script::VirtualObj> args;
        args.push_back(vdt);
        ( scene_proxy_ | "Update" )(args);
    }

private:
    ugdk::script::VirtualObj scene_proxy_;
};

class SceneProxy;
class SceneProxy : public Scene, public ugdk::script::BaseProxy<SceneProxy> {
  public:
    SceneProxy(const ugdk::script::VirtualObj& proxy) :
        Scene(), ugdk::script::BaseProxy<SceneProxy>(proxy) {
            this->AddTask(new ScriptUpdateTask(proxy));
        }

    virtual void Focus() { 
        Scene::Focus();
        (proxy_ | "Focus")();
    }
    virtual void DeFocus() {
        Scene::DeFocus();
        (proxy_ | "DeFocus")();
    }
    
    virtual void End() { 
        Scene::End();
        script::VirtualObj::List args;
        (proxy_ | "End")(args);
    }

};

}
}
#endif
