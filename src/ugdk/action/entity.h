#ifndef UGDK_ACTION_ENTITY_H_
#define UGDK_ACTION_ENTITY_H_

#include <ugdk/action.h>

namespace ugdk {

namespace action {

class Entity {
  public:
    virtual ~Entity() {}

    virtual bool to_be_removed() const { return to_be_removed_; }
    
    virtual void Update(double dt) = 0;
    
    virtual void OnSceneAdd(Scene* scene) {}

  protected:
    Entity() : to_be_removed_(false) {}
    bool to_be_removed_;
};

}  // namespace action

}  // namespace ugdk

#endif // UGDK_ACTION_ENTITY_H_
