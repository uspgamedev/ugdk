#ifndef MODULE_PROXY_COLLISIONLOGIC_H_
#define MODULE_PROXY_COLLISIONLOGIC_H_

#include <ugdk/script/virtualobj.h>
#include <pyramidworks/collision/collisionlogic.h>

namespace pyramidworks {
namespace collision {

class CollisionLogic;

class CollisionLogicProxy : public CollisionLogic {
public:
    CollisionLogicProxy(const ugdk::script::VirtualObj& proxy) : proxy_(proxy) {}
    
    virtual void Handle(void *data) {
        ugdk::script::VirtualObj vdata = ugdk::script::VirtualObj(proxy_.wrapper());
        vdata.set_value(data);
        std::list<ugdk::script::VirtualObj> args;
        args.push_back(vdata);
        ( proxy_ | "Handle" )(args);
    }
    
protected:
    ugdk::script::VirtualObj proxy_;
};

}
}

#endif
