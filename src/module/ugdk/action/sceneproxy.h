
#ifndef MODULE_PROXY_SCENE_H_
#define MODULE_PROXY_SCENE_H_

#include <cstdio>

#include <ugdk/script/virtualobj.h>
#include <ugdk/action/scene.h>
#include <ugdk/script/baseproxy.h>

namespace ugdk {
namespace action {

class SceneProxy;

class SceneProxy : public Scene, public ugdk::script::BaseProxy<SceneProxy> {
  public:
    SceneProxy(const ugdk::script::VirtualObj& proxy) :
        Scene(), ugdk::script::BaseProxy<SceneProxy>(proxy) {}

    virtual void Focus() { 
        Scene::Focus();
        printf("SCENE (0x%p)\n", this);
        (proxy_ | "Focus")();
    }
    virtual void DeFocus() {
        Scene::DeFocus();
        (proxy_ | "DeFocus")();
    }
    
    virtual void Update(double delta_t) {
        Scene::Update(delta_t);
        ugdk::script::VirtualObj vdt = ugdk::script::VirtualObj(proxy_.wrapper());
        vdt.set_value(delta_t);
        std::list<ugdk::script::VirtualObj> args;
        args.push_back(vdt);
        ( proxy_ | "Update" )(args);
    }
    
    virtual void End() { 
        Scene::End();
        printf("SCENE (%p)\n", this);
        printf("END-BEFORE (%p)\n", proxy_.unsafe_data());
        script::VirtualObj::List args;
        //proxy_["End"]((proxy_, args));
        (proxy_ | "End")(args);
        puts("END-AFTER");
    }

};

}
}
#endif
