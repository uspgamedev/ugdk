#include <ugdk/graphic/3D/manager.h>

#include <OgreSceneManager.h>

namespace ugdk {
namespace graphic {
namespace 3D {

Manager* Manager::reference_ = nullptr;

Manager::Manager(const std::string& window_title) : window_title_(window_title) {
}

Manager::~Manager() {
    //if (mTrayMgr) delete mTrayMgr;
    //if (mCameraMan) delete mCameraMan;
    delete root_;
    static_loader_.unload();
}

bool Manager::Initialize() {
    // Create the Root
    root_ = new Ogre::Root("");
    
    // Load static plugins
    static_loader_.load();
    
    // Setup resources
    Ogre::ConfigFile cf;
    cf.load("resources.cfg");
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
                archName, typeName, secName);
        }
    }
    
    // Initialize the Root
    
    // Show the configuration dialog and initialise the system
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg
    if(!root_->showConfigDialog()) {
        return false;
    }        
    // If returned true, user clicked OK so initialise
    // Here we choose to let the system create a default rendering window by passing 'true'
    window_ = root_->initialise(true, window_title_);
    
    // Set default number of mipmaps.
    // Not sure if this particularly needed, but...
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    // Initialize resource groups.
    // TODO: We might need to set resource listeners, if any, before this call.    
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    return true;
}


} // namespace 3D
} // namespace graphic
} // namespace ugdk
