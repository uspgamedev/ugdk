
#include <ugdk/base/resourcemanager.h>

#include <ugdk/action/animationset.h>
#include <ugdk/base/engine.h>
#include <ugdk/graphic/spritesheet.h>
#include <ugdk/graphic/texture.h>
#include <ugdk/util/animationprotocol.h>
#include <ugdk/util/languageword.h>

namespace ugdk {
namespace base {

ResourceManager::ResourceManager() 
    : animation_loader_(new AnimationLoader(new AnimationProtocol)) {}

ResourceManager::~ResourceManager() {
    delete animation_loader_;
}

graphic::Texture*     ResourceManager::GetTextureFromTag        (const std::string& tag) {
    return RESOURCE_MANAGER()->texture_container().Find(tag);
}

graphic::Spritesheet* ResourceManager::GetSpritesheetFromTag    (const std::string& tag) {
    return RESOURCE_MANAGER()->spritesheet_container().Find(tag);
}

graphic::Text*        ResourceManager::CreateTextFromLanguageTag(const std::string& tag) {
    return RESOURCE_MANAGER()->word_container().Find(tag)->GenerateText();
}

} // namespace base
} // namespace ugdk
