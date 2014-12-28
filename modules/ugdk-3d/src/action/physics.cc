#include <ugdk/action/3D/physics.h>
#include <ugdk/action/3D/component/physicsbody.h>
#include <ugdk/action/3D/scene3d.h>

#include <btBulletDynamicsCommon.h>
#include <BtOgreExtras.h>
#include <OgreSceneManager.h>

#include <vector>

namespace ugdk {
namespace action {
namespace mode3d {

using component::PhysicsBody;

//void tickCallback(btDynamicsWorld *world, btScalar timeStep);

Physics::Physics(const btVector3& grav, Scene3D* scene) {
    // Broadphase is the initial collision detecting: checks for colliding pairs given their bounding boxes
    broadphase_ = new btDbvtBroadphase();

    // Set up the collision configuration and dispatcher
    config_ = new btDefaultCollisionConfiguration();
    dispatcher_ = new btCollisionDispatcher(config_);

    // The actual physics solver
    solver_ = new btSequentialImpulseConstraintSolver;

    // The world.
    world_ = new btDiscreteDynamicsWorld(dispatcher_, broadphase_, solver_, config_);
    world_->setGravity(grav);

    btContactSolverInfo& info = world_->getSolverInfo();
    info.m_splitImpulse = 1; //enable split impulse feature

    debug_drawer_ = new BtOgre::DebugDrawer(scene->manager()->getRootSceneNode(), world_);
    debug_drawer_->setDebugMode(false);
    world_->setDebugDrawer(debug_drawer_);

    world_->setInternalTickCallback(&tickCallback);
    world_->setWorldUserInfo(static_cast<void*>(this));
}

Physics::~Physics() {
    delete world_;
	delete broadphase_;
    delete dispatcher_;
    delete config_;
    delete solver_;
    delete debug_drawer_;
}

void Physics::Update(double dt) {
    // stepSimulation( dt, maxSubSteps, fixedDtSubStep=1/60)
    // dt < maxSubSteps * fixedDtSubStep
    world_->stepSimulation(dt, static_cast<btScalar>(10));
    debug_drawer_->step();
}

void Physics::AddBody(PhysicsBody* obj) {
    world_->addRigidBody(obj->body_, obj->physics_data_.collision_group,
                         obj->physics_data_.collides_with);
}
void Physics::RemoveBody(PhysicsBody* obj) {
    world_->removeRigidBody(obj->body_);
}

void Physics::set_debug_draw_enabled(bool enable) {
    debug_drawer_->setDebugMode(enable);
}
bool Physics::debug_draw_enabled() {
    return debug_drawer_->getDebugMode() != 0; 
}

void Physics::tickCallback(btDynamicsWorld *world, btScalar timeStep) {
    int numManifolds = world->getDispatcher()->getNumManifolds();
	for (int i=0; i<numManifolds; i++)
	{
		btPersistentManifold* contactManifold =  world->getDispatcher()->getManifoldByIndexInternal(i);
        //TODO: check if we can move these variable declarations to inner for, so 
        //      that we execute this cast less often.
        PhysicsBody* bodyA = static_cast<PhysicsBody*>(contactManifold->getBody0()->getUserPointer());
        PhysicsBody* bodyB = static_cast<PhysicsBody*>(contactManifold->getBody1()->getUserPointer());

        component::ContactPointVector pointsA;
        component::ContactPointVector pointsB;
		int numContacts = contactManifold->getNumContacts();
		for (int j=0; j<numContacts; j++)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
            if (pt.getDistance() <= 0) {
                pointsA.push_back(component::ContactPoint(pt.getPositionWorldOnA(), pt.getPositionWorldOnB(),
                                                          pt.getAppliedImpulse(), pt.getDistance()));
                pointsB.push_back(component::ContactPoint(pt.getPositionWorldOnB(), pt.getPositionWorldOnA(),
                                                          pt.getAppliedImpulse(), pt.getDistance()));
            }
		}

        if (pointsA.size() > 0) {
            auto actionA = bodyA->action_map_.find(bodyB->collision_group());
            if (actionA != bodyA->action_map_.end()) {
                actionA->second(bodyA->owner(), bodyB->owner(), pointsA);
            }
            auto actionB = bodyB->action_map_.find(bodyA->collision_group());
            if (actionB != bodyB->action_map_.end()) {
                actionB->second(bodyB->owner(), bodyA->owner(), pointsB);
            }
        }
	}
}

}
}
}