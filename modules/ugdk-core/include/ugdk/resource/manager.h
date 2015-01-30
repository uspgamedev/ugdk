#ifndef UGDK_RESOURCE_MANAGER_H_
#define UGDK_RESOURCE_MANAGER_H_

#include <typeindex>
#include <unordered_map>
#include <memory>
#include <ugdk/resource/resourcecontainer.h>
#include <ugdk/resource/genericcontainer.h>

namespace ugdk {
namespace resource {

class Manager {
  public:
    Manager();
    ~Manager();
    
    void AddContainer(std::type_index type, std::unique_ptr<ResourceContainerBase> container) {
        containers_[type] = std::move(container);
    }

    template <class T>
    void AddContainer(std::unique_ptr<ResourceContainer<T>> container) {
        AddContainer(typeid(T), std::move(container));
    }

    template <class T, class ...Args>
    void CreateContainer(Args... args) {
        containers_[typeid(T)] = MakeUnique<GenericContainer<T>>(std::forward<Args>(args)...);
    }


    template <class T>
    ResourceContainer<T>* GetContainer() const {
        auto base = containers_.find(typeid(T));
        if(base == containers_.end())
            return nullptr;
        return dynamic_cast<ResourceContainer<T>*>(base->second.get());
    }
    
  private:
    std::unordered_map<std::type_index, std::unique_ptr<ResourceContainerBase>> containers_;
};

} // namespace resource
} // namespace ugdk

#endif // UGDK_RESOURCE_MANAGER_H_

