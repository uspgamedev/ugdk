#ifndef UGDK_DESKTOP_MANAGER_H_
#define UGDK_DESKTOP_MANAGER_H_

#include <ugdk/internal.h>
#include <ugdk/desktop/windowsettings.h>
#include <ugdk/desktop.h>
#include <ugdk/structure/types.h>

#include <vector>
#include <memory>
#include <map>

namespace ugdk {
namespace desktop {

class Manager {
  public:
    Manager();
    ~Manager();

    bool Initialize();
    void Release();

    std::weak_ptr<Window> CreateWindow(const WindowSettings& settings);
    // TODO: DestroyWindow

    void set_primary_window(const std::weak_ptr<Window>& window) {
        primary_window_ = window;
    }
    std::shared_ptr<Window> primary_window() const { return primary_window_.lock(); }
    std::shared_ptr<Window> window(uint32 index) const;
    
    const internal::SDLEventHandler* sdlevent_handler() { return sdlevent_handler_.get(); }

    void PresentAll();

  private:
    std::unique_ptr<internal::SDLEventHandler> sdlevent_handler_;
    std::weak_ptr<Window> primary_window_;
    std::map< uint32, std::shared_ptr<Window> > windows_;
    
    friend class DesktopSDLEventHandler;
};

}  // namespace desktop
}  // namespace ugdk

#endif // UGDK_DESKTOP_MANAGER_H_
