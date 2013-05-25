#ifndef UGDK_RESOURCE_MANAGER_H_
#define UGDK_RESOURCE_MANAGER_H_

#include <typeinfo>
#include <map>
#include <ugdk/action.h>
#include <ugdk/action/spritetypes.h>
#include <ugdk/resource/resourcecontainer.h>
#include <ugdk/graphic.h>
#include <ugdk/util.h>

// Convenience for users.
#include <ugdk/util/languageword.h>

namespace ugdk {
namespace resource {

class Manager {
  public:
    Manager();
    ~Manager();
    
    // Generic Methods
    template <class T>
    void add_container(ResourceContainer<T>* container) {
        containers_[&typeid(T)] = container;
    }

    template <class T>
    ResourceContainer<T>& get_container() {
        ResourceContainerBase* base = containers_[&typeid(T)];
        ResourceContainer<T>* container = static_cast<ResourceContainer<T>*>(base);
        return *container;
    }

    ResourceContainer<graphic::Texture*>& texture_container() { 
        return get_container<graphic::Texture*>();
    }
    ResourceContainer<graphic::Spritesheet*>& spritesheet_container() {
        return get_container<graphic::Spritesheet*>();
    }
    ResourceContainer<action::SpriteAnimationTable*>& spriteanimation_loader() {
        return get_container<action::SpriteAnimationTable*>();
    }
    ResourceContainer<LanguageWord*>& word_container() { 
        return get_container<LanguageWord*>();
    }
    
  private:
    typedef std::map<const std::type_info*, ResourceContainerBase*, 
        bool (*)(const std::type_info*, const std::type_info*) > ResourceMap;
    ResourceMap containers_;
};

} // namespace resource
} // namespace ugdk

#endif // UGDK_RESOURCE_MANAGER_H_

