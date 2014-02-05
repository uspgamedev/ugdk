#ifndef MODULE_PROXY_SCENE_H_
#define MODULE_PROXY_SCENE_H_

#include <ugdk/script/virtualobj.h>
#include <ugdk/action/scene.h>
#include <ugdk/script/baseproxy.h>

namespace ugdk {
namespace action {

class SceneProxy;
class SceneProxy : public Scene, public ugdk::script::BaseProxy<SceneProxy> {
  public:
    SceneProxy(const ugdk::script::VirtualObj& proxy) :
        Scene(), ugdk::script::BaseProxy<SceneProxy>(proxy) {
            this->AddTask([&](double dt){
                ( proxy_ | "Update" )(dt);
                return true;
            });
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
        (proxy_ | "End")();
    }

};

}
}
#endif
