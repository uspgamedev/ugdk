#include "collisionmanager.h"

#include "pyramidworks/collision/collisionclass.h"
#include "pyramidworks/collision/collisionobject.h"

namespace pyramidworks {
namespace collision {

CollisionManager::CollisionManager(const ugdk::structure::Box<2>& tree_bounding_box) 
    :   tree_bounding_box_(tree_bounding_box) {}

CollisionManager::~CollisionManager() {}

CollisionClass& CollisionManager::Find(const std::string &name) {
    auto find = classes_.find(name);
    if(find == classes_.end()) {
        classes_[name] = std::unique_ptr<CollisionClass>(new CollisionClass(tree_bounding_box_));
#ifdef DEBUG
        classes_[name]->set_name(name);
#endif
        return *classes_[name];
    } else
        return *find->second;
}

void CollisionManager::ChangeParent(const std::string &name, const std::string &parent) {
    CollisionClass& colclass = Find(name);
    colclass.ChangeParent(&Find(parent));
}

ugdk::action::Task CollisionManager::GenerateHandleCollisionTask() {
    return [this](double) -> bool {
        std::list<CollisionInstance> collision_list;
        for (const CollisionObject* obj : this->active_objects_)
            obj->SearchCollisions(collision_list);
        for(const CollisionInstance& it : collision_list)
            it.first(it.second);
        return true;
    };
}

} // namespace collision
} // namespace pyramidworks
