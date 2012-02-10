#ifndef UGDK_BASE_RESOURCEMANAGER_H_
#define UGDK_BASE_RESOURCEMANAGER_H_

#include <ugdk/action.h>
#include <ugdk/base/resourcecontainer.h>
#include <ugdk/base/texturecontainer.h>
#include <ugdk/graphic.h>
#include <ugdk/util/gdd/cachedloader.h>

#define RESOURCE_MANAGER()  (ugdk::Engine::reference()->resource_manager())

namespace ugdk {
namespace base {

typedef gdd::CachedLoader<AnimationSet> AnimationLoader;

class ResourceManager {
  public:
    ResourceManager();
    ~ResourceManager();
    
    TextureContainer&                         texture_container()     { return texture_container_;     }
    ResourceContainer<graphic::Spritesheet*>& spritesheet_container() { return spritesheet_container_; }
    ResourceContainer<std::string>&           string_container()      { return string_container_;      }
    AnimationLoader&                          animation_loader()      { return *animation_loader_;     }
    
  private:
    TextureContainer                         texture_container_;
    ResourceContainer<graphic::Spritesheet*> spritesheet_container_;
    ResourceContainer<std::string>           string_container_;
    AnimationLoader*                         animation_loader_;
};

} // namespace base
} // namespace ugdk

#endif // UGDK_BASE_RESOURCEMANAGER_H_

