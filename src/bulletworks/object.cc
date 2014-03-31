#include <bulletworks/object.h>
#include <bulletworks/module.h>

#include <btBulletDynamicsCommon.h>

#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

#include <BtOgreExtras.h>
#include <BtOgrePG.h>


namespace bulletworks {

Object::Object(Ogre::Entity* entity, double mass) 
    : entity_(entity), mass_(mass), shape_(nullptr), body_(nullptr)
{
}

Object::~Object() {
    if (body_) {
        manager()->RemoveBody(this);
        delete body_->getMotionState();
        delete body_;
        delete shape_;
    }

    if (entity_->getParentSceneNode()) {
        Ogre::SceneNode* node = entity_->getParentSceneNode();
        Ogre::SceneManager* mgr = node->getCreator();
        node->removeAndDestroyAllChildren();
        mgr->destroySceneNode(node);
        mgr->destroyEntity(entity_);
    }
}

std::string Object::entity_name() {
    return entity_->getName();
}

void Object::SetupCollision(btCollisionShape* shape, short collisionGroup, short collidesWith, const btTransform& offset) {
    collision_group_ = collisionGroup;
    collides_with_ = collidesWith;
    shape_ = shape;

    //ObjectMotionState* motionState = new ObjectMotionState(entity);
    Ogre::Quaternion orient = entity_->getParentSceneNode()->getOrientation();
    Ogre::Vector3 pos = entity_->getParentSceneNode()->getPosition();
    btTransform t = btTransform( BtOgre::Convert::toBullet(orient), BtOgre::Convert::toBullet(pos));
    BtOgre::RigidBodyState* motionState = new BtOgre::RigidBodyState(entity_->getParentSceneNode(), t, offset);

    btVector3 inertia(0,0,0);
    if (mass_ > 0.0)
        shape_->calculateLocalInertia(static_cast<btScalar>(mass_),inertia);
    btRigidBody::btRigidBodyConstructionInfo  bodyInfo(static_cast<btScalar>(mass_),motionState,shape_,inertia);
    body_ = new btRigidBody(bodyInfo);
    body_->setLinearFactor(btVector3(1,1,1));
	body_->setActivationState(DISABLE_DEACTIVATION);
    body_->setUserPointer(this);
	manager()->AddBody(this);
}

void Object::Translate(const Ogre::Vector3& move) {
    body_->translate(BtOgre::Convert::toBullet(move));
}
void Object::Move(const Ogre::Vector3& delta) {
    body_->activate();
    body_->applyCentralImpulse(BtOgre::Convert::toBullet(delta));
}
void Object::Rotate(double yaw, double pitch, double roll) {
    body_->activate();    
    body_->applyTorque(btVector3(static_cast<btScalar>(yaw),
                                 static_cast<btScalar>(pitch),
                                 static_cast<btScalar>(roll)));

    /*btTransform t = body_->getCenterOfMassTransform();
    Ogre::Quaternion rot = BtOgre::Convert::toOgre(t.getRotation());

    Ogre::Matrix3 ypr = Ogre::Matrix3::IDENTITY;
    ypr.FromEulerAnglesXYZ(Ogre::Radian(yaw), Ogre::Radian(pitch), Ogre::Radian(roll));
    t.setRotation( BtOgre::Convert::toBullet(rot * ypr) );
    body_->setCenterOfMassTransform(t);*/
}

}
