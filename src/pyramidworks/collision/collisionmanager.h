#ifndef PYRAMIDWORKS_COLLISION_COLLISIONMANAGER_H_
#define PYRAMIDWORKS_COLLISION_COLLISIONMANAGER_H_

#include <string>
#include <map>
#include <set>
#include <ugdk/structures/intervalkdtree.h>
#include <pyramidworks/collision.h>
#include <ugdk/action.h>
#include <ugdk/util/uncopyable.h>

namespace pyramidworks {
namespace collision {

/// \class CollisionManager collisionmanager.h "pyramidworks/collision/collisionmanager.h"
/// A singleton that manages all collisions.
class CollisionManager : public ugdk::util::Uncopyable {
  public:
    CollisionManager(const ugdk::ikdtree::Box<2>& tree_bounding_box);
    ~CollisionManager();

    /// Creates a CollisionClass with no parent.
    void Generate(const std::string &name);
    void Generate(const char n[]) { const std::string str(n); Generate(str); }

    /// Creates a CollisionClass with a parent.
    void Generate(const std::string &name, const std::string &parent);
    void Generate(const char n[], const char p[]) { 
        const std::string name(n), parent(p); 
        Generate(name, parent); 
    }

    /// Returns a CollisionClass of the asked name.
    /** Searches for a CollisionClass with the given name, returning nullptr is none is found.
      * @param name The name to search for.
      * @return A pointer to a CollisionClass. */
    CollisionClass* Get(const std::string &name) { return cache_[name]; }
    CollisionClass* Get(const char n[]) { const std::string str(n); return Get(str); }

    void AddActiveObject(const CollisionObject* obj) { active_objects_.insert(obj); }
    void RemoveActiveObject(const CollisionObject* obj) { active_objects_.erase(obj); }

    /// Warning: this task depends on resources from this object. Do not use it after this object is destroyed.
    ugdk::action::Task GenerateHandleCollisionTask();
    
  private:
    const ugdk::ikdtree::Box<2> tree_bounding_box_;
    std::map<std::string, CollisionClass*> cache_;
    std::set<const CollisionObject*> active_objects_;
};

} // namespace collision
} // namespace pyramidworks

#endif /* PYRAMIDWORKS_COLLISION_COLLISIONMANAGER_H_ */
