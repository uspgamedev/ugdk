
#ifndef UGDK_DESKTOP_MANAGER_H_
#define UGDK_DESKTOP_MANAGER_H_

#include <ugdk/desktop.h>
#include <ugdk/structure/types.h>
#include <ugdk/system.h>

#include <vector>
#include <memory>

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

    void PresentAll();

  private:
    std::weak_ptr<Window> RegisterAndGetWindow(const std::shared_ptr<Window>& new_window);
    std::unique_ptr<system::SDLEventHandler> sdlevent_handler_;

    std::weak_ptr<Window> primary_window_;
    std::vector< std::shared_ptr<Window> > windows_;

    friend class DesktopSDLEventHandler;
};

}  // namespace desktop
}  // namespace ugdk

#endif // UGDK_DESKTOP_MANAGER_H_
