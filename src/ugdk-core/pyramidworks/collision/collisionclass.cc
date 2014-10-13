#include <pyramidworks/collision/collisionclass.h>

#include <list>
#include <ugdk/structure/intervalkdtree.h>
#include <ugdk/math/vector2D.h>
#include "pyramidworks/collision/collisionobject.h"
#include "pyramidworks/geometry/geometricshape.h"

namespace pyramidworks {
namespace collision {

CollisionClass::~CollisionClass() {}

CollisionClass::CollisionClass(const ugdk::structure::Box<2>& tree_bounding_box) 
  : parent_(nullptr),
    objects_tree_(new ObjectTree(tree_bounding_box,5)) {}

void CollisionClass::FindCollidingObjects(const CollisionObject *target,
                                                               CollisionObjectList& result) const {
    
    std::list<const CollisionObject *> filtered_results;
    objects_tree_->FindIntersectingItems(target->CreateBoundingBox(), std::back_inserter(filtered_results));

    for(const CollisionObject* it : filtered_results) {
        if(it == target) continue;
        if(it->IsColliding(target))
            result.push_front(it);
    }
}

void CollisionClass::FindCollidingObjects(const ugdk::math::Vector2D& position, 
                                          const geometry::GeometricShape& shape, 
                                          CollisionObjectList& collisions) const {
    std::list<const CollisionObject *> filtered_results;
    objects_tree_->FindIntersectingItems(shape.GetBoundingBox(position), std::back_inserter(filtered_results));
    for(const CollisionObject* it : filtered_results) {
        if(shape.Intersects(position, it->shape(), it->absolute_position()))
            collisions.push_front(it);
    }
}
    
void CollisionClass::AddObject(const CollisionObject *obj) {
	if(parent_) parent_->AddObject(obj);
    objects_tree_->Insert(obj->CreateBoundingBox(), obj);
}

void CollisionClass::RemoveObject(const CollisionObject *obj) { 
	if(parent_) parent_->RemoveObject(obj);
    objects_tree_->Remove(obj);
}

void CollisionClass::RefreshObject(const CollisionObject *obj) {
	if(parent_) parent_->RefreshObject(obj);
    objects_tree_->Update(obj->CreateBoundingBox(), obj);
}
    
void CollisionClass::ChangeParent(CollisionClass* parent) {
    if(parent_)
        for(const auto& it : *objects_tree_)
            parent_->RemoveObject(it.first);
    parent_ = parent;
    if(parent_)
        for(const auto& it : *objects_tree_)
            parent_->AddObject(it.first);
}

} // namespace collision
} // namespace pyramidworks
