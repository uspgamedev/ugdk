#include <ugdk/action/3D/ogrescene.h>
#include <ugdk/action/3D/camera.h>
#include <ugdk/desktop/3D/module.h>

#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreCamera.h>
#include <OgreRenderWindow.h>
#include <OgreViewport.h>
#include <OgreOverlaySystem.h>

namespace ugdk {
namespace action {
namespace threed {

OgreScene::OgreScene() {
    scene_mgr_ = ugdk::desktop::threed::manager()->root()->createSceneManager("OctreeSceneManager");
	scene_mgr_->addRenderQueueListener( new Ogre::OverlaySystem() );
	camera_ = new Camera(this);
	z_order_ = -1;
}

OgreScene::~OgreScene() {
    if (z_order_ > 0)
        ugdk::desktop::threed::manager()->window()->removeViewport(z_order_);
    delete camera_;
    ugdk::desktop::threed::manager()->root()->destroySceneManager(scene_mgr_);
}

void OgreScene::OnPushed(int index) {
    z_order_ = index;
	//TODO: we could let user set with x/y/w/h of this viewport...
	viewport_ = ugdk::desktop::threed::manager()->window()->addViewport(camera_->camera(), z_order_, 0, 0, 1, 1);
    viewport_->setBackgroundColour(Ogre::ColourValue(0,0,0));
    camera_->camera()->setAspectRatio(Ogre::Real(viewport_->getActualWidth()) / Ogre::Real(viewport_->getActualHeight()));
}

} // namespace threed
} // namespace action
} // namespace ugdk
