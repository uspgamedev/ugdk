
#include <ugdk/base/resourcemanager.h>

#include <ugdk/action/animationset.h>
#include <ugdk/base/engine.h>
#include <ugdk/base/genericcontainer.h>
#include <ugdk/graphic/spritesheet.h>
#include <ugdk/graphic/texture.h>
#include <ugdk/util/animationprotocol.h>
#include <ugdk/util/languageword.h>

#ifdef DEBUG
#include <cstdio>
#include <typeinfo>
#define TOSTRING(X) typeid(X).name()
#endif

template <class T>
static T* NullLoad(const std::string& filepath) { 
#ifdef DEBUG
    fprintf(stderr, "Warning: NullLoad<%s> called with path '%s'.", TOSTRING(T), filepath.c_str());
#endif
    return NULL;
}

namespace ugdk {
namespace base {

ResourceManager::ResourceManager() 
    : 
    texture_container_(new GenericContainer<graphic::Texture*>(graphic::Texture::CreateFromFile)),
    spritesheet_container_(new GenericContainer<graphic::Spritesheet*>(NullLoad<graphic::Spritesheet>)),
    animation_loader_(new AnimationLoader(new AnimationProtocol)),
    word_container_(new GenericContainer<LanguageWord*>(NullLoad<LanguageWord>)) {}

ResourceManager::~ResourceManager() {
    delete texture_container_;
    delete spritesheet_container_;
    delete animation_loader_;
    delete word_container_;
}

graphic::Texture*     ResourceManager::GetTextureFromTag        (const std::string& tag) {
    return RESOURCE_MANAGER()->texture_container().Find(tag);
}

graphic::Texture*     ResourceManager::GetTextureFromFile       (const std::string& file) {
    return RESOURCE_MANAGER()->texture_container().Load(file, file);
}

graphic::Spritesheet* ResourceManager::GetSpritesheetFromTag    (const std::string& tag) {
    return RESOURCE_MANAGER()->spritesheet_container().Find(tag);
}

graphic::Text*        ResourceManager::CreateTextFromLanguageTag(const std::string& tag) {
    return RESOURCE_MANAGER()->word_container().Find(tag)->GenerateText();
}

} // namespace base
} // namespace ugdk
