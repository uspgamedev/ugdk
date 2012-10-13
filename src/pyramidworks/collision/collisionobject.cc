
#include <cstdlib>
#include <cstdio>

#include "collisionobject.h"

#include <ugdk/util/intervalkdtree.h>
#include "pyramidworks/collision/collisionmanager.h"
#include "pyramidworks/collision/collisionclass.h"
#include "pyramidworks/collision/collisionlogic.h"
#include "pyramidworks/geometry/geometricshape.h"

namespace pyramidworks {
namespace collision {


CollisionObject::CollisionObject(CollisionManager* manager, void *data) 
    :   manager_(manager),
        data_(data),
        collision_class_(NULL),
        shape_(NULL),
        is_active_(false) {}

CollisionObject::~CollisionObject() {
    std::map<const CollisionClass*, CollisionLogic*>::iterator it;
    for(it = known_collisions_.begin(); it != known_collisions_.end(); ++it)
        delete it->second;
    known_collisions_.clear();

    if(is_active_)
        StopColliding();

    if(shape_ != NULL)
        delete shape_;
}

void CollisionObject::SearchCollisions(std::vector<CollisionInstance> &collision_list) const {
    std::map<const CollisionClass*, CollisionLogic*>::const_iterator it;
    for(it = known_collisions_.begin(); it != known_collisions_.end(); ++it) {

        const CollisionObjectList& target_list = it->first->FindCollidingObjects(this);
        CollisionObjectList::const_iterator obj;
        for(obj = target_list.begin(); obj != target_list.end(); ++obj)
            collision_list.push_back(CollisionInstance(it->second, (*obj)->data_));
    }
}

bool CollisionObject::IsColliding(const CollisionObject* obj) const {
    if(this->shape_ == NULL || obj->shape_ == NULL) return false;
    return this->shape_->Intersects(absolute_position(), obj->shape_, obj->absolute_position());
}

void CollisionObject::AddCollisionLogic(const std::string& colclass, CollisionLogic* logic) {
    const CollisionClass* collision_class = manager_->Get(colclass);
    CollisionLogic *query = known_collisions_[collision_class];
    if( query != NULL ) delete query;
    known_collisions_[collision_class] = logic;
}

void CollisionObject::InitializeCollisionClass(const std::string& colclass) {
    CollisionClass* collision_class = manager_->Get(colclass);
#ifdef DEBUG
    if(collision_class_ != NULL) fprintf(stderr, "Pyramidworks - CollisionObject Fatal Error: Changing the collision_class of.\n");
    if(collision_class == NULL) fprintf(stderr, "Pyramidworks - CollisionObject Warning: Initializing the collision_class with NULL.\n");
#endif
    collision_class_ = collision_class;
}

void CollisionObject::StartColliding() {
    if(is_active_) return;
#ifdef DEBUG
    if(collision_class_ == NULL) fprintf(stderr, "Pyramidworks - CollisionObject Warning: StartColliding called with an object with NULL collision_class.\n");
    if(shape_ == NULL) {
        fprintf(stderr, "Pyramidworks - CollisionObject Error: StartColliding called with an object with NULL shape.\n");
        return;
    }
#endif
    collision_class_->AddObject(this);
    manager_->AddActiveObject(this);
    is_active_ = true;
}

void CollisionObject::StopColliding() {
    if(!is_active_) return;
#ifdef DEBUG
    if(collision_class_ == NULL) fprintf(stderr, "Pyramidworks - CollisionObject Warning: StopColliding called with an object with NULL collision_class.\n");
#endif
    collision_class_->RemoveObject(this);
    manager_->RemoveActiveObject(this);
    is_active_ = false;
}

void CollisionObject::set_shape(geometry::GeometricShape* shape) { 
    if(shape_) delete shape_;
    shape_ = shape;
    if(is_active_) this->collision_class_->RefreshObject(this);
}

void CollisionObject::MoveTo(const ugdk::math::Vector2D& position) {
    position_ = position;
    if(is_active_) this->collision_class_->RefreshObject(this);
}

ugdk::ikdtree::Box<2> CollisionObject::GetBoundingBox() const {
    return shape_->GetBoundingBox(this->absolute_position());
}

} // namespace collision
} // namespace pyramidworks
