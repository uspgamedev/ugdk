#ifndef UGDK_RESOURCE_MODULE_H_
#define UGDK_RESOURCE_MODULE_H_

#include <ugdk/resource/manager.h>

#include <ugdk/internal.h>
#include <ugdk/action/spritetypes.h> // For action::SpriteAnimationTable
#include <ugdk/graphic.h> // For graphic::Texture and graphic::TextureAtlas
#include <ugdk/util/languageword.h> // For ugdk::LanguageWord, and user convenience.

namespace ugdk {
namespace resource {

/// Initializes the module with the given Manager.
bool Initialize(Manager* manager);

/// Releases the module, disabling it.
void Release();

/// Getter for the manager of the module.
Manager* manager();

internal::GLTexture*             GetTextureFromTag(const std::string& tag);
internal::GLTexture*             GetTextureFromFile(const std::string& file);
graphic::TextureAtlas*           GetTextureAtlasFromTag(const std::string& tag);
graphic::TextureAtlas*           GetTextureAtlasFromFile(const std::string& file);
action::SpriteAnimationTable*    GetSpriteAnimationTableFromFile(const std::string& file);
ugdk::LanguageWord*              GetLanguageWord(const std::string& tag);

} // namespace resource
} // namespace ugdk

#endif // UGDK_RESOURCE_MODULE_H_

