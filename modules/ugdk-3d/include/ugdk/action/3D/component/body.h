#ifndef UGDK_ACTION_3D_COMPONENT_BODY_H
#define UGDK_ACTION_3D_COMPONENT_BODY_H

#include <ugdk/action/3D/component.h>
#include <OgreVector3.h>
#include <functional>
#include <vector>

namespace Ogre {
class Vector3;
}

class btManifoldPoint;

namespace ugdk {
namespace action {
namespace mode3d {
namespace component {

using ElementPtr = std::shared_ptr < Element > ;
using ManifoldPointVector = std::vector < btManifoldPoint > ;
using CollisionAction = std::function < void(const ElementPtr&, const ElementPtr&, const ManifoldPointVector&) >;

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
