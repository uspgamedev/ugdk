#include "collisionobject.h"

#include <cstdlib>
#include <cstdio>

#include <ugdk/structure/intervalkdtree.h>
#include "pyramidworks/collision/collisionmanager.h"
#include "pyramidworks/collision/collisionclass.h"
#include "pyramidworks/collision/collisionlogic.h"
#include "pyramidworks/geometry/geometricshape.h"

namespace pyramidworks {
namespace collision {


CollisionObject::CollisionObject(CollisionManager* manager, void *data) 
    :   collision_class_(nullptr)
    ,   data_(data)
    ,   is_active_(false)
    ,   shape_(nullptr)
    ,   manager_(manager) {}

CollisionObject::~CollisionObject() {
    for(auto& it : known_collisions_)
        delete it.second;

    if(is_active_)
        StopColliding();

    delete shape_;
}

void CollisionObject::SearchCollisions(std::vector<CollisionInstance> &collision_list) const {
    std::map<const CollisionClass*, CollisionLogic*>::const_iterator it;
    for(it = known_collisions_.begin(); it != known_collisions_.end(); ++it) {

        CollisionObjectList target_list;
        it->first->FindCollidingObjects(this, target_list);
        
        CollisionObjectList::const_iterator obj;
        for(obj = target_list.begin(); obj != target_list.end(); ++obj)
            collision_list.push_back(CollisionInstance(it->second, (*obj)->data_));
    }
}

bool CollisionObject::IsColliding(const CollisionObject* obj) const {
    if(this->shape_ == nullptr || obj->shape_ == nullptr) return false;
    return this->shape_->Intersects(absolute_position(), obj->shape_, obj->absolute_position());
}

void CollisionObject::AddCollisionLogic(const std::string& colclass, CollisionLogic* logic) {
    const CollisionClass* collision_class = manager_->Get(colclass);
    delete known_collisions_[collision_class];
    known_collisions_[collision_class] = logic;
}

void CollisionObject::InitializeCollisionClass(const std::string& colclass) {
    CollisionClass* new_collision_class = manager_->Get(colclass);
    assert(!collision_class_);
    assert(new_collision_class);
    collision_class_ = new_collision_class;
}

void CollisionObject::StartColliding() {
    if(is_active_) return;
    assert(shape_);
    collision_class_->AddObject(this);
    manager_->AddActiveObject(this);
    is_active_ = true;
}

void CollisionObject::StopColliding() {
    if(!is_active_) return;
    collision_class_->RemoveObject(this);
    manager_->RemoveActiveObject(this);
    is_active_ = false;
}

void CollisionObject::set_shape(geometry::GeometricShape* shape) { 
    delete shape_;
    shape_ = shape;
    if(is_active_)
        this->collision_class_->RefreshObject(this);
}

void CollisionObject::MoveTo(const ugdk::math::Vector2D& position) {
    position_ = position;
    if(is_active_)
        this->collision_class_->RefreshObject(this);
}

ugdk::structure::Box<2> CollisionObject::GetBoundingBox() const {
    return shape_->GetBoundingBox(this->absolute_position());
}

} // namespace collision
} // namespace pyramidworks
