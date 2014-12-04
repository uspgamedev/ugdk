#include <bulletworks/physicscene.h>
#include <bulletworks/manager.h>

#include <ugdk/system/task.h>

namespace bulletworks {

PhysicScene::PhysicScene(const btVector3& grav) {
    physics_mgr_.reset(new Manager(grav, scene_mgr_));
    
    AddTask(ugdk::system::Task([&](double dt) {
        physics_mgr_->Update(dt);
    }, 0.2));
}

void PhysicScene::AddObject(const std::shared_ptr<Object> &the_object) {
    objetcs_.push_back(the_object);
}

PhysicScene::~PhysicScene() {}

} // namespace bulletworks
