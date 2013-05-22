#include "collisionclass.h"

#include <vector>
#include <ugdk/util/intervalkdtree.h>
#include <ugdk/math/vector2D.h>
#include "pyramidworks/collision/collisionobject.h"
#include "pyramidworks/geometry/geometricshape.h"

namespace pyramidworks {
namespace collision {

CollisionClass::~CollisionClass() {delete objects_tree_;}

CollisionClass::CollisionClass(const ugdk::ikdtree::Box<2>& tree_bounding_box) 
  : parent_(NULL),
    objects_tree_(new ObjectTree(tree_bounding_box,5)) {}

void CollisionClass::FindCollidingObjects(const CollisionObject *target,
                                                               CollisionObjectList& result) const {
    
    std::vector<const CollisionObject *> *filtered_results 
        = objects_tree_->getIntersectingItems(target->GetBoundingBox());

    for(std::vector<const CollisionObject *>::const_iterator it
           = filtered_results->begin(); it != filtered_results->end(); ++it) {
        if((*it) == target) continue;
        if( (*it)->IsColliding(target) )
            result.push_front(*it);
    }

    delete filtered_results;
}

void CollisionClass::FindCollidingObjects(const ugdk::math::Vector2D& position, const geometry::GeometricShape& shape, CollisionObjectList& collisions) const {
    std::vector<const CollisionObject *> *filtered_results 
        = objects_tree_->getIntersectingItems(shape.GetBoundingBox(position));
    for(std::vector<const CollisionObject *>::const_iterator it
           = filtered_results->begin(); it != filtered_results->end(); ++it) {
        if( shape.Intersects(position, (*it)->shape(), (*it)->absolute_position() ) )
            collisions.push_front(*it);
    }
    delete filtered_results;
}
    
void CollisionClass::AddObject(const CollisionObject *obj) {
	if(parent_) parent_->AddObject(obj);
    objects_tree_->Insert(obj->GetBoundingBox(), obj);
}

void CollisionClass::RemoveObject(const CollisionObject *obj) { 
	if(parent_) parent_->RemoveObject(obj);
    objects_tree_->Remove(obj);
}

void CollisionClass::RefreshObject(const CollisionObject *obj) {
	if(parent_) parent_->RefreshObject(obj);
    objects_tree_->Update(obj->GetBoundingBox(), obj);
}

} // namespace collision
} // namespace pyramidworks
