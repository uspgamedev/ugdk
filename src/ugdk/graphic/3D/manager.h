#ifndef UGDK_GRAPHIC_3D_MANAGER_H_
#define UGDK_GRAPHIC_3D_MANAGER_H_

#include <string>

namespace Ogre {
class Root;
class RenderWindow;
class StaticPluginLoader;
}

namespace ugdk {
namespace graphic {
namespace threed {


class Manager {
public:
	Manager(const std::string& window_title);
	~Manager();

    bool Initialize();

    void PresentAll(double dt);
    
    Ogre::Root* root() { return root_; }
    Ogre::RenderWindow* window() { return window_; }

private:
    std::string window_title_;
    Ogre::Root* root_;
    Ogre::StaticPluginLoader* static_loader_;
    Ogre::RenderWindow* window_;
};

} // namespace threed
} // namespace graphic
} // namespace ugdk
#endif
