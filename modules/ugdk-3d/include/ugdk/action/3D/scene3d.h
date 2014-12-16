#ifndef UGDK_ACTION_3D_OGRESCENE_H_
#define UGDK_ACTION_3D_OGRESCENE_H_

#include <ugdk/action/scene.h>
#include <bulletworks/manager.h>

namespace Ogre {
class SceneManager;
class Viewport;
class Overlay;
class OverlaySystem;
}

namespace ugdk {
namespace action {
namespace mode3d {

class Camera;
class Element;

/**
   @class OgreScene
   @brief A 3D game scene, or screen.

   Represents a Ogre 3D scene, which contains a tree of nodes (entities) to
   render, and the associated camera and viewport.
*/
class OgreScene : public ugdk::action::Scene {
//TODO: this does not allow showing at the same time multiple cameras from the same scene (splitscreen coop for example).
public:
    OgreScene();
    virtual ~OgreScene();
    
    /// Method called when this scene is pushed to the Engine's Scene stack.
    virtual void OnPushed(int index);
    
    void ShowFrameStats();
    bool IsFrameStatsVisible();
    void UpdateFrameStats();
    void HideFrameStats();

    std::shared_ptr<Element>& AddElement();
    const std::unique_ptr<bulletworks::Manager>& physics_manager() const {
        return physics_mgr_;
    }
    
    Ogre::SceneManager* manager() const { return scene_mgr_; }
    Camera* camera() const { return camera_; }

protected:
    Ogre::SceneManager* scene_mgr_;
    Ogre::OverlaySystem* overlay_system_;
    Camera* camera_;
    int z_order_;
    Ogre::Viewport* viewport_;
    Ogre::Overlay* fps_stats_;

    std::unique_ptr<bulletworks::Manager> physics_mgr_;
    std::vector<std::shared_ptr<Element>> elements_;
    
}; // class OgreScene.

} // namespace 3D
} /* namespace action */
} /* namespace ugdk */

#endif /* UGDK_ACTION_3D_OGRESCENE_H_ */
