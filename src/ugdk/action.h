#ifndef UGDK_ACTION_H_
#define UGDK_ACTION_H_

#include <ugdk/system.h>

namespace ugdk {
namespace action {

class MediaPlayer;
class MediaManager;

template<class T>
class AnimationPlayer;

template<class T>
class TableAnimationPlayer;

class SpriteAnimationFrame;
class SpriteAnimation;

class Scene;
class Entity;
class Observer;

using system::Task;

}  // namespace action
}  // namespace ugdk

#endif
