#ifndef PYRAMIDWORKS_COLLISION_COLLISIONMANAGER_H_
#define PYRAMIDWORKS_COLLISION_COLLISIONMANAGER_H_

#include <string>
#include <map>
#include <set>
#include <memory>
#include <ugdk/system.h>
#include <pyramidworks/collision.h>
#include <ugdk/util/uncopyable.h>
#include <ugdk/structure/box.h>

namespace pyramidworks {
namespace collision {

/// \class CollisionManager collisionmanager.h "pyramidworks/collision/collisionmanager.h"
/// An instance of the collision system.
class CollisionManager : public ugdk::util::Uncopyable {
  public:
    CollisionManager(const ugdk::structure::Box<2>& tree_bounding_box);
    ~CollisionManager();

    /// Returns a CollisionClass of the asked name.
    /** Searches for a CollisionClass with the given name, creating it if none is found.
      * @param name The name to search for.
      * @return A reference to a CollisionClass. */
    CollisionClass& Find(const std::string &name);

    /// Changes the parent of a CollisionClass.
    /** @param name The name of the class whose parent will be changed.
      * @param parent The name of the parent class.
      */
    void ChangeClassParent(const std::string &name, const std::string &parent);

    void AddActiveObject(const CollisionObject* obj) { active_objects_.insert(obj); }
    void RemoveActiveObject(const CollisionObject* obj) { active_objects_.erase(obj); }
    const std::set<const CollisionObject*>& active_objects() const { return active_objects_; }

    /// Warning: this task depends on resources from this object. Do not use it after this object is destroyed.
    ugdk::system::Task GenerateHandleCollisionTask(double priority);
    
  private:
    const ugdk::structure::Box<2> tree_bounding_box_;
    std::map<std::string, std::unique_ptr<CollisionClass> > classes_;
    std::set<const CollisionObject*> active_objects_;
};

} // namespace collision
} // namespace pyramidworks

#endif /* PYRAMIDWORKS_COLLISION_COLLISIONMANAGER_H_ */
