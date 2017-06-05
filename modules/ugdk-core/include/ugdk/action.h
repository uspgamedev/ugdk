#ifndef UGDK_ACTION_H_
#define UGDK_ACTION_H_

namespace ugdk {
namespace action {

class MediaPlayer;
class MediaManager;

template<class T>
class Animation;

template<class T>
class AnimationPlayer;

class Scene;

class SpriteAnimationFrame;
using SpriteAnimation = ::ugdk::action::Animation<SpriteAnimationFrame>;
using SpriteAnimationPlayer = ::ugdk::action::AnimationPlayer<SpriteAnimationFrame>;

}  // namespace action
}  // namespace ugdk

#endif
