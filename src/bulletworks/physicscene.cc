#include <bulletworks/physicscene.h>
#include <bulletworks/manager.h>

#include <ugdk/system/task.h>

namespace bulletworks {

PhysicScene::PhysicScene(const btVector3& grav) {
    physics_mgr_ = new Manager(grav, scene_mgr_);
    
    AddTask(ugdk::system::Task([&](double dt) {
        physics_mgr_->Update(dt);
    }, 0.2));
}

PhysicScene::~PhysicScene() {
    delete physics_mgr_;
}

} // namespace bulletworks
