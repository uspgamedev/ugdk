#ifndef MODULE_PROXY_OBSERVER_H_
#define MODULE_PROXY_OBSERVER_H_

#include <ugdk/script/virtualobj.h>
#include <list>
#include <ugdk/action/observer.h>

namespace ugdk {

class Observer;

class ObserverProxy : public Observer {
public:
    ObserverProxy(const ugdk::script::VirtualObj& proxy) : proxy_(proxy) {}
    
    virtual void Tick() {
        std::list<ugdk::script::VirtualObj> args;
        ( proxy_ | "Tick" )(args);
    }
    
protected:
    ugdk::script::VirtualObj proxy_;
};

}

#endif
