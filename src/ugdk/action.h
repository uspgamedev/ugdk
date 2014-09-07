#ifndef UGDK_ACTION_H_
#define UGDK_ACTION_H_

#include <ugdk/structure.h>
#include <vector>

namespace ugdk {
namespace action {

class MediaPlayer;
class MediaManager;

template<class T>
class AnimationPlayer;

template<class T>
class TableAnimationPlayer;

class SpriteAnimationFrame;
typedef ::ugdk::action::AnimationPlayer<ugdk::action::SpriteAnimationFrame> SpriteAnimationPlayer;
typedef ::ugdk::structure::IndexableTable<std::vector<SpriteAnimationFrame*>*> SpriteAnimationTable;

class Scene;
class Observer;

}  // namespace action
}  // namespace ugdk

#endif
