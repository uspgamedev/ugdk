#include <ugdk/resource/module.h>

#include <ugdk/structure/indexabletable.h>
#include <ugdk/system/compatibility.h>
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

bool Initialize(Manager* manager) {
    if(manager) {        
#ifndef UGDK_3D_ENABLED

        manager->CreateContainer<graphic::SpriteAnimationTable>(graphic::LoadSpriteAnimationTableFromFile);
        manager->CreateContainer<graphic::GLTexture>(graphic::GLTexture::CreateFromFile);
        manager->CreateContainer<graphic::TextureAtlas>(graphic::TextureAtlas::LoadFromFile);
        manager->CreateContainer<text::LanguageWord>(NullLoad<text::LanguageWord>);
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
    return manager()->GetContainer<graphic::SpriteAnimationTable>()->Load(file, file);
}

graphic::GLTexture* GetTextureFromTag(const std::string& tag) {
    return manager()->GetContainer<graphic::GLTexture>()->Find(tag);
}

graphic::GLTexture* GetTextureFromFile(const std::string& file) {
    return manager()->GetContainer<graphic::GLTexture>()->Load(file, file);
}

graphic::TextureAtlas* GetTextureAtlasFromTag(const std::string& tag) {
    return manager()->GetContainer<graphic::TextureAtlas>()->Find(tag);
}

graphic::TextureAtlas* GetTextureAtlasFromFile(const std::string& file) {
    return manager()->GetContainer<graphic::TextureAtlas>()->Load(file, file);
}

text::LanguageWord* GetLanguageWord(const std::string& tag) {
    return manager()->GetContainer<text::LanguageWord>()->Find(tag);
}

#endif // UGDK_3D_ENABLED

} // namespace resource
} // namespace ugdk
