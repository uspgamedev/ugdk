
#ifndef UGDK_DESKTOP_2D_MANAGER_H_
#define UGDK_DESKTOP_2D_MANAGER_H_

#include <ugdk/desktop/windowsettings.h>
#include <ugdk/desktop/manager.h>
#include <ugdk/desktop.h>
#include <ugdk/internal.h>
#include <ugdk/structure/types.h>

#include <map>
#include <memory>
#include <vector>

namespace ugdk {
namespace desktop {
namespace mode2d {

class Manager final : public desktop::Manager {
  public:
    Manager();
    ~Manager();

    bool Initialize() override;
    void Release() override;
    
    void PresentAll() override;

  protected:
    std::shared_ptr<desktop::Window> DoCreateWindow(const WindowSettings& settings) override;
    std::shared_ptr<desktop::Window> DoCreateWindow(unsigned long hwnd) override;

  private:
    std::unique_ptr<internal::SDLEventHandler> sdlevent_handler_;
    
    friend class DesktopSDLEventHandler;
};

} // namespace mode2d
} // namespace desktop
} // namespace ugdk

#endif // UGDK_DESKTOP_MANAGER_H_
