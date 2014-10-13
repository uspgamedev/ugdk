#ifndef PYRAMIDWORKS_COLLISION_COLLISIONDATA_H_
#define PYRAMIDWORKS_COLLISION_COLLISIONDATA_H_

namespace pyramidworks {
namespace collision {

/// An interface for other classes implement so they can be associated with an CollsionObject.
class CollisionData {
  public:
    virtual ~CollisionData() {}
};

} // namespace collision
} // namespace pyramidworks

#endif /* PYRAMIDWORKS_COLLISION_COLLISIONDATA_H_ */
