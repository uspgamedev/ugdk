#include "collisionmanager.h"

#include "pyramidworks/collision/collisionclass.h"
#include "pyramidworks/collision/collisionobject.h"
#include "pyramidworks/collision/collisionlogic.h"

namespace pyramidworks {
namespace collision {

CollisionManager::CollisionManager(const ugdk::structure::Box<2>& tree_bounding_box) 
    :   tree_bounding_box_(tree_bounding_box) {}

CollisionManager::~CollisionManager() { 
    for(const auto& it : classes_)
        delete it.second;
}

void CollisionManager::Generate(const std::string &name) {
    classes_[name] = new CollisionClass(tree_bounding_box_);
#ifdef DEBUG
    classes_[name]->set_name(name);
#endif
}

void CollisionManager::Generate(const std::string &name, const std::string &parent) {
    CollisionClass* colclass = classes_[name] = new CollisionClass(tree_bounding_box_);
    colclass->set_parent(classes_[parent]);
#ifdef DEBUG
    colclass->set_name(name);
#endif
}

ugdk::action::Task CollisionManager::GenerateHandleCollisionTask() {
    return [this](double) -> bool {
        std::vector<CollisionInstance> collision_list;
        for (const CollisionObject* obj : this->active_objects_)
            obj->SearchCollisions(collision_list);
        for(const CollisionInstance& it : collision_list)
            it.first->Handle(it.second);
        return true;
    };
}

} // namespace collision
} // namespace pyramidworks
