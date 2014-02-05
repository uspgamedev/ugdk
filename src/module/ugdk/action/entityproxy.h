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
    
	virtual bool to_be_removed() const {
		ugdk::script::VirtualObj to_be_removed = proxy_["to_be_removed"];
		if (to_be_removed)
			return to_be_removed.value<bool>();
		return to_be_removed_; 
	}

    virtual void Update(double dt) {
		if (to_be_removed()) return;
        ( proxy_ | "Update" )(dt);
    }

    virtual void OnSceneAdd(Scene* scene) {
        SceneProxy* sceneproxy = dynamic_cast<SceneProxy*>(scene);
        if (sceneproxy != nullptr) {
            (proxy_ | "OnSceneAdd")(sceneproxy->get_proxy_vobj());
        }
        else {
            (proxy_ | "OnSceneAdd")(scene);
        }
    }
    
};

}
}
#endif
