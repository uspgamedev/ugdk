#ifndef UGDK_RESOURCE_MANAGER_H_
#define UGDK_RESOURCE_MANAGER_H_

#include <ugdk/resource/resourcecontainer.h>
#include <ugdk/system/compatibility.h>

#include <utility>
#include <vector>

namespace ugdk {
namespace resource {

class Manager {
  public:
    Manager();
    ~Manager();
    
    template <class T, class ...Args>
    void CreateContainer(Args... args) {
        ResourceContainer<T>::Create(std::forward<Args>(args)...);
        clear_functions_.push_back(&ResourceContainer<T>::Clear);
    }


    template <class T>
    ResourceContainer<T>* GetContainer() const {
        return ResourceContainer<T>::Get();
    }
    
  private:
    std::vector<void(*)(void)> clear_functions_;
};

} // namespace resource
} // namespace ugdk

#endif // UGDK_RESOURCE_MANAGER_H_

