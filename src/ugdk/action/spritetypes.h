#ifndef UGDK_ACTION_SPRITETYPES_H_
#define UGDK_ACTION_SPRITETYPES_H_

#include <ugdk/action/spriteanimationframe.h>
#include <ugdk/action/animationplayer.h>
#include <ugdk/structure/indexabletable.h>

#include <string>
#include <vector>

namespace ugdk {
namespace action {

typedef ::ugdk::action::AnimationPlayer<ugdk::action::SpriteAnimationFrame> SpriteAnimationPlayer;
typedef ::ugdk::structure::IndexableTable<std::vector<SpriteAnimationFrame*>*> SpriteAnimationTable;

action::SpriteAnimationTable* LoadSpriteAnimationTableFromFile(const std::string& filepath);

}  // namespace action
}  // namespace ugdk

#endif
