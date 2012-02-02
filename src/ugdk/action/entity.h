#ifndef UGDK_ACTION_ENTITY_H_
#define UGDK_ACTION_ENTITY_H_

namespace ugdk {

class Entity {
  public:
    Entity() {}
    virtual void Update(float dt) = 0;
};

}  // namespace ugdk

#endif // UGDK_ACTION_ENTITY_H_
