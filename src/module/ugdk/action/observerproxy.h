#ifndef MODULE_PROXY_OBSERVER_H_
#define MODULE_PROXY_OBSERVER_H_

#include <ugdk/script/virtualobj.h>
#include <ugdk/action/observer.h>
#include <ugdk/script/baseproxy.h>

namespace ugdk {

class ObserverProxy;

class ObserverProxy : public Observer, public ugdk::script::BaseProxy<ObserverProxy> {
public:
    ObserverProxy(const ugdk::script::VirtualObj& proxy) : proxy_(proxy) {}
    
    virtual void Tick() {
        ( proxy_ | "Tick" )();
    }
    
    
    ugdk::script::VirtualObj get_proxy_vobj() const { return proxy_; }
    
protected:
    ugdk::script::VirtualObj proxy_;
};

}

#endif
