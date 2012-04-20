#ifndef MODULE_PROXY_ENTITY_H_
#define MODULE_PROXY_ENTITY_H_

#include <ugdk/action/entity.h>
#include <ugdk/script/baseproxy.h>
#include <list>

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
    
};

}
}
#endif
