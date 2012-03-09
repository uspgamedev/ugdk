#include <ugdk/util/intervalkdtree.h>

#include "collisionmanager.h"

#include "pyramidworks/collision/collisionclass.h"
#include "pyramidworks/collision/collisionobject.h"

namespace pyramidworks {
namespace collision {

CollisionManager::CollisionManager(const ugdk::ikdtree::Box<2>& tree_bounding_box) 
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

} // namespace collision
} // namespace pyramidworks
