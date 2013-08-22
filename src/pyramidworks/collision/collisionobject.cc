#include "collisionobject.h"

#include <cstdlib>
#include <cstdio>

#include <ugdk/structure/intervalkdtree.h>
#include "pyramidworks/collision/collisionmanager.h"
#include "pyramidworks/collision/collisionclass.h"
#include "pyramidworks/geometry/geometricshape.h"

namespace pyramidworks {
namespace collision {


CollisionObject::CollisionObject(ugdk::action::Entity* data, const std::string& colclass) 
    : collision_class_(colclass)
    , data_(data)
    , shape_(nullptr)
    , manager_(nullptr) {}

CollisionObject::~CollisionObject() {
    if(manager_)
        StopColliding();
}

void CollisionObject::SearchCollisions(std::list<CollisionInstance> &collision_list) const {
    for(const auto& it : known_collisions_) {
        CollisionObjectList target_list;
        manager_->Find(it.first).FindCollidingObjects(this, target_list);

        for(const CollisionObject *obj : target_list)
            collision_list.emplace_back(it.second, obj->data_);
    }
}

bool CollisionObject::IsColliding(const CollisionObject* obj) const {
    if(!this->shape_ || !obj->shape_) return false;
    return this->shape()->Intersects(absolute_position(), obj->shape(), obj->absolute_position());
}

void CollisionObject::AddCollisionLogic(const std::string& colclass, const CollisionLogic& logic) {
    known_collisions_[colclass] = logic;
}

void CollisionObject::ChangeCollisionClass(const std::string& colclass) {
    if(manager_)
        manager_->Find(collision_class_).RemoveObject(this);
    collision_class_ = colclass;
    if(manager_)
        manager_->Find(collision_class_).AddObject(this);
}

void CollisionObject::StartColliding(CollisionManager* manager) {
    if(manager_) {
        //throw 
        return;
    }
    manager_ = manager;
    manager_->Find(collision_class_).AddObject(this);
    manager_->AddActiveObject(this);
}

void CollisionObject::StopColliding() {
    if(!manager_) return;
    manager_->Find(collision_class_).RemoveObject(this);
    manager_->RemoveActiveObject(this);
    manager_ = nullptr;
}

void CollisionObject::set_shape(geometry::GeometricShape* shape) { 
    shape_.reset(shape);
    if(manager_)
        manager_->Find(collision_class_).RefreshObject(this);
}

void CollisionObject::MoveTo(const ugdk::math::Vector2D& position) {
    position_ = position;
    if(manager_)
        manager_->Find(collision_class_).RefreshObject(this);
}

ugdk::structure::Box<2> CollisionObject::CreateBoundingBox() const {
    return shape_->GetBoundingBox(this->absolute_position());
}

} // namespace collision
} // namespace pyramidworks
