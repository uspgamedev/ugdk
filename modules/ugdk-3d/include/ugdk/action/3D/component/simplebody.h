#ifndef UGDK_ACTION_3D_COMPONENT_SIMPLEBODY_H
#define UGDK_ACTION_3D_COMPONENT_SIMPLEBODY_H

#include <ugdk/action/3D/component/body.h>

namespace ugdk {
namespace action {
namespace mode3d {

namespace component {

class SimpleBody final : public Body {
public:
    using Body::ApplyImpulse;

    SimpleBody(const Ogre::Vector3& initial_pos, const Ogre::Quaternion& initial_dir);
    ~SimpleBody();

    double mass() const override;
    short collision_group() const override;
    short collides_with() const override;
    void set_angular_factor(double x_axis, double y_axis, double z_axis) override {}
    void set_restitution(double factor) override {}
    void set_friction(double frict) override {}
    void set_damping(double linear, double angular) override {}
    void SetRespondsOnContact(bool has_response) override {}
    void SetContinuousCollisionDetection(double speed_threshold, double sphere_radius) override {}

    Ogre::Vector3 position() const override;
    Ogre::Quaternion orientation() const override;
    void set_orientation(const Ogre::Vector3& dir) override;
    Ogre::Vector3 linear_velocity() const override;
    void set_linear_velocity(const Ogre::Vector3& velocity) override {}
    Ogre::Vector3 angular_velocity() const override;
    Ogre::Vector3 GetVelocityInPoint(const Ogre::Vector3& point) const override;

    void Translate(double move_x, double move_y, double move_z) override;
    void ApplyImpulse(double imp_x, double imp_y, double imp_z) override;
    void ApplyImpulse(const Ogre::Vector3& imp, const Ogre::Vector3& relative_pos) override;
    void Rotate(double yaw, double pitch, double roll) override;
    void Scale(double factor_x, double factor_y, double factor_z) override;

    void AddCollisionAction(short target_mask, const CollisionAction& action) override {}

  protected:
    void OnTaken() override;

    Ogre::Vector3 initial_pos_;
    Ogre::Quaternion initial_dir_;
};

inline SimpleBody::SimpleBody(const Ogre::Vector3& initial_pos, const Ogre::Quaternion& initial_dir) 
    : initial_pos_(initial_pos), initial_dir_(initial_dir) {}

inline double SimpleBody::mass() const  {
    return 0.0;
}
inline short SimpleBody::collision_group() const  {
    return 0;
}
inline short SimpleBody::collides_with() const  {
    return 0;
}
inline Ogre::Vector3 SimpleBody::linear_velocity() const {
    return Ogre::Vector3::ZERO;
}
inline Ogre::Vector3 SimpleBody::angular_velocity() const {
    return Ogre::Vector3::ZERO;
}
inline Ogre::Vector3 SimpleBody::GetVelocityInPoint(const Ogre::Vector3& point) const {
    return Ogre::Vector3::ZERO;
}

template <>
class NullComponent<SimpleBody> : public NullComponent<Body>{};

} // namespace component
} // namespace mode3d
} // namespace action
} // namespace ugdk

#endif // UGDK_ACTION_3D_COMPONENT_SIMPLEBODY_H
