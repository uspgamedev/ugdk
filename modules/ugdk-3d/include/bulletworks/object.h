#ifndef BULLETWORKS_OBJECT_H_
#define BULLETWORKS_OBJECT_H_

#include <ugdk/action/3D/ogreentity.h>
#include <ugdk/action.h>

#include <LinearMath/btTransform.h>

#include <string>

namespace Ogre {
class Entity;
class Vector3;
}

class btCollisionShape;
class btRigidBody;

namespace bulletworks {
class Manager;

class Object : ugdk::action::mode3d::OgreEntity {
public:
    struct PhysicsData {
        double mass;                // rigid body mass (must be >= 0, and 0 means static "infinite mass" object)
        short collision_group;      // flag (power of 2 bit mask) of the collision group this object belongs to
        short collides_with;        // flags (combination of flags - |'ed) of groups this object collides with
        btCollisionShape* shape;    // collision shape of this object
        btTransform initial;        // initial position and orientation of the object
        btTransform offset;         // offset to center of mass
    };

    Object(Ogre::Entity* entity, const PhysicsData& physics_data);
    ~Object();

    virtual void OnSceneAdd(ugdk::action::Scene* scene);

    Ogre::Entity* entity() const { return entity_; }
	btRigidBody* body() const { return body_; }
    std::string entity_name();

    double mass() const { return physics_data_.mass; }
    short collision_group() const { return physics_data_.collision_group; }
    short collides_with() const { return physics_data_.collides_with; }

    void Translate(const Ogre::Vector3& move);
    void Move(const Ogre::Vector3& delta);
    void Rotate(double yaw, double pitch, double roll);

protected:
	Ogre::Entity* entity_;

    PhysicsData physics_data_;
	btRigidBody* body_;

    Manager* parent_physics_mgr_;
    void setupCollision();
};

}
#endif
