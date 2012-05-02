#ifndef UGDK_ACTION_ENTITY_H_
#define UGDK_ACTION_ENTITY_H_

#include <ugdk/action.h>

namespace ugdk {

namespace action {

class Entity {
  public:
    Entity() {}
    virtual void Update(double dt) = 0;
    
    virtual void OnSceneAdd(Scene* scene) {}
};

}  // namespace action

}  // namespace ugdk

#endif // UGDK_ACTION_ENTITY_H_
