#ifndef UGDK_ACTION_3D_PHYSICS_H_
#define UGDK_ACTION_3D_PHYSICS_H_

#include <functional>

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

    void set_debug_draw_enabled(bool enable);
    bool debug_draw_enabled();

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
