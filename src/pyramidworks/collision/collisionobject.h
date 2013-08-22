#ifndef PYRAMIDWORKS_COLLISION_COLLISIONOBJECT_H_
#define PYRAMIDWORKS_COLLISION_COLLISIONOBJECT_H_

#include <map>
#include <list>
#include <string>
#include <memory>
#include <ugdk/action.h>
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
      * @param colclass Name of the collision class of this object.
      * @see CollisionLogic */
    CollisionObject(ugdk::action::Entity* owner, const std::string& colclass);
    ~CollisionObject();

    /// Search if there's any collision.
    /** For each CollisionClass this object knows how to collide with, it gathers a
      * list of CollisionObject it's colliding with and generates a list of the collisions.
      * @param collision_list A list in which the result is appended to.
      * @return A list of CollisionInstance, appended to the collision_list argument. */
    void SearchCollisions(std::list<CollisionInstance> &collision_list) const;

    /// Verifies if this CollisionObject is colliding with another CollisionObject.
    /** @param obj The CollisionObject to check against.
      * @return True if there's a collision with the given CollisionObject. */
    bool IsColliding(const CollisionObject* obj) const;

    /// Adds a new collision to the known collisions.
    /** @param colclass Name of the collision class.
      * @param logic The handler called when the collision occurs. */
    void AddCollisionLogic(const std::string& colclass, const CollisionLogic& logic);

    /// Changes the collision class associated with this object.
    /** @param colclass The string representating the collision class to set to. */
    void ChangeCollisionClass(const std::string&);

    /// Enables collisions with this object in the given CollisionManager.
    void StartColliding(CollisionManager*);

    /// Removes this object from it's CollisionClass object list.
    void StopColliding();

    /// Moves the object to the given position.
    void MoveTo(const ugdk::math::Vector2D& position);

    /// Wrapper to shape()->CreateBoundingBox(absolute_position())
    ugdk::structure::Box<2> CreateBoundingBox() const;

    /// Calculates the position of this object, with the offset added.
    ugdk::math::Vector2D absolute_position() const { return position_ + offset_; }
    
    /// Changes the shape used.
    /** @param shape The shape to use. 
      * @see GeometricShape */
    void ChangeShape(geometry::GeometricShape* shape);
    
    /// Returns the name of the collision class of this object.
    const std::string& collision_class() const { return collision_class_; }
    
    /// Returns the Entity that owns this object.
    ugdk::action::Entity* owner() const { return owner_; }

    /// Returns the position of this object.
    const ugdk::math::Vector2D& position() const { return position_; }

    /// Returns the offset of this object.
    const ugdk::math::Vector2D& offset() const { return offset_; }

    /// Returns the shape used.
    /** @return A const GeometricShape pointer. */
    const geometry::GeometricShape* shape() const { return shape_.get(); };
    
    /// Changes the offset of this object.
    void set_offset(const ugdk::math::Vector2D& _offset) { offset_ = _offset; }

  private:
    std::string collision_class_;

    ugdk::action::Entity* owner_;

    ugdk::math::Vector2D position_;
    ugdk::math::Vector2D offset_;

    std::unique_ptr<geometry::GeometricShape> shape_;

    CollisionManager* manager_;
    
    std::map<std::string, CollisionLogic> known_collisions_;
};

} // namespace collision
} // namespace pyramidworks

#endif /* PYRAMIDWORKS_COLLISION_COLLISIONOBJECT_H_ */
