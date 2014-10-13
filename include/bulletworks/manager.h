#ifndef BULLETWORKS_MANAGER_H_
#define BULLETWORKS_MANAGER_H_

#include <functional>

namespace BtOgre {
class DebugDrawer;
}
namespace Ogre {
class SceneManager;
}

class btVector3;
class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
class btManifoldPoint;

namespace bulletworks {

class Object;

typedef std::function<void (Object*,Object*,btManifoldPoint&)> CollisionLogic;

class Manager {
public:
	Manager(const btVector3& grav, Ogre::SceneManager* sceneMgr);
	~Manager();

    void Update(double dt);

    void AddBody(Object* obj);
    void RemoveBody(Object* obj);

    void set_debug_draw_enabled(bool enable);
    bool debug_draw_enabled();

    void set_collision_logic(CollisionLogic& logic) { col_logic_ = logic; }
    CollisionLogic& collision_logic() { return col_logic_; }

private:
    btBroadphaseInterface* broadphase_;
    btDefaultCollisionConfiguration* config_;
    btCollisionDispatcher* dispatcher_;
    btSequentialImpulseConstraintSolver* solver_;
    btDiscreteDynamicsWorld* world_;

    BtOgre::DebugDrawer* debug_drawer_;

    CollisionLogic col_logic_;
};

}
#endif
