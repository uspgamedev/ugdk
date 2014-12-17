#ifndef UGDK_ACTION_3D_COMPONENT_SIMPLEBODY_H
#define UGDK_ACTION_3D_COMPONENT_SIMPLEBODY_H

#include <ugdk/action/3D/component/body.h>

namespace ugdk {
namespace action {
namespace mode3d {

namespace component {

class SimpleBody final : public Body {
  public:
    using Body::Move;

    SimpleBody(const Ogre::Vector3& initial_pos, const Ogre::Quaternion& initial_dir);
    ~SimpleBody();

    double mass() const override;
    short collision_group() const override;
    short collides_with() const override;
    void set_angular_factor(double x_axis, double y_axis, double z_axis) override {}
    void set_restitution(double factor) override {}

    void Translate(double move_x, double move_y, double move_z) override;
    void Move(double delta_x, double delta_y, double delta_z) override;
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

template <>
class NullComponent<SimpleBody> : public NullComponent<Body>{};

} // namespace component
} // namespace mode3d
} // namespace action
} // namespace ugdk

#endif // UGDK_ACTION_3D_COMPONENT_SIMPLEBODY_H
