
#include <bulletworks/component/physicsbody.h>
#include <bulletworks/object.h>
#include <bulletworks/manager.h>

#include <btBulletDynamicsCommon.h>
#include <OgreSceneNode.h>
#include <BtOgrePG.h>

namespace bulletworks {
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
        manager_.RemoveBody(this);
        delete body_->getMotionState();
        delete body_;
        delete physics_data_.shape;
    }
}

void PhysicsBody::set_angular_factor(double x_axis, double y_axis, double z_axis) {
    body_->setAngularFactor(btVector3(x_axis, y_axis, z_axis));
}

void PhysicsBody::Translate(double move_x, double move_y, double move_z) {
    body_->translate(btVector3(move_x, move_y, move_z));
}

void PhysicsBody::Move(double delta_x, double delta_y, double delta_z) {
    body_->activate();
    body_->applyCentralImpulse(btVector3(delta_x, delta_y, delta_z));
}

void PhysicsBody::Rotate(double yaw, double pitch, double roll) {
    body_->activate();
    body_->applyTorque(btVector3(static_cast<btScalar>(yaw),
                                 static_cast<btScalar>(pitch),
                                 static_cast<btScalar>(roll)));
}

void PhysicsBody::Scale(double factor_x, double factor_y, double factor_z) {
    owner()->node().scale(factor_x, factor_y, factor_z);
    body_->getCollisionShape()->setLocalScaling(btVector3(factor_x, factor_y, factor_z));
}

void PhysicsBody::set_restitution(double factor) {
    body_->setRestitution(factor);
}

void PhysicsBody::OnTake() {
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
    manager_.AddBody(this);
}

} // namespace component
} // namespace bulletworks
