#ifndef UGDK_BASE_RESOURCEMANAGER_H_
#define UGDK_BASE_RESOURCEMANAGER_H_

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
    
    // Non-const
    ResourceContainer<graphic::Texture*>&     texture_container()     { return *texture_container_;    }
    ResourceContainer<graphic::Spritesheet*>& spritesheet_container() { return *spritesheet_container_;}
    ResourceContainer<AnimationSet*>&         animation_loader()      { return *animation_loader_;     }
    ResourceContainer<LanguageWord*>&         word_container()        { return *word_container_;       }


    static graphic::Texture*     GetTextureFromTag        (const std::string& tag);
    static graphic::Texture*     GetTextureFromFile       (const std::string& file);
    static graphic::Spritesheet* GetSpritesheetFromTag    (const std::string& tag);
    static AnimationSet*         GetAnimationSetFromFile  (const std::string& file);
    static graphic::Text*        CreateTextFromLanguageTag(const std::string& tag);
    
  private:
    ResourceContainer<graphic::Texture*>*     texture_container_;
    ResourceContainer<graphic::Spritesheet*>* spritesheet_container_;
    ResourceContainer<AnimationSet*>*         animation_loader_;
    ResourceContainer<LanguageWord*>*         word_container_;
};

} // namespace base
} // namespace ugdk

#endif // UGDK_BASE_RESOURCEMANAGER_H_

