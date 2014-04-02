#ifndef UGDK_GRAPHIC_3D_MANAGER_H_
#define UGDK_GRAPHIC_3D_MANAGER_H_

#include <OgreStaticPluginLoader.h>

#include <string>

namespace Ogre {
class Root;
class RenderWindow;
}

namespace ugdk {
namespace graphic {
namespace 3D {


class Manager {
public:
	Manager(const std::string& window_title);
	~Manager();

    bool Initialize();


private:
    std::string window_title_;
    Ogre::Root root_;
    Ogre::StaticPluginLoader static_loader_;
    Ogre::RenderWindow* window_;
};

} // namespace 3D
} // namespace graphic
} // namespace ugdk
#endif
