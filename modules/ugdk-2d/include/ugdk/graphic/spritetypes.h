#ifndef UGDK_GRAPHIC_SPRITETYPES_H_
#define UGDK_GRAPHIC_SPRITETYPES_H_

#include <ugdk/graphic.h>

// The following includes are necessary to use SpriteAnimationPlayer and SpriteAnimationTable.
#include <ugdk/graphic/spriteanimationframe.h>
#include <ugdk/action/animationplayer.h>
#include <ugdk/structure/indexabletable.h>

#include <string>

namespace ugdk {
namespace graphic {

// These types are declared in <ugdk/graphic.h>.
// using SpriteAnimationPlayer = ::ugdk::action::AnimationPlayer<SpriteAnimationFrame>;
// using SpriteAnimationTable = ::ugdk::structure::IndexableTable<std::vector<SpriteAnimationFrame*>*>;

graphic::SpriteAnimationTable* LoadSpriteAnimationTableFromFile(const std::string& filepath);

}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_SPRITETYPES_H_
