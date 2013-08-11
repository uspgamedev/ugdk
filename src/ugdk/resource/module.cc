#include <ugdk/resource/module.h>

#include <ugdk/action/spritetypes.h>
#include <ugdk/graphic/spritesheet.h>
#include <ugdk/graphic/texture.h>
#include <ugdk/resource/genericcontainer.h>
#include <ugdk/structure/indexabletable.h>
#include <ugdk/util/languageword.h>

namespace ugdk {
namespace resource {

static Manager* reference_ = NULL;

template <class T>
static T* NullLoad(const std::string& filepath) { 
#ifdef DEBUG
    fprintf(stderr, "Warning: NullLoad<%s> called with path '%s'.", TOSTRING(T), filepath.c_str());
#endif
    return nullptr;
}

bool Initialize(Manager* manager) {
    if(manager) {
        manager->add_container(new GenericContainer<graphic::Texture*>(graphic::Texture::CreateFromFile));
        manager->add_container(new GenericContainer<graphic::Spritesheet*>(graphic::CreateSpritesheetFromTag));
        manager->add_container(new GenericContainer<action::SpriteAnimationTable*>(action::LoadSpriteAnimationTableFromFile));
        manager->add_container(new GenericContainer<LanguageWord*>(NullLoad<LanguageWord>));

        // The manager initialized correctly, so we can use it.
        reference_ = manager;
        return true;
    } else {
        // Error initializing the manager, delete it and don't activate the module.
        delete manager;
        // TODO: log the error.
        return false;
    }
}

void Release() {
    delete reference_;
    reference_ = nullptr;
}

Manager* manager() {
    return reference_;
}

graphic::Texture* GetTextureFromTag(const std::string& tag) {
    return manager()->get_container<graphic::Texture*>()->Find(tag);
}

graphic::Texture* GetTextureFromFile(const std::string& file) {
    return manager()->get_container<graphic::Texture*>()->Load(file, file);
}

graphic::Spritesheet* GetSpritesheetFromTag(const std::string& tag) {
    return manager()->get_container<graphic::Spritesheet*>()->Load(tag, tag);
}

action::SpriteAnimationTable* GetSpriteAnimationTableFromFile(const std::string& file) {
    return manager()->get_container<action::SpriteAnimationTable*>()->Load(file, file);
}

ugdk::LanguageWord* GetLanguageWord(const std::string& tag) {
    return manager()->get_container<LanguageWord*>()->Find(tag);
}

} // namespace resource
} // namespace ugdk
