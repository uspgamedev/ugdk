#include <ugdk/desktop/3D/manager.h>
#include <ugdk/desktop/window.h>

#include <OgreStaticPluginLoader.h>
#include <OgreSceneManager.h>
#include <OgreStringConverter.h>
#include <OgreRoot.h>

#include <SDL.h>
#include <SDL_syswm.h>
#include <SDL_endian.h>


namespace ugdk {
namespace desktop {
namespace threed {

Manager::Manager() {
}

Manager::~Manager() {
    //if (mTrayMgr) delete mTrayMgr;
    //if (mCameraMan) delete mCameraMan;
    delete root_;
    static_loader_->unload();
    delete static_loader_;
}

bool Manager::Initialize() {
    // Create the Root
    root_ = new Ogre::Root("");
    
    // Load static plugins
    static_loader_ = new Ogre::StaticPluginLoader();
    static_loader_->load();
    
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
    
    root_->initialise(false);

    return true;
}

bool Manager::AddWindow(const std::shared_ptr<Window>& window) {
    //TODO: refactor this class (and whatever else is necessary) to allow for multiple windows
    //      like ugdk's desktop module.
    
    // If returned true, user clicked OK so initialise
    // Here we choose to let the system create a default rendering window by passing 'true'
    //window_ = root_->initialise(true, window_title_);
    if (!createWindowFromSDL(window))
        return false;
        
    
    /**** the following was on Initialize, after creating window. However after refactoring the
          create window code to this, it gave segfault, meaning it needed a created window...
          For now, we do this.
    *****/    
    // Set default number of mipmaps.
    // Not sure if this particularly needed, but...
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    // Initialize resource groups.
    // TODO: We might need to set resource listeners, if any, before this call.    
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    return true;
}

void Manager::PresentAll(double dt) {
    root_->renderOneFrame(static_cast<Ogre::Real>(dt));
}

bool Manager::createWindowFromSDL(const std::shared_ptr<Window>& window) {
    //get the native whnd
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);

    if (SDL_GetWindowWMInfo(window->sdl_window_, &wmInfo) == SDL_FALSE)
        return false; //Couldn't get WM Info!

    Ogre::String winHandle;
    Ogre::NameValuePairList params; //TODO: this could (should?) be a parameter

    switch (wmInfo.subsystem)
    {
#ifdef WIN32
    case SDL_SYSWM_WINDOWS:
        // Windows code
        winHandle = Ogre::StringConverter::toString((unsigned long)wmInfo.info.win.window);
        break;
#elif __MACOSX__
    case SDL_SYSWM_COCOA:
        //required to make OGRE play nice with our window
        /*params.insert(std::make_pair("macAPI", "cocoa"));
        params.insert(std::make_pair("macAPICocoaUseNSView", "true"));

        winHandle  = Ogre::StringConverter::toString(WindowContentViewHandle(wmInfo));
        break;*/
        return false;
#else
    case SDL_SYSWM_X11:
        winHandle = Ogre::StringConverter::toString((unsigned long)wmInfo.info.x11.window);
        break;
#endif
    default:
        //unexpected WM
        return false;
    }

    //TODO: externalWindowHandle is deprecated according to the source code. Figure out a way to get parentWindowHandle
    //      to work properly. On Linux/X11 it causes an occasional GLXBadDrawable error.
    params.insert(std::make_pair("externalWindowHandle",  winHandle));
    params["externalGLControl"] = Ogre::String("True");

    window_ = root_->createRenderWindow(Ogre::String(window->title()), window->size().x, window->size().y, window->fullscreen(), &params);
    return true;
}


} // namespace threed
} // namespace desktop
} // namespace ugdk
