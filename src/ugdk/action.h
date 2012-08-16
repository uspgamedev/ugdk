#ifndef UGDK_ACTION_H_
#define UGDK_ACTION_H_

#include <ugdk/util.h>

namespace ugdk {
namespace action {

class Animation;
class AnimationManager;
typedef ::ugdk::util::IndexableTable<ugdk::action::Animation*> AnimationSet;
class Scene;
class Entity;
class Observer;
class Task;

}  // namespace action
}  // namespace ugdk

#endif
