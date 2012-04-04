#ifndef UGDK_BASE_RESOURCEMANAGER_H_
#define UGDK_BASE_RESOURCEMANAGER_H_

#ifdef DEBUG
#include <cstdio>
#include <stdint.h>
#endif
#include <typeinfo>
#include <map>
#include <ugdk/action.h>
#include <ugdk/base/resourcecontainer.h>
#include <ugdk/graphic.h>
#include <ugdk/util.h>

#define RESOURCE_MANAGER()  (ugdk::Engine::reference()->resource_manager())

namespace ugdk {
namespace base {

class ResourceManager {
  public:
    ResourceManager();
    ~ResourceManager();
    
    static graphic::Texture*     GetTextureFromTag        (const std::string& tag);
    static graphic::Texture*     GetTextureFromFile       (const std::string& file);
    static graphic::Spritesheet* GetSpritesheetFromTag    (const std::string& tag);
    static AnimationSet*         GetAnimationSetFromFile  (const std::string& file);
    static graphic::Text*        CreateTextFromLanguageTag(const std::string& tag);

    // Generic Methods
    template <class T>
    void add_container(ResourceContainer<T>* container) {
#ifdef DEBUG
		fprintf(stdout, "UGDK::ResourceManager - Log: add_container<%s>(%X)\n", 
				typeid(T).name(),
				reinterpret_cast<uintptr_t>(container));
#endif
        containers_[&typeid(T)] = container;
    }

    template <class T>
    ResourceContainer<T>& get_container() {
		ResourceContainer<T>* container = static_cast<ResourceContainer<T>*>(containers_[&typeid(T)]);
#ifdef DEBUG
		fprintf(stdout, "UGDK::ResourceManager - Log: get_container<%s>(%X)\n", 
				typeid(T).name(),
				reinterpret_cast<uintptr_t>(container));
#endif
        return *container;
    }

    // Retro-compatibility
    ResourceContainer<graphic::Texture*>&     texture_container()     { return get_container<graphic::Texture*>();     }
    ResourceContainer<graphic::Spritesheet*>& spritesheet_container() { return get_container<graphic::Spritesheet*>(); }
    ResourceContainer<AnimationSet*>&         animation_loader()      { return get_container<AnimationSet*>();         }
    ResourceContainer<LanguageWord*>&         word_container()        { return get_container<LanguageWord*>();         }
    
  private:
    std::map<const std::type_info*, ResourceContainerBase*> containers_;
};

} // namespace base
} // namespace ugdk

#endif // UGDK_BASE_RESOURCEMANAGER_H_

