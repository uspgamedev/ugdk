#ifndef MODULE_PROXY_ENTITY_H_
#define MODULE_PROXY_ENTITY_H_

#include <ugdk/action/entity.h>
#include <ugdk/script/baseproxy.h>
#include <list>
#include <module/ugdk/action/sceneproxy.h>

namespace ugdk {
namespace action {

class EntityProxy;

class EntityProxy : public Entity, public ugdk::script::BaseProxy<EntityProxy> {
public:
    EntityProxy(const ugdk::script::VirtualObj& proxy) : ugdk::script::BaseProxy<EntityProxy>(proxy) {}
    
    virtual void Update(double dt) {
        ugdk::script::VirtualObj vdt = ugdk::script::VirtualObj(proxy_.wrapper());
        vdt.set_value(dt);
        std::list<ugdk::script::VirtualObj> args;
        args.push_back(vdt);
        ( proxy_ | "Update" )(args);
    }

    virtual void OnSceneAdd(Scene* scene) {
        std::list<ugdk::script::VirtualObj> args;

        SceneProxy* sceneproxy = dynamic_cast<SceneProxy*>(scene);
        if (sceneproxy != NULL) {
            args.push_back( sceneproxy->get_proxy_vobj() );
        }
        else {
            ugdk::script::VirtualObj vscene = ugdk::script::VirtualObj(proxy_.wrapper());
            vscene.set_value<Scene*>(scene);
            args.push_back( vscene );
        }

        if(proxy_["OnSceneAdd"])
            ( proxy_ | "OnSceneAdd" )(args);
    }
    
};

}
}
#endif
