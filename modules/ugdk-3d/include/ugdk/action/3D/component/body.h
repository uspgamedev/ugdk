#ifndef UGDK_ACTION_3D_COMPONENT_BODY_H
#define UGDK_ACTION_3D_COMPONENT_BODY_H

#include <ugdk/action/3D/component.h>
#include <OgreVector3.h>
#include <LinearMath/btVector3.h>
#include <functional>
#include <vector>

class btVector3;
namespace Ogre {
class Vector3;
class Quaternion;
}

class btManifoldPoint;

namespace ugdk {
namespace action {
namespace mode3d {
namespace component {

struct ContactPoint {
    btVector3 world_positionA;
    btVector3 world_positionB;
    double applied_impulse;
    double distance;

    ContactPoint(const btVector3& posA, const btVector3& posB, double impulse, double dist) 
        : world_positionA(posA), world_positionB(posB), applied_impulse(impulse), distance(dist) {}
};
using ElementPtr = std::shared_ptr < Element > ;
using ContactPointVector = std::vector < ContactPoint > ;
using CollisionAction = std::function < void(const ElementPtr&, const ElementPtr&, const ContactPointVector&) >;

class Body : public Component {

  public:

    std::type_index type() const override;

    virtual double mass() const = 0;
    virtual short collision_group() const = 0;
    virtual short collides_with() const = 0;
    virtual void set_angular_factor(double x_factor, double y_factor, double z_factor) = 0;
    virtual void set_restitution(double factor) = 0;
    virtual void set_friction(double frict) = 0;
    virtual void set_damping(double linear, double angular) = 0;
    virtual void SetRespondsOnContact(bool has_response) = 0;
    virtual void SetContinuousCollisionDetection(double speed_threshold, double sphere_radius) = 0;

    virtual Ogre::Vector3 position() const = 0;
    virtual Ogre::Quaternion orientation() const = 0;
    virtual void set_orientation(const Ogre::Vector3& dir) = 0;
    virtual Ogre::Vector3 linear_velocity() const = 0;
    virtual void set_linear_velocity(const Ogre::Vector3& velocity) = 0;
    virtual Ogre::Vector3 angular_velocity() const = 0;
    virtual Ogre::Vector3 GetVelocityInPoint(const Ogre::Vector3& point) const = 0;

    void Translate(const Ogre::Vector3& move);
    void Move(const Ogre::Vector3& delta);

    virtual void Translate(double move_x, double move_y, double move_z) = 0;
    virtual void Move(double delta_x, double delta_y, double delta_z) = 0;
    virtual void Rotate(double yaw, double pitch, double roll) = 0;
    virtual void Scale(double factor_x, double factor_y, double factor_z) = 0;

    virtual void AddCollisionAction(short target_mask, const CollisionAction& action) = 0;

  protected:

    Body () {}
    void OnTaken() override;

};

inline std::type_index Body::type() const {
    return typeid(Body);
}

inline void Body::Translate(const Ogre::Vector3 &move) {
    this->Translate(move.x, move.y, move.z);
}

inline void Body::Move(const Ogre::Vector3 &delta) {
    this->Move(delta.x, delta.y, delta.z);
}

inline void Body::OnTaken() {}

template <>
class NullComponent<Body> : public Body {
  public:
    const static bool is_valid = true;
    virtual void Translate(double move_x, double move_y, double move_z) override {}
    virtual void Move(double delta_x, double delta_y, double delta_z) override {}
    virtual void Rotate(double yaw, double pitch, double roll) override {}
    virtual void Scale(double factor_x, double factor_y, double factor_z) override {}
};

} // namespace component
} // namespace mode3d
} // namespace action
} // namespace ugdk

#endif // UGDK_ACTION_3D_COMPONENT_BODY_H
