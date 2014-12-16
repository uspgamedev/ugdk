#ifndef BULLETWORKS_PHYSICSCENE_H_
#define BULLETWORKS_PHYSICSCENE_H_

#include <ugdk/action/3D/ogrescene.h>

class btVector3;

namespace bulletworks {

class Manager;
class Object;

/**
   @class PhysicScene
   @brief A physics game scene.

   Represents a funcional 3D game scene, with 3D graphics and physics simulation support.
*/
class PhysicScene : public ugdk::action::mode3d::OgreScene {
  public:
    PhysicScene(const btVector3& grav);
    virtual ~PhysicScene();
    
    Manager& physics_manager() const { return *physics_mgr_; }
    //void AddObject(const std::unique_ptr<Object> &the_object);

  protected:
    std::unique_ptr<Manager> physics_mgr_;
    //std::vector<std::unique_ptr<Object>> objetcs_;
};

} // namespace bulletworks

#endif /* BULLETWORKS_PHYSICSCENE_H_ */
