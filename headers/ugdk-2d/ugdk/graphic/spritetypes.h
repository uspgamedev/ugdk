#ifndef UGDK_GRAPHIC_SPRITETYPES_H_
#define UGDK_GRAPHIC_SPRITETYPES_H_

#include <ugdk/graphic.h>
#include <ugdk/graphic/spriteanimationframe.h>
#include <ugdk/action/animationplayer.h>
#include <ugdk/structure/indexabletable.h>

#include <string>

namespace ugdk {
namespace graphic {

graphic::SpriteAnimationTable* LoadSpriteAnimationTableFromFile(const std::string& filepath);

}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_SPRITETYPES_H_
