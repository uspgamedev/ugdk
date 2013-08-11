#ifndef PYRAMIDWORKS_COLLISION_COLLISIONMASK_H_
#define PYRAMIDWORKS_COLLISION_COLLISIONMASK_H_

#include <string>
#include <list>
#include <set>
#include <ugdk/structure.h>
#include <ugdk/math.h>
#include <pyramidworks/collision.h>
#include <pyramidworks/geometry.h>

namespace pyramidworks {
namespace collision {

/// \class CollisionClass collisionclass.h "pyramidworks/collision/collisionclass.h"
class CollisionClass {
  typedef ugdk::structure::ikdtree::IntervalKDTree<const CollisionObject*, 2> ObjectTree;
  public:
    ~CollisionClass();
    const CollisionClass* parent() const { return parent_; }
    void set_parent(CollisionClass* _parent) { parent_ = _parent; }

    void FindCollidingObjects(const CollisionObject *target, CollisionObjectList& collisions) const;
    void FindCollidingObjects(const ugdk::math::Vector2D& position, const geometry::GeometricShape& shape, CollisionObjectList& collisions) const;

    void AddObject(const CollisionObject *obj);
    void RemoveObject(const CollisionObject *obj);
    void RefreshObject(const CollisionObject *obj);

#ifdef DEBUG
    void set_name(const std::string &name) { name_ = name; }
#endif
  private:
    friend class CollisionManager;
    CollisionClass(const ugdk::structure::Box<2>& tree_bounding_box);
    
    CollisionClass(const CollisionClass&); // Uncopyable
    void operator=(const CollisionClass&); // Uncopyable 

#ifdef DEBUG
    // Unnecessary, used for debugging purposes.
    std::string name_;
#endif
    CollisionClass* parent_;
    ObjectTree* objects_tree_;
};

} // namespace collision
} // namespace pyramidworks

#endif /* PYRAMIDWORKS_COLLISION_COLLISIONMASK_H_ */
