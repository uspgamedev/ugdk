#include <ugdk/resource/module.h>

#include <ugdk/resource/genericcontainer.h>
#include <ugdk/structure/indexabletable.h>
#include <ugdk/debug/log.h>

#ifndef UGDK_3D_ENABLED
#include <ugdk/graphic/spritetypes.h>
#include <ugdk/graphic/textureatlas.h>
#include <ugdk/text/languageword.h>
#include "graphic/gltexture.h"
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
#ifndef UGDK_3D_ENABLED
        manager->add_container(new GenericContainer<graphic::SpriteAnimationTable*>(graphic::LoadSpriteAnimationTableFromFile));
        manager->add_container(new GenericContainer<graphic::GLTexture*>(graphic::GLTexture::CreateFromFile));
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

#ifndef UGDK_3D_ENABLED

graphic::SpriteAnimationTable* GetSpriteAnimationTableFromFile(const std::string& file) {
    return manager()->get_container<graphic::SpriteAnimationTable*>()->Load(file, file);
}

graphic::GLTexture* GetTextureFromTag(const std::string& tag) {
    return manager()->get_container<graphic::GLTexture*>()->Find(tag);
}

graphic::GLTexture* GetTextureFromFile(const std::string& file) {
    return manager()->get_container<graphic::GLTexture*>()->Load(file, file);
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
