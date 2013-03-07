#ifndef UGDK_ACTION_SPRITETYPES_H_
#define UGDK_ACTION_SPRITETYPES_H_

#include <string>
#include <ugdk/action.h>
#include <ugdk/util.h>

namespace ugdk {
namespace action {

typedef ::ugdk::action::AnimationPlayer<ugdk::action::SpriteAnimation> SpriteAnimationPlayer;
typedef ::ugdk::action::TableAnimationPlayer<ugdk::action::SpriteAnimation> SpriteTableAnimationPlayer;
typedef ::ugdk::util::IndexableTable<ugdk::action::SpriteAnimation*, 16> SpriteAnimationTable;

action::SpriteAnimationTable* LoadSpriteAnimationTableFromFile(const std::string& filepath);

}  // namespace action
}  // namespace ugdk

#endif
