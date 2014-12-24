
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

void SimpleBody::Translate(double move_x, double move_y, double move_z) {
    owner()->node().translate(static_cast<Real>(move_x), static_cast<Real>(move_y), static_cast<Real>(move_z));
}

void SimpleBody::Move(double delta_x, double delta_y, double delta_z) {
    Translate(delta_x, delta_y, delta_z);
}

void SimpleBody::Rotate(double yaw, double pitch, double roll) {
    auto& node = owner()->node();
    node.yaw(static_cast<Radian>(yaw));
    node.pitch(static_cast<Radian>(pitch));
    node.roll(static_cast<Radian>(roll));
}

void SimpleBody::Scale(double factor_x, double factor_y, double factor_z) {
    owner()->node().scale(factor_x, factor_y, factor_z);
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