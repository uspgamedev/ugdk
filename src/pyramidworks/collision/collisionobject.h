#ifndef PYRAMIDWORKS_COLLISION_COLLISIONOBJECT_H_
#define PYRAMIDWORKS_COLLISION_COLLISIONOBJECT_H_

#include <map>
#include <vector>
#include <string>
#include <ugdk/math/vector2D.h>
#include <ugdk/structure.h>
#include <pyramidworks/collision.h>
#include <pyramidworks/geometry.h>

namespace pyramidworks {
namespace collision {

/// \class CollisionObject collisionobject.h "pyramidworks/collision/collisionobject.h"
/// A class that knows how to manage collisions.
/** It encapsulates everything that is related to collisions, from finding the
  * collisions to calling the corresponding methods. */
class CollisionObject {
  public:
    /** @param data The data sent to the CollisionLogic when a collision happens.
      * @see CollisionLogic */
    CollisionObject(CollisionManager* manager, void *data);
    ~CollisionObject();

    /// Search if there's any collision.
    /** For each CollisionClass this object knows how to collide with, it gathers a
      * list of CollisionObject it's colliding with and generates a list of the collisions.
      * @param collision_list A list in which the result is appended to.
      * @return A list of CollisionInstance, appended to the collision_list argument. */
    void SearchCollisions(std::vector<CollisionInstance> &collision_list) const;

    /// Verifies if this CollisionObject is colliding with another CollisionObject.
    /** @param obj The CollisionObject to check against.
      * @return True if there's a collision with the given CollisionObject. */
    bool IsColliding(const CollisionObject* obj) const;

    /// Adds a new collision to the known collisions.
    /** @param colclass Name of the collision class.
      * @param logic Is not changed.
      * @see AddCollisionLogic */
    void AddCollisionLogic(const std::string& colclass, CollisionLogic* logic);
    void AddCollisionLogic(const char n[], CollisionLogic* logic) { 
        const std::string str(n); AddCollisionLogic(str, logic); 
    }

    /// Defines the CollisionClass associated with this object.
    /** Warning: will quit the program with a Fatal Error if used to the
        collision_class is already set.
        @param colclass The CollisionClass to set to.
        @see CollisionClass */
    void InitializeCollisionClass(const std::string&);
    void InitializeCollisionClass(const char n[]) { const std::string str(n); InitializeCollisionClass(str); }

    /// Adds this object to it's CollisionClass object list.
    void StartColliding();

    /// Removes this object from it's CollisionClass object list.
    void StopColliding();

    // TODO document
    CollisionClass* collision_class() const { return collision_class_; }

    /// Returns the shape used.
    /** @return A const GeometricShape pointer. */
    const geometry::GeometricShape* shape() const { return shape_; };

    /// Changes the shape used.
    /** @param shape The shape to use. 
      * @see GeometricShape */
    void set_shape(geometry::GeometricShape* shape);

    /// TODO document
    ugdk::math::Vector2D absolute_position() const { return position_ + offset_; }

    /// TODO document
    void MoveTo(const ugdk::math::Vector2D& position);

    /// Wrapper to shape()->GetBoundingBox(absolute_position())
    ugdk::structure::Box<2> GetBoundingBox() const;
    
    /// Getter for the stored data.
    void* data() const { return data_; }

  private:
    CollisionClass* collision_class_;

    // Data that is sent to CollisionLogic::Handle
    void *data_;

    bool is_active_;
    ugdk::math::Vector2D position_;
    ugdk::math::Vector2D offset_;

    geometry::GeometricShape* shape_;

    CollisionManager* manager_;

    std::map<const CollisionClass*, CollisionLogic*> known_collisions_;
};

} // namespace collision
} // namespace pyramidworks

#endif /* PYRAMIDWORKS_COLLISION_COLLISIONOBJECT_H_ */
