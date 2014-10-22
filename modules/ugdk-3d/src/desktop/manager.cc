#include <ugdk/desktop/3D/manager.h>

#include <ugdk/desktop/3D/window.h>
#include <ugdk/desktop/module.h>
#include <ugdk/desktop/windowsettings.h>
#include <ugdk/math/integer2D.h>
#include <ugdk/debug/log.h>
#include <ugdk/system/engine.h>

#ifdef UGDK_OGRE_STATIC
// FIXME: HELP, WE'RE HARD CODING THIS
#define OGRE_STATIC_GL
#endif

#include <OgreStaticPluginLoader.h>
#include <OgreSceneManager.h>
#include <OgreStringConverter.h>
#include <OgreRoot.h>

#include <string>
#include <sstream>
#include <vector>

namespace ugdk {
namespace desktop {
namespace mode3d {

using std::string;
using std::vector;
using std::stringstream;
using std::stoi;

Manager::Manager() {
}

Manager::~Manager() {
    delete root_;
    static_loader_->unload();
    delete static_loader_;
}

bool Manager::Initialize() {

    std::string mResourcesCfg = system::ResolvePath("resources" OGRE_BUILD_SUFFIX ".cfg");
    std::string mPluginsCfg = system::ResolvePath("plugins" OGRE_BUILD_SUFFIX ".cfg");

    // Create the Root
    root_ = new Ogre::Root(mPluginsCfg);
    
    // Load static plugins
    static_loader_ = new Ogre::StaticPluginLoader();
    static_loader_->load();
    
    // Setup resources
    Ogre::ConfigFile cf;
    cf.load(mResourcesCfg);
    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements()) {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)     {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                system::ResolvePath(archName), typeName, secName);
        }
    }
    
    // Initialize the Root
    
    // Show the configuration dialog and initialise the system
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg
    if(!root_->showConfigDialog()) {
        return false;
    }
    //root_->restoreConfig();
    
    root_->initialise(false);

    return true;
}

void Manager::Release() {
    // FIXME: DO SOMETHING?
}

void Manager::PresentAll(/*double dt*/) {
    //TODO: check if we dont need to use the overload which receives delta_t
    root_->renderOneFrame();
}

std::shared_ptr<desktop::Window> Manager::DoCreateWindow(const WindowSettings& settings) {

    Ogre::NameValuePairList params;
    Ogre::RenderWindow* window = root_->createRenderWindow(Ogre::String(settings.title), 
                                                           settings.size.x, settings.size.y, 
                                                           settings.fullscreen, &params);
    
    // Set default number of mipmaps. Not sure if this particularly needed, but...
    //Pretty sure these calls need a RenderWindow created to work, that's why they are here for now...
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    // Initialize resource groups.
    // TODO: We might need to set resource listeners, if any, before this call.    
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    
    return std::shared_ptr<desktop::Window>(new mode3d::Window(window));
}

std::shared_ptr<desktop::Window> Manager::DoCreateWindow(unsigned long hwnd) {
    //TODO: Not sure how to do this, or even if we can do this. So just return null
    return std::shared_ptr<desktop::Window>();
}


} // namespace mode3d
} // namespace desktop
} // namespace ugdk
