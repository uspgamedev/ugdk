#ifndef UGDK_ACTION_ENTITY_H_
#define UGDK_ACTION_ENTITY_H_

namespace ugdk {

namespace action {

class Entity {
  public:
    Entity() {}
    virtual ~Entity() {}
    virtual void Update(double dt) = 0;
};

}  // namespace action

}  // namespace ugdk

#endif // UGDK_ACTION_ENTITY_H_
