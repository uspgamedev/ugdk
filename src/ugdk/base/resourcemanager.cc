
#include <ugdk/base/resourcemanager.h>

#include <ugdk/action/animationset.h>
#include <ugdk/graphic/spritesheet.h>
#include <ugdk/graphic/texture.h>
#include <ugdk/util/animationprotocol.h>

namespace ugdk {
namespace base {

ResourceManager::ResourceManager() 
    : animation_loader_(new AnimationLoader(new AnimationProtocol)) {}

ResourceManager::~ResourceManager() {
    animation_loader_->ClearCache();
}

} // namespace base
} // namespace ugdk
