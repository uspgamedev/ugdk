#ifndef BULLETWORKS_COMPONENT_BODY_H
#define BULLETWORKS_COMPONENT_BODY_H

#include <bulletworks/component.h>
#include <OgreVector3.h>

namespace Ogre {
class Vector3;
}

namespace bulletworks {
namespace component {

class Body : public Component {
  public:
    std::type_index GetType() const override;
    virtual void Translate(const Ogre::Vector3& move);
    virtual void Move(const Ogre::Vector3& delta);
    virtual void Translate(double move_x, double move_y, double move_z) = 0;
    virtual void Move(double delta_x, double delta_y, double delta_z) = 0;
    virtual void Rotate(double yaw, double pitch, double roll) = 0;
    virtual void Scale(double factor_x, double factor_y, double factor_z) = 0;
  protected:
    Body () {}
    void OnTake() override;
};

inline std::type_index Body::GetType() const {
    return typeid(Body);
}

inline void Body::Translate(const Ogre::Vector3 &move) {
    this->Translate(move.x, move.y, move.z);
}

inline void Body::Move(const Ogre::Vector3 &delta) {
    this->Translate(delta.x, delta.y, delta.z);
}

inline void Body::OnTake() {}

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
} // namespace bulletworks

#endif // BULLETWORKS_COMPONENT_BODY_H
