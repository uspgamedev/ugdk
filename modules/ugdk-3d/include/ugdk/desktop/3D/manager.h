#ifndef UGDK_DESKTOP_3D_MANAGER_H_
#define UGDK_DESKTOP_3D_MANAGER_H_

#include <ugdk/desktop/manager.h>
#include <ugdk/desktop.h>

#include <string>
#include <vector>
#include <memory>

namespace Ogre {
class Root;
class StaticPluginLoader;
class RenderWindow;
}

namespace ugdk {
namespace desktop {
namespace mode3d {

class Window;

class Manager final : public desktop::Manager  {
public:
    Manager(const std::vector<std::string>& ogre_plugins);
    ~Manager();
    
    bool Initialize() override;
    void Release() override;
    
    void PresentAll() override;

    void LoadPlugin(const std::string& plugin_name);
    
    Ogre::Root* root() { return root_; }
    Ogre::RenderWindow& window();

protected:
    std::shared_ptr<desktop::Window> DoCreateWindow(const WindowSettings& settings) override; /**/
    std::shared_ptr<desktop::Window> DoCreateWindow(unsigned long hwnd) override;             /**/
    
private:
    Ogre::Root* root_;
    Ogre::StaticPluginLoader* static_loader_;
    std::shared_ptr<Window> window_;
    std::vector<std::string> ogre_plugins_;
};

} // namespace mode3d
} // namespace desktop
} // namespace ugdk
#endif
