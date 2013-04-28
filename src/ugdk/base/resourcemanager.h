#ifndef UGDK_BASE_RESOURCEMANAGER_H_
#define UGDK_BASE_RESOURCEMANAGER_H_

#include <typeinfo>
#include <map>
#include <ugdk/action.h>
#include <ugdk/action/spritetypes.h>
#include <ugdk/base/resourcecontainer.h>
#include <ugdk/graphic.h>
#include <ugdk/util.h>

// Convenience for users.
#include <ugdk/util/languageword.h>

#define RESOURCE_MANAGER()  (ugdk::Engine::reference()->resource_manager())

namespace ugdk {
namespace base {

class ResourceManager {
  public:
    ResourceManager();
    ~ResourceManager();
    
    static graphic::Texture*                GetTextureFromTag        (const std::string& tag);
    static graphic::Texture*                GetTextureFromFile       (const std::string& file);
    static graphic::Spritesheet*            GetSpritesheetFromTag    (const std::string& tag);
    static action::SpriteAnimationTable*    GetSpriteAnimationTableFromFile(const std::string& file);
    static ugdk::LanguageWord*              GetLanguageWord(const std::string& tag);

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

    ResourceContainer<graphic::Texture*>&               texture_container()     { return get_container<graphic::Texture*>();     }
    ResourceContainer<graphic::Spritesheet*>&           spritesheet_container() { return get_container<graphic::Spritesheet*>(); }
    ResourceContainer<action::SpriteAnimationTable*>&   spriteanimation_loader(){ return get_container<action::SpriteAnimationTable*>(); }
    ResourceContainer<LanguageWord*>&                   word_container()        { return get_container<LanguageWord*>();         }
    
  private:
    typedef std::map<const std::type_info*, ResourceContainerBase*, 
        bool (*)(const std::type_info*, const std::type_info*) > ResourceMap;
    ResourceMap containers_;
};

} // namespace base
} // namespace ugdk

#endif // UGDK_BASE_RESOURCEMANAGER_H_

