#ifndef UGDK_ACTION_3D_PHYSICS_H_
#define UGDK_ACTION_3D_PHYSICS_H_

#include <functional>
#include <forward_list>
#include <memory>

namespace BtOgre {
class DebugDrawer;
}

class btVector3;
class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDynamicsWorld;
class btDiscreteDynamicsWorld;
class btManifoldPoint;

namespace ugdk {
namespace action {
namespace mode3d {

class Scene3D;
class Element;
namespace component {
class PhysicsBody;
}

class Physics {
public:
    Physics(const btVector3& grav, Scene3D* scene);
    ~Physics();

    void Update(double dt);

    void AddBody(component::PhysicsBody* obj);
    void RemoveBody(component::PhysicsBody* obj);

    /** Performs a spherical contact/collision query on the physics world, returning a list with the objects in contact (inside the sphere). */
    std::forward_list<std::weak_ptr<Element>> ContactQuery(short collision_groups, const btVector3& pos, double radius);

    /** If physics debug drawing is enabled. If it is, UGDK will render a wireframe model of the collision objects. */
    bool debug_draw_enabled();
    void set_debug_draw_enabled(bool enable);

private:
    btBroadphaseInterface* broadphase_;
    btDefaultCollisionConfiguration* config_;
    btCollisionDispatcher* dispatcher_;
    btSequentialImpulseConstraintSolver* solver_;
    btDiscreteDynamicsWorld* world_;

    BtOgre::DebugDrawer* debug_drawer_;

    static void tickCallback(btDynamicsWorld *world, float timeStep);
};

} // namespace mode3d
} // namespace action
} // namespace ugdk

#endif // UGDK_ACTION_3D_PHYSICS_H_
