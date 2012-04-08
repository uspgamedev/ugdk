#ifndef MODULE_PROXY_ENTITY_H_
#define MODULE_PROXY_ENTITY_H_

#include <ugdk/script/virtualobj.h>
#include <ugdk/action/entity.h>
#include <list>

namespace ugdk {

class Entity;

class EntityProxy : public Entity {
public:
    EntityProxy(const ugdk::script::VirtualObj& proxy) : proxy_(proxy) {}
    
    virtual void Update(double dt) {
        ugdk::script::VirtualObj vdt = ugdk::script::VirtualObj(proxy_.wrapper());
        vdt.set_value(dt);
        std::list<ugdk::script::VirtualObj> args;
        args.push_back(vdt);
        ( proxy_ | "Update" )(args);
    }
    
    
    ugdk::script::VirtualObj get_proxy_vobj() const { return proxy_; }
    
protected:
    ugdk::script::VirtualObj proxy_;
};

}

#endif
