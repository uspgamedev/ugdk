#ifndef UGDK_DESKTOP_3D_MANAGER_H_
#define UGDK_DESKTOP_3D_MANAGER_H_

#include <string>
#include <memory>

namespace Ogre {
class Root;
class RenderWindow;
class StaticPluginLoader;
}

namespace ugdk {
namespace desktop {

class Window;

namespace threed {

class Manager {
public:
	Manager();
	~Manager();

    bool Initialize();
    
    bool AddWindow(const std::shared_ptr<Window>& window);

    void PresentAll(double dt);
    
    Ogre::Root* root() { return root_; }
    Ogre::RenderWindow* window() { return window_; }

private:
    Ogre::Root* root_;
    Ogre::StaticPluginLoader* static_loader_;
    Ogre::RenderWindow* window_;
    
    bool createWindowFromSDL(const std::shared_ptr<Window>& window);
};

} // namespace threed
} // namespace desktop
} // namespace ugdk
#endif
