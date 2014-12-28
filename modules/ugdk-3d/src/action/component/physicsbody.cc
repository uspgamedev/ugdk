
#include <ugdk/action/3D/component/physicsbody.h>
#include <ugdk/action/3D/element.h>
#include <ugdk/action/3D/physics.h>

#include <btBulletDynamicsCommon.h>
#include <OgreSceneNode.h>
#include <BtOgrePG.h>

namespace ugdk {
namespace action {
namespace mode3d {
namespace component {

PhysicsBody::PhysicsData::PhysicsData() {
    mass = 1.0;
    collision_group = collides_with = 0;
    shape = nullptr;
    auto identity = BtOgre::Convert::toBullet(Ogre::Quaternion::IDENTITY);
    initial = btTransform(identity, btVector3(0, 0, 0));
    offset = btTransform(identity, btVector3(0, 0, 0));
}

PhysicsBody::~PhysicsBody() {
    if (body_) {
        physics_.RemoveBody(this);
        delete body_->getMotionState();
        delete body_;
        delete physics_data_.shape;
    }
}

void PhysicsBody::set_angular_factor(double x_axis, double y_axis, double z_axis) {
    body_->setAngularFactor(btVector3(static_cast<btScalar>(x_axis), 
                                      static_cast<btScalar>(y_axis), 
                                      static_cast<btScalar>(z_axis)));
}

Ogre::Vector3 PhysicsBody::position() const {
    return BtOgre::Convert::toOgre(body_->getCenterOfMassPosition());
}
Ogre::Quaternion PhysicsBody::orientation() const {
    return BtOgre::Convert::toOgre(body_->getOrientation());
}
Ogre::Vector3 PhysicsBody::linear_velocity() const {
    return BtOgre::Convert::toOgre(body_->getLinearVelocity());
}
Ogre::Vector3 PhysicsBody::angular_velocity() const {
    return BtOgre::Convert::toOgre(body_->getAngularVelocity());
}
Ogre::Vector3 PhysicsBody::GetVelocityInPoint(const Ogre::Vector3& point) const {
    return BtOgre::Convert::toOgre(body_->getVelocityInLocalPoint(BtOgre::Convert::toBullet(point)));
}

void PhysicsBody::Translate(double move_x, double move_y, double move_z) {
    body_->translate(btVector3(static_cast<btScalar>(move_x), 
                               static_cast<btScalar>(move_y), 
                               static_cast<btScalar>(move_z)));
}

void PhysicsBody::Move(double delta_x, double delta_y, double delta_z) {
    body_->activate();
    body_->applyCentralImpulse(btVector3(static_cast<btScalar>(delta_x), 
                                         static_cast<btScalar>(delta_y), 
                                         static_cast<btScalar>(delta_z)));
}

void PhysicsBody::Rotate(double yaw, double pitch, double roll) {
    body_->activate();
    body_->applyTorque(btVector3(static_cast<btScalar>(yaw),
                                 static_cast<btScalar>(pitch),
                                 static_cast<btScalar>(roll)));
}

void PhysicsBody::Scale(double factor_x, double factor_y, double factor_z) {
    owner()->node().scale(static_cast<Ogre::Real>(factor_x), 
                          static_cast<Ogre::Real>(factor_y),
                          static_cast<Ogre::Real>(factor_z));
    body_->getCollisionShape()->setLocalScaling(btVector3(static_cast<btScalar>(factor_x), 
                                                          static_cast<btScalar>(factor_y), 
                                                          static_cast<btScalar>(factor_z)));
}

void PhysicsBody::set_restitution(double factor) {
    body_->setRestitution(static_cast<btScalar>(factor));
}

void PhysicsBody::set_friction(double frict) {
    body_->setFriction(static_cast<btScalar>(frict));
}

void PhysicsBody::set_damping(double linear, double angular) {
    body_->setDamping(static_cast<btScalar>(linear), static_cast<btScalar>(angular));
}

void PhysicsBody::OnTaken() {
    BtOgre::RigidBodyState* motionState = new BtOgre::RigidBodyState(&owner()->node(),
                                                                     physics_data_.initial,
                                                                     physics_data_.offset);

    btVector3 inertia(0,0,0);
    if (physics_data_.mass > 0.0)
        physics_data_.shape->calculateLocalInertia(static_cast<btScalar>(physics_data_.mass),
                                                   inertia);
    btRigidBody::btRigidBodyConstructionInfo bodyInfo(static_cast<btScalar>(physics_data_.mass),
                                                      motionState, physics_data_.shape, inertia);
    body_ = new btRigidBody(bodyInfo);
    body_->setLinearFactor(btVector3(1,1,1));
    body_->setActivationState(DISABLE_DEACTIVATION);
    body_->setUserPointer(this);
    physics_.AddBody(this);
}

void PhysicsBody::AddCollisionAction(short target_mask, const CollisionAction& action) {
    action_map_[target_mask] = action;
}

} // namespace component
} // namespace mode3d
} // namespace action
} // namespace ugdk