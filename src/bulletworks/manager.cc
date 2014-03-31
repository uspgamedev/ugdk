#include <bulletworks/manager.h>

#include <bulletworks/object.h>

#include <btBulletDynamicsCommon.h>
#include <BtOgreExtras.h>
#include <OgreSceneManager.h>

namespace bulletworks {

Manager* Manager::reference_ = nullptr;

Manager::Manager(const btVector3& grav, Ogre::SceneManager* sceneMgr) :
    gravity_(grav), scene_mgr_(sceneMgr)
{
}

Manager::~Manager() {
	delete broadphase_;
    delete config_;
    delete dispatcher_;
    delete solver_;
    delete debug_drawer_;
    delete world_;
}

void tickCallback(btDynamicsWorld *world, btScalar timeStep);

bool Manager::Initialize() {
	// Broadphase is the initial collision detecting: checks for colliding pairs given their bounding boxes
	broadphase_ = new btDbvtBroadphase();
 
    // Set up the collision configuration and dispatcher
    config_ = new btDefaultCollisionConfiguration();
    dispatcher_ = new btCollisionDispatcher(config_);
 
    // The actual physics solver
    solver_ = new btSequentialImpulseConstraintSolver;
 
    // The world.
    world_ = new btDiscreteDynamicsWorld(dispatcher_,broadphase_,solver_,config_);
    world_->setGravity(gravity_);

	btContactSolverInfo& info = world_->getSolverInfo();
	info.m_splitImpulse = 1; //enable split impulse feature
    
    debug_drawer_ = new BtOgre::DebugDrawer(scene_mgr_->getRootSceneNode(), world_);
    debug_drawer_->setDebugMode(false);
    world_->setDebugDrawer(debug_drawer_);

    world_->setInternalTickCallback( &tickCallback );
    
    return true;
}

void Manager::Update(double dt) {
    // stepSimulation( dt, maxSubSteps, fixedDtSubStep=1/60)
    // dt < maxSubSteps * fixedDtSubStep
    world_->stepSimulation(dt, 10);
    debug_drawer_->step();
}

void Manager::AddBody(Object* obj) {
    world_->addRigidBody(obj->body(), obj->collision_group(), obj->collides_with());
}
void Manager::RemoveBody(Object* obj) {
    world_->removeRigidBody(obj->body());
}

void Manager::set_debug_draw_enabled(bool enable) { 
    debug_drawer_->setDebugMode(enable);
}
bool Manager::debug_draw_enabled() { 
    return debug_drawer_->getDebugMode() != 0; 
}

void tickCallback(btDynamicsWorld *world, btScalar timeStep) {
    int numManifolds = world->getDispatcher()->getNumManifolds();
	for (int i=0;i<numManifolds;i++)
	{
		btPersistentManifold* contactManifold =  world->getDispatcher()->getManifoldByIndexInternal(i);
        Object* obA = static_cast<Object*>(contactManifold->getBody0()->getUserPointer());
		Object* obB = static_cast<Object*>(contactManifold->getBody1()->getUserPointer());

		int numContacts = contactManifold->getNumContacts();
		for (int j=0;j<numContacts;j++)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
            //std::cout << obA->entity_name() << " IS NEAR (" << pt.getDistance() << ") " << obB->entity_name() << std::endl;
			if (pt.getDistance() <= 0)
			{
                CollisionLogic& logic = Manager::reference()->collision_logic();
                if (logic) {
                    logic(obA, obB, pt);
                }
                //std::cout << obA->entity_name() << "(" << ptA.x() << ", " << ptA.y() << ", "<<ptA.z() << ") collided with " << 
                //    obB->entity_name() << "(" << ptB.x() << ", " << ptB.y() << ", "<<ptB.z() << ")" << std::endl;
			}
		}
	}
}

}
