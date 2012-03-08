
#include <ugdk/base/texturecontainer.h>
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
    : 
    texture_container_(new TextureContainer),
    spritesheet_container_(new ResourceContainer<graphic::Spritesheet*>),
    animation_loader_(new AnimationLoader(new AnimationProtocol)),
    word_container_(new ResourceContainer<LanguageWord*>) {}

ResourceManager::~ResourceManager() {
    delete texture_container_;
    delete spritesheet_container_;
    delete animation_loader_;
    delete word_container_;
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
