#ifndef UGDK_ACTION_H_
#define UGDK_ACTION_H_

#include <functional>

namespace ugdk {
namespace action {

class MediaPlayer;

template<class T>
class AnimationPlayer;

template<class T>
class TableAnimationPlayer;

class SpriteAnimationFrame;
class SpriteAnimation;

class Scene;
class Entity;
class Observer;

typedef std::function<bool (double)> Task;

}  // namespace action
}  // namespace ugdk

#endif
