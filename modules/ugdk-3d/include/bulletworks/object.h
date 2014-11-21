#ifndef BULLETWORKS_OBJECT_H_
#define BULLETWORKS_OBJECT_H_

#include <ugdk/action/3D/element.h>
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
class PhysicScene;

class Object : public ugdk::action::mode3d::Element {
public:
    struct PhysicsData {
        double mass;                // rigid body mass (must be >= 0, and 0 means static "infinite mass" object)
        short collision_group;      // flag (power of 2 bit mask) of the collision group this object belongs to
        short collides_with;        // flags (combination of flags - |'ed) of groups this object collides with
        btCollisionShape* shape;    // collision shape of this object
        btTransform initial;        // initial position and orientation of the object
        btTransform offset;         // offset to center of mass

        PhysicsData();
    };

    Object(PhysicScene& scene, Ogre::Entity* entity, const PhysicsData& physics_data);
    ~Object();

    void AddToScene(bulletworks::PhysicScene* scene);

    Ogre::Entity* entity() const { return entity_; }
    btRigidBody* body() const { return body_; }
    std::string entity_name();

    double mass() const { return physics_data_.mass; }
    short collision_group() const { return physics_data_.collision_group; }
    short collides_with() const { return physics_data_.collides_with; }

    void Translate(const Ogre::Vector3& move);
    void Translate(double move_x, double move_y, double move_z);
    void Move(const Ogre::Vector3& delta);
    void Move(double delta_x, double delta_y, double delta_z);
    void Rotate(double yaw, double pitch, double roll);
    void Scale(double factor_x, double factor_y, double factor_z);

protected:
    Ogre::Entity* entity_;

    PhysicsData physics_data_;
    btRigidBody* body_;

    Manager* parent_physics_mgr_;
    void setupCollision();
};

}
#endif
