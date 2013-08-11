#include <ugdk/structure/intervalkdtree.h>

#include "collisionmanager.h"

#include "pyramidworks/collision/collisionclass.h"
#include "pyramidworks/collision/collisionobject.h"
#include "pyramidworks/collision/collisionlogic.h"

namespace pyramidworks {
namespace collision {

CollisionManager::CollisionManager(const ugdk::structure::ikdtree::Box<2>& tree_bounding_box) 
    :   tree_bounding_box_(tree_bounding_box) {}

CollisionManager::~CollisionManager() { 
    std::map<std::string, CollisionClass*>::iterator it;
    for(it = cache_.begin(); it != cache_.end(); ++it) {
        delete it->second;
    }
    cache_.clear();
}

void CollisionManager::Generate(const std::string &name) {
    cache_[name] = new CollisionClass(tree_bounding_box_);
#ifdef DEBUG
    cache_[name]->set_name(name);
#endif
}

void CollisionManager::Generate(const std::string &name, const std::string &parent) {
    CollisionClass* colclass = cache_[name] = new CollisionClass(tree_bounding_box_);
    colclass->set_parent(cache_[parent]);
#ifdef DEBUG
    colclass->set_name(name);
#endif
}

class HandleCollisionTask {
  public:
    HandleCollisionTask(const std::set<const CollisionObject*>& s) : objects_(s) {}

    bool operator()(double dt) {
        std::vector<CollisionInstance> collision_list;
    
        std::set<const CollisionObject*>::const_iterator i;
        for (i = objects_.begin(); i != objects_.end(); ++i)
            (*i)->SearchCollisions(collision_list);

        std::vector<CollisionInstance>::iterator it;
        for(it = collision_list.begin(); it != collision_list.end(); ++it) {
            it->first->Handle(it->second);
        }

        return true;
    }

  protected:
    const std::set<const CollisionObject*>& objects_;
};

ugdk::action::Task CollisionManager::GenerateHandleCollisionTask() {
    return HandleCollisionTask(active_objects_);
}

} // namespace collision
} // namespace pyramidworks
