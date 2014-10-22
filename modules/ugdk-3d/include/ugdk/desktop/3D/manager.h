#ifndef UGDK_DESKTOP_3D_MANAGER_H_
#define UGDK_DESKTOP_3D_MANAGER_H_

#include <ugdk/desktop/manager.h>
#include <ugdk/desktop.h>

#include <string>
#include <memory>

namespace Ogre {
class Root;
class StaticPluginLoader;
class RenderWindow;
}

namespace ugdk {
namespace desktop {
namespace mode3d {

class Manager final : public desktop::Manager  {
public:
	Manager();
	~Manager();
	
	bool Initialize() override;
    void Release() override;
    
    void PresentAll() override;
    
    Ogre::Root* root() { return root_; }
    Ogre::RenderWindow* window() { return window_; }

protected:
    std::shared_ptr<desktop::Window> DoCreateWindow(const WindowSettings& settings) override; /**/
    std::shared_ptr<desktop::Window> DoCreateWindow(unsigned long hwnd) override;             /**/
    
private:
    Ogre::Root* root_;
    Ogre::StaticPluginLoader* static_loader_;
    Ogre::RenderWindow* window_;
};

} // namespace mode3d
} // namespace desktop
} // namespace ugdk
#endif
