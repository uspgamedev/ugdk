#ifndef PYRAMIDWORKS_COLLISION_COLLISIONMANAGER_H_
#define PYRAMIDWORKS_COLLISION_COLLISIONMANAGER_H_

#include <string>
#include <map>
#include <ugdk/util/intervalkdtree.h>
#include <pyramidworks/collision.h>

namespace pyramidworks {
namespace collision {

/// \class CollisionManager collisionmanager.h "pyramidworks/collision/collisionmanager.h"
/// A singleton that manages all collisions.
class CollisionManager {
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
    /** Searches for a CollisionClass with the given name, returning NULL is none is found.
      * @param name The name to search for.
      * @return A pointer to a CollisionClass. */
    CollisionClass* Get(const std::string &name) { return cache_[name]; }
    CollisionClass* Get(const char n[]) { const std::string str(n); return Get(str); }
    
  private:
    const ugdk::ikdtree::Box<2> tree_bounding_box_;
    std::map<std::string, CollisionClass*> cache_;
};

} // namespace collision
} // namespace pyramidworks

#endif /* PYRAMIDWORKS_COLLISION_COLLISIONMANAGER_H_ */
