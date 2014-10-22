#include <ugdk/action/3D/ogrescene.h>
#include <ugdk/action/3D/camera.h>

#include <ugdk/desktop/module.h>
#include <ugdk/desktop/3D/manager.h>

#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreCamera.h>
#include <OgreRenderWindow.h>
#include <OgreViewport.h>
#include <OgreOverlaySystem.h>
#include <OgreFont.h>

#include <iostream>

namespace ugdk {
namespace action {
namespace mode3d {

namespace {
ugdk::desktop::mode3d::Manager* desktop_manager() {
    return dynamic_cast<desktop::mode3d::Manager*>(desktop::manager());
}
}

OgreScene::OgreScene() {
    scene_mgr_ = desktop_manager()->root()->createSceneManager("OctreeSceneManager");
    overlay_system_ = new Ogre::OverlaySystem();
	scene_mgr_->addRenderQueueListener( overlay_system_ );
	camera_ = new Camera(this);
	z_order_ = -1;
	fps_stats_ = nullptr;
}

OgreScene::~OgreScene() {
    auto mgr = desktop_manager();
    if (z_order_ > 0)
        mgr->window().removeViewport(z_order_);
    delete camera_;
    mgr->root()->destroySceneManager(scene_mgr_);
}

void OgreScene::OnPushed(int index) {
    z_order_ = index;
	//TODO: we could let user set with x/y/w/h of this viewport...
    viewport_ = desktop_manager()->window().addViewport(camera_->camera(), z_order_, 0, 0, 1, 1);
    viewport_->setBackgroundColour(Ogre::ColourValue(0,0,0));
    viewport_->setOverlaysEnabled(true);
    camera_->camera()->setAspectRatio(Ogre::Real(viewport_->getActualWidth()) / Ogre::Real(viewport_->getActualHeight()));
    std::cout << "ZOrder = " << z_order_ << std::endl;
    std::cout << "Viewport size = " << viewport_->getActualWidth() << " x " << viewport_->getActualHeight() << std::endl;
}

void OgreScene::ShowFrameStats() {
    if (fps_stats_ == nullptr) {
        Ogre::OverlayManager* overlay_mgr = Ogre::OverlayManager::getSingletonPtr();
        std::string over_name = identifier() + "_FrameStats";
        fps_stats_ = overlay_mgr->create(over_name);
        fps_stats_->setZOrder(600);
        
        Ogre::TextAreaOverlayElement* avgFPS = static_cast<Ogre::TextAreaOverlayElement*>(overlay_mgr->createOverlayElement("TextArea", over_name+"/AvgFPS"));
        avgFPS->setMetricsMode(Ogre::GMM_PIXELS);
        avgFPS->setPosition(0.0, 0.0);
        avgFPS->setDimensions(200, 16);
        Ogre::FontPtr font = Ogre::FontManager::getSingleton().create("testeFont", "General");
        font->setType(Ogre::FT_TRUETYPE);
        font->setSource("cuckoo.ttf");
        font->setTrueTypeSize(16);
        font->setTrueTypeResolution(96);
        Ogre::FontManager::getSingleton().getByName("testeFont")->load();

        avgFPS->setFontName("testeFont");
        avgFPS->setCharHeight(16);
        avgFPS->setColour(Ogre::ColourValue(1.0, 0.0, 0.0));
        avgFPS->setCaption("Average FPS: WAT");
        
        Ogre::OverlayContainer* panel = static_cast<Ogre::OverlayContainer*>( overlay_mgr->createOverlayElement("Panel", over_name+"/Panel") );
        panel->setMetricsMode(Ogre::GMM_PIXELS);
        panel->setPosition(0.0, 0.0);
        panel->setDimensions(200, 60);
        panel->setMaterialName( "BaseWhite" );
        panel->addChild(avgFPS);
        fps_stats_->add2D(panel);

        AddTask(ugdk::system::Task([&](double dt) {
            this->UpdateFrameStats();
        }, 1.0));
    }
    fps_stats_->show();
}

bool OgreScene::IsFrameStatsVisible() {
    if (fps_stats_ != nullptr)
        return fps_stats_->isVisible();
    return false;
}

void OgreScene::HideFrameStats() {
    //if (fps_stats_ != nullptr)
    //    fps_stats_->hide();
}

void OgreScene::UpdateFrameStats() {
    if (!IsFrameStatsVisible()) return;
    
    auto stats = desktop_manager()->window().getStatistics();
    std::string over_name = identifier() + "_FrameStats";
    Ogre::OverlayContainer* panel = fps_stats_->getChild(over_name + "/Panel");
    Ogre::TextAreaOverlayElement* avgFPS = static_cast<Ogre::TextAreaOverlayElement*>(panel->getChild(over_name + "/AvgFPS"));
    avgFPS->setCaption("Avg FPS = " + std::to_string(stats.avgFPS));
    //std::cout << "Avg FPS = " << stats.avgFPS << std::endl;

    /*log("Best FPS = " << stats.bestFPS);
    log("Worst FPS = " << stats.worstFPS);
    log("triangle count = " << stats.triangleCount);
    log("best frame time = " << stats.bestFrameTime);
    log("worst frame time = " << stats.worstFrameTime);*/
    
}

} // namespace mode3d
} // namespace action
} // namespace ugdk
