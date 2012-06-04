#ifndef PYRAMIDWORKS_COLLISION_COLLISIONMASK_H_
#define PYRAMIDWORKS_COLLISION_COLLISIONMASK_H_

#include <string>
#include <list>
#include <set>
#include <ugdk/util.h>
#include <pyramidworks/collision.h>

namespace pyramidworks {
namespace collision {

/// \class CollisionClass collisionclass.h "pyramidworks/collision/collisionclass.h"
class CollisionClass {
  typedef ugdk::ikdtree::IntervalKDTree<const CollisionObject*, 2> ObjectTree;
  public:
    ~CollisionClass();
    const CollisionClass* parent() const { return parent_; }
    void set_parent(CollisionClass* parent) { parent_ = parent; }

    const CollisionObjectList FindCollidingObjects(const CollisionObject *target) const;

    void AddObject(const CollisionObject *obj);
    void RemoveObject(const CollisionObject *obj);
    void RefreshObject(const CollisionObject *obj);

#ifdef DEBUG
    void set_name(const std::string &name) { name_ = name; }
#endif
  private:
    friend class CollisionManager;
    CollisionClass(const ugdk::ikdtree::Box<2>& tree_bounding_box);

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
