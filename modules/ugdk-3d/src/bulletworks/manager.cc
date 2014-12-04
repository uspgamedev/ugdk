#include <bulletworks/manager.h>

#include <bulletworks/component/physicsbody.h>

#include <btBulletDynamicsCommon.h>
#include <BtOgreExtras.h>
#include <OgreSceneManager.h>

namespace bulletworks {

using component::PhysicsBody;

void tickCallback(btDynamicsWorld *world, btScalar timeStep);

Manager::Manager(const btVector3& grav, Ogre::SceneManager* sceneMgr) {
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

    debug_drawer_ = new BtOgre::DebugDrawer(sceneMgr->getRootSceneNode(), world_);
    debug_drawer_->setDebugMode(false);
    world_->setDebugDrawer(debug_drawer_);

    world_->setInternalTickCallback(&tickCallback);
    world_->setWorldUserInfo(static_cast<void*>(this));
}

Manager::~Manager() {
    delete world_;
	delete broadphase_;
    delete dispatcher_;
    delete config_;
    delete solver_;
    delete debug_drawer_;
}

void Manager::Update(double dt) {
    // stepSimulation( dt, maxSubSteps, fixedDtSubStep=1/60)
    // dt < maxSubSteps * fixedDtSubStep
    world_->stepSimulation(dt, 10);
    debug_drawer_->step();
}

void Manager::AddBody(PhysicsBody* obj) {
    world_->addRigidBody(obj->body_, obj->physics_data_.collision_group,
                         obj->physics_data_.collides_with);
}
void Manager::RemoveBody(PhysicsBody* obj) {
    world_->removeRigidBody(obj->body_);
}

void Manager::set_debug_draw_enabled(bool enable) { 
    debug_drawer_->setDebugMode(enable);
}
bool Manager::debug_draw_enabled() { 
    return debug_drawer_->getDebugMode() != 0; 
}

void tickCallback(btDynamicsWorld *world, btScalar timeStep) {
    int numManifolds = world->getDispatcher()->getNumManifolds();
	for (int i=0; i<numManifolds; i++)
	{
		btPersistentManifold* contactManifold =  world->getDispatcher()->getManifoldByIndexInternal(i);
        PhysicsBody* obA = static_cast<PhysicsBody*>(contactManifold->getBody0()->getUserPointer());
        PhysicsBody* obB = static_cast<PhysicsBody*>(contactManifold->getBody1()->getUserPointer());

		int numContacts = contactManifold->getNumContacts();
		for (int j=0; j<numContacts; j++)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
            //std::cout << obA->entity_name() << " IS NEAR (" << pt.getDistance() << ") " << obB->entity_name() << std::endl;
			if (pt.getDistance() <= 0)
			{
                //CollisionLogic logic = static_cast<Manager*>(world->getWorldUserInfo())->collision_logic();
                // TODO call collision logic sbrubles
			}
		}
	}
}

}
