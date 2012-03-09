#ifndef PYRAMIDWORKS_COLLISION_H_
#define PYRAMIDWORKS_COLLISION_H_

#include <list>
#include <utility>

namespace pyramidworks {
namespace collision {

class CollisionClass;
class CollisionObject;
class CollisionLogic;
class CollisionManager;

typedef std::list<const CollisionObject *> CollisionObjectList;
typedef std::pair<CollisionLogic*, void* > CollisionInstance;

} // namespace collision
} // namespace pyramidworks

#endif /* PYRAMIDWORKS_COLLISION_H_ */
