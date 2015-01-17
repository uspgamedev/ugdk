
#include <ugdk/action/3D/component/simplebody.h>
#include <ugdk/action/3D/element.h>

#include <OgreSceneNode.h>

namespace ugdk {
namespace action {
namespace mode3d {
namespace component {

using Ogre::Real;
using Ogre::Radian;

SimpleBody::~SimpleBody() {
    
}

Ogre::Vector3 SimpleBody::position() const {
    return owner()->node().getPosition();
}
Ogre::Quaternion SimpleBody::orientation() const {
    return owner()->node().getOrientation();
}
void SimpleBody::set_orientation(const Ogre::Vector3& dir) {
    owner()->node().setOrientation(Ogre::Vector3::UNIT_Z.getRotationTo(dir));
}

void SimpleBody::Translate(double move_x, double move_y, double move_z) {
    owner()->node().translate(static_cast<Real>(move_x), static_cast<Real>(move_y), static_cast<Real>(move_z));
}

void SimpleBody::ApplyImpulse(double imp_x, double imp_y, double imp_z) {
    Translate(imp_x, imp_y, imp_z);
}
void SimpleBody::ApplyImpulse(const Ogre::Vector3& imp, const Ogre::Vector3& relative_pos) {
    ApplyImpulse(imp);
}

void SimpleBody::Rotate(double yaw, double pitch, double roll) {
    auto& node = owner()->node();
    node.yaw(static_cast<Radian>(yaw));
    node.pitch(static_cast<Radian>(pitch));
    node.roll(static_cast<Radian>(roll));
}

void SimpleBody::Scale(double factor_x, double factor_y, double factor_z) {
    owner()->node().scale(static_cast<Ogre::Real>(factor_x), 
                          static_cast<Ogre::Real>(factor_y), 
                          static_cast<Ogre::Real>(factor_z));
}

void SimpleBody::OnTaken() {
    auto& node = owner()->node();
    node.setPosition(initial_pos_);
    node.setOrientation(initial_dir_);
}

} // namespace component
} // namespace mode3d
} // namespace action
} // namespace ugdk