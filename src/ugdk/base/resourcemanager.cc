
#include <ugdk/base/resourcemanager.h>

#include <ugdk/action/animationset.h>
#include <ugdk/base/engine.h>
#include <ugdk/base/genericcontainer.h>
#include <ugdk/graphic/spritesheet.h>
#include <ugdk/graphic/texture.h>
#include <ugdk/util/animationprotocol.h>
#include <ugdk/util/languageword.h>
#include <ugdk/util/gdd/cachedloader.h>

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

bool type_info_cmp(const std::type_info* a, const std::type_info* b) {
    return a->before(*b);
}

namespace ugdk {
namespace base {

typedef gdd::CachedLoader<AnimationSet> AnimationLoader;

ResourceManager::ResourceManager() : containers_(type_info_cmp) {
    add_container(new GenericContainer<graphic::Texture*>(graphic::Texture::CreateFromFile));
    add_container(new GenericContainer<graphic::Spritesheet*>(NullLoad<graphic::Spritesheet>));
    add_container(new AnimationLoader(new AnimationProtocol));
    add_container(new GenericContainer<LanguageWord*>(NullLoad<LanguageWord>));
}

ResourceManager::~ResourceManager() {
    for(ResourceMap::iterator it = containers_.begin();
        it != containers_.end(); ++it)
        delete it->second;
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

AnimationSet*         ResourceManager::GetAnimationSetFromFile  (const std::string& file) {
    return RESOURCE_MANAGER()->animation_loader().Load(file, file);
}

graphic::Text*        ResourceManager::CreateTextFromLanguageTag(const std::string& tag) {
    return RESOURCE_MANAGER()->word_container().Find(tag)->GenerateText();
}

} // namespace base
} // namespace ugdk
