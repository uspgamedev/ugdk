#ifndef UGDK_ACTION_3D_OGRESCENE_H_
#define UGDK_ACTION_3D_OGRESCENE_H_

#include <ugdk/action/scene.h>

namespace Ogre {
class SceneManager;
class Viewport;
}

namespace ugdk {
namespace action {
namespace threed {

class Camera;

/**
   @class OgreScene
   @brief A 3D game scene, or screen.

   Represents a Ogre 3D scene, which contains a tree of nodes (entities) to
   render, and the associated camera and viewport.
*/
class OgreScene : public ugdk::action::Scene {
  public:
    OgreScene();
    virtual ~OgreScene();
    
    /// Method called when this scene is pushed to the Engine's Scene stack.
    virtual void OnPushed(int index);
    
    Ogre::SceneManager* manager() const { return scene_mgr_; }

  protected:
    
    Ogre::SceneManager* scene_mgr_;
    Camera* camera_;
    int z_order_;
    Ogre::Viewport* viewport_;
}; // class OgreScene.

} // namespace 3D
} /* namespace action */
} /* namespace ugdk */

#endif /* UGDK_ACTION_3D_OGRESCENE_H_ */
