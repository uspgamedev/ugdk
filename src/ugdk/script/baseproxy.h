#ifndef UGDK_MODULE_PROXY_CACHE_H_
#define UGDK_MODULE_PROXY_CACHE_H_

#include <map>
#include <ugdk/script/virtualobj.h>
#

namespace ugdk {
namespace script {


template <class T>
class BaseProxy {
public:
    BaseProxy(const ugdk::script::VirtualObj& proxy) : proxy_(proxy) {}
    ~BaseProxy() {
        BaseProxy::table_.erase(proxy_.unsafe_data());
    }

    ugdk::script::VirtualObj get_proxy_vobj() const { return proxy_; }
    

    static void Set(void* key, T* object) { BaseProxy::table_[key] = object; }

    static bool Check(const ugdk::script::VirtualObj& proxy) {
        return BaseProxy::table_.count(proxy.unsafe_data()) > 0;
    }

    static T* Get(const ugdk::script::VirtualObj& proxy) {
        void* key = proxy.unsafe_data();
        T* obj;
        if (BaseProxy::table_.count(key) == 0) {
            obj = new T(proxy);
            Set(key, obj);
        }
        else {
            obj = BaseProxy::table_[key]; 
        }
        return obj;
    }

    static T* Get(ugdk::script::VirtualData* proxy) {
        void* key = proxy->unsafe_data();
        T* obj;
        if (BaseProxy::table_.count(key) == 0) {
            obj = new T(VirtualObj(VirtualData::Ptr(proxy)));
            Set(key, obj);
        }
        else {
            obj = BaseProxy::table_[key];
        }
        return obj;
    }

protected:
    ugdk::script::VirtualObj proxy_;

private:
    static std::map<void*, T*> table_;
};

template <class T>
std::map<void*, T*> BaseProxy<T>::table_;

}
}
#endif
