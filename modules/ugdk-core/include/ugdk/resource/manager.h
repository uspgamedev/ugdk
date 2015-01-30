#ifndef UGDK_RESOURCE_MANAGER_H_
#define UGDK_RESOURCE_MANAGER_H_

#include <typeindex>
#include <unordered_map>
#include <ugdk/resource/resourcecontainer.h>

namespace ugdk {
namespace resource {

class Manager {
  public:
    Manager();
    ~Manager();
    
    // Generic Methods
    template <class T>
    void add_container(ResourceContainer<T>* container) {
        containers_[typeid(T)] = container;
    }

    template <class T>
    ResourceContainer<T>* get_container() {
        ResourceMap::iterator base = containers_.find(typeid(T));
        if(base == containers_.end())
            return nullptr;
        return dynamic_cast<ResourceContainer<T>*>(base->second);
    }
    
  private:
    typedef std::unordered_map<std::type_index, ResourceContainerBase*> ResourceMap;
    ResourceMap containers_;
};

} // namespace resource
} // namespace ugdk

#endif // UGDK_RESOURCE_MANAGER_H_

