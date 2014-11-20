#include <bulletworks/object.h>
#include <bulletworks/manager.h>
#include <bulletworks/physicscene.h>

#include <btBulletDynamicsCommon.h>

#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

#include <BtOgreExtras.h>
#include <BtOgrePG.h>


namespace bulletworks {

Object::PhysicsData::PhysicsData() {
    mass = 1.0;
    collision_group = collides_with = 0;
    shape = nullptr;
    auto identity = BtOgre::Convert::toBullet(Ogre::Quaternion::IDENTITY);
    initial = btTransform(identity, btVector3(0, 0, 0));
    offset = btTransform(identity, btVector3(0, 0, 0));
}

Object::Object(Ogre::Entity* entity, const PhysicsData& physics_data)
    : entity_(entity), physics_data_(physics_data), body_(nullptr)
{
}

Object::~Object() {
    if (body_) {
        parent_physics_mgr_->RemoveBody(this);
        delete body_->getMotionState();
        delete body_;
        delete physics_data_.shape;
    }

    node_->getCreator()->destroyEntity(entity_);
    //OgreEntity takes care of destroying our node_
}

std::string Object::entity_name() {
    return entity_->getName();
}

void Object::AddToScene(bulletworks::PhysicScene* scene) {
    node_ = scene->manager()->getRootSceneNode()->createChildSceneNode();
    node_->attachObject(entity_);
    node_->setPosition(BtOgre::Convert::toOgre(physics_data_.initial.getOrigin()));
    node_->setOrientation(BtOgre::Convert::toOgre(physics_data_.initial.getRotation()));
        
    if (physics_data_.shape != nullptr) {
        this->setupCollision();
        parent_physics_mgr_ = scene->physics_manager();
        parent_physics_mgr_->AddBody(this);
    }
}

void Object::setupCollision() {
    BtOgre::RigidBodyState* motionState = new BtOgre::RigidBodyState(node_, physics_data_.initial, physics_data_.offset);

    btVector3 inertia(0,0,0);
    if (physics_data_.mass > 0.0)
        physics_data_.shape->calculateLocalInertia(static_cast<btScalar>(physics_data_.mass), inertia);
    btRigidBody::btRigidBodyConstructionInfo  bodyInfo(static_cast<btScalar>(physics_data_.mass), motionState, physics_data_.shape, inertia);
    body_ = new btRigidBody(bodyInfo);
    body_->setLinearFactor(btVector3(1,1,1));
    body_->setActivationState(DISABLE_DEACTIVATION);
    body_->setUserPointer(this);
}

void Object::Translate(const Ogre::Vector3& move) {
    Translate(move.x, move.y, move.z);
}
void Object::Translate(double move_x, double move_y, double move_z) {
    if (body_)
        body_->translate(btVector3(move_x, move_y, move_z));
}

void Object::Move(const Ogre::Vector3& delta) {
    Move(delta.x, delta.y, delta.z);
}
void Object::Move(double delta_x, double delta_y, double delta_z) {
    if (body_) {
        body_->activate();
        body_->applyCentralImpulse(btVector3(delta_x, delta_y, delta_z));
    }
}

void Object::Rotate(double yaw, double pitch, double roll) {
    if (body_) {
        body_->activate();
        body_->applyTorque(btVector3(static_cast<btScalar>(yaw),
            static_cast<btScalar>(pitch),
            static_cast<btScalar>(roll)));
    }
    /*btTransform t = body_->getCenterOfMassTransform();
    Ogre::Quaternion rot = BtOgre::Convert::toOgre(t.getRotation());

    Ogre::Matrix3 ypr = Ogre::Matrix3::IDENTITY;
    ypr.FromEulerAnglesXYZ(Ogre::Radian(yaw), Ogre::Radian(pitch), Ogre::Radian(roll));
    t.setRotation( BtOgre::Convert::toBullet(rot * ypr) );
    body_->setCenterOfMassTransform(t);*/
}

}
