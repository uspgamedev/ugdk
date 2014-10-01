#include <ugdk/resource/module.h>

#include <ugdk/resource/genericcontainer.h>
#include <ugdk/structure/indexabletable.h>
#include <ugdk/debug/log.h>

#include <ugdk/action/spritetypes.h>

#ifndef UGDK_3D_ENABLED
#include <ugdk/graphic/textureatlas.h>
#include <ugdk/internal/gltexture.h>
#include <ugdk/text/languageword.h>
#endif

namespace ugdk {
namespace resource {

static Manager* reference_ = NULL;

template <class T>
static T* NullLoad(const std::string& filepath) {
    debug::DebugLog(debug::LogLevel::WARNING, "UGDK",
                    "NullLoad<", TOSTRING(T), "> called with path '", filepath, "'");
    return nullptr;
}

bool Initialize(Manager* manager) {
    if(manager) {
        manager->add_container(new GenericContainer<action::SpriteAnimationTable*>(action::LoadSpriteAnimationTableFromFile));
#ifndef UGDK_3D_ENABLED
        manager->add_container(new GenericContainer<internal::GLTexture*>(internal::GLTexture::CreateFromFile));
        manager->add_container(new GenericContainer<graphic::TextureAtlas*>(graphic::TextureAtlas::LoadFromFile));
        manager->add_container(new GenericContainer<text::LanguageWord*>(NullLoad<text::LanguageWord>));
#endif

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

action::SpriteAnimationTable* GetSpriteAnimationTableFromFile(const std::string& file) {
    return manager()->get_container<action::SpriteAnimationTable*>()->Load(file, file);
}

#ifndef UGDK_3D_ENABLED

internal::GLTexture* GetTextureFromTag(const std::string& tag) {
    return manager()->get_container<internal::GLTexture*>()->Find(tag);
}

internal::GLTexture* GetTextureFromFile(const std::string& file) {
    return manager()->get_container<internal::GLTexture*>()->Load(file, file);
}

graphic::TextureAtlas* GetTextureAtlasFromTag(const std::string& tag) {
    return manager()->get_container<graphic::TextureAtlas*>()->Find(tag);
}

graphic::TextureAtlas* GetTextureAtlasFromFile(const std::string& file) {
    return manager()->get_container<graphic::TextureAtlas*>()->Load(file, file);
}

text::LanguageWord* GetLanguageWord(const std::string& tag) {
    return manager()->get_container<text::LanguageWord*>()->Find(tag);
}

#endif // UGDK_3D_ENABLED

} // namespace resource
} // namespace ugdk
