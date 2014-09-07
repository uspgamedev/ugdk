#ifndef PYRAMIDWORKS_COLLISION_H_
#define PYRAMIDWORKS_COLLISION_H_

#include <functional>
#include <list>
#include <utility>

namespace pyramidworks {
namespace collision {

class CollisionClass;
class CollisionData;
class CollisionObject;
class CollisionManager;

typedef std::list<const CollisionObject*> CollisionObjectList;
typedef std::function<void (const CollisionObject*)> CollisionLogic;
typedef std::pair<const CollisionLogic&, const CollisionObject*> CollisionInstance;

} // namespace collision
} // namespace pyramidworks

#endif /* PYRAMIDWORKS_COLLISION_H_ */
