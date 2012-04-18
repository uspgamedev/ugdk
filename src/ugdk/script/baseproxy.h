#ifndef UGDK_MODULE_PROXY_CACHE_H_
#define UGDK_MODULE_PROXY_CACHE_H_

#include <map>
#include <ugdk/script/virtualobj.h>

namespace ugdk {
namespace script {


template <class T>
class BaseProxy {
public:
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

private:
    static std::map<void*, T*> table_;
};

template <class T>
std::map<void*, T*> BaseProxy<T>::table_;

}
}
#endif
