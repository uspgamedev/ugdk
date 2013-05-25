#include <ugdk/resource/manager.h>

#include <ugdk/action/spriteanimationframe.h>
#include <ugdk/graphic/spritesheet.h>
#include <ugdk/graphic/texture.h>
#include <ugdk/resource/genericcontainer.h>
#include <ugdk/resource/module.h>
#include <ugdk/system/engine.h>
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
    return nullptr;
}

bool type_info_cmp(const std::type_info* a, const std::type_info* b) {
    return (a->before(*b) != 0);
}

namespace ugdk {
namespace resource {

Manager::Manager() : containers_(type_info_cmp) {
    add_container(new GenericContainer<graphic::Texture*>(graphic::Texture::CreateFromFile));
    add_container(new GenericContainer<graphic::Spritesheet*>(graphic::CreateSpritesheetFromTag));
    add_container(new GenericContainer<action::SpriteAnimationTable*>(action::LoadSpriteAnimationTableFromFile));
    add_container(new GenericContainer<LanguageWord*>(NullLoad<LanguageWord>));
}

Manager::~Manager() {
    for(ResourceMap::iterator it = containers_.begin();
        it != containers_.end(); ++it)
        delete it->second;
}

graphic::Texture*     Manager::GetTextureFromTag        (const std::string& tag) {
    return resource::manager()->texture_container().Find(tag);
}

graphic::Texture*     Manager::GetTextureFromFile       (const std::string& file) {
    return resource::manager()->texture_container().Load(file, file);
}

graphic::Spritesheet* Manager::GetSpritesheetFromTag    (const std::string& tag) {
    return resource::manager()->spritesheet_container().Load(tag, tag);
}

action::SpriteAnimationTable* Manager::GetSpriteAnimationTableFromFile(const std::string& file) {
    return resource::manager()->spriteanimation_loader().Load(file, file);
}

ugdk::LanguageWord*        Manager::GetLanguageWord(const std::string& tag) {
    ugdk::LanguageWord* word = resource::manager()->word_container().Find(tag);
    return word;
}

} // namespace resource
} // namespace ugdk
