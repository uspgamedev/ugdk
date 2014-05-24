#ifndef UGDK_ACTION_SPRITETYPES_H_
#define UGDK_ACTION_SPRITETYPES_H_

#include <ugdk/action/spriteanimationframe.h>
#include <ugdk/structure/indexabletable.h>

#include <ugdk/action.h>
#include <ugdk/structure.h>

#include <string>

namespace ugdk {
namespace action {

typedef ::ugdk::action::AnimationPlayer<ugdk::action::SpriteAnimation> SpriteAnimationPlayer;
typedef ::ugdk::structure::IndexableTable<ugdk::action::SpriteAnimation*> SpriteAnimationTable;

action::SpriteAnimationTable* LoadSpriteAnimationTableFromFile(const std::string& filepath);

}  // namespace action
}  // namespace ugdk

#endif
