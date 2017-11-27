
#ifndef UGDK_DESKTOP_MANAGER_H_
#define UGDK_DESKTOP_MANAGER_H_

#include <ugdk/desktop.h>
#include <ugdk/structure/types.h>
#include <ugdk/system.h>

#include <vector>
#include <unordered_set>
#include <memory>

namespace ugdk {
namespace desktop {

class Manager {
  public:
    Manager();
    ~Manager();

    bool Initialize();
    void Release();

    uint32_t num_windows();

    std::weak_ptr<Window> CreateWindow(const WindowSettings& settings);
    void DestroyWindow(const std::weak_ptr<Window>&);

    std::weak_ptr<Window> primary_window() const { return primary_window_; }
    void set_primary_window(const std::weak_ptr<Window>& window) {
        primary_window_ = window;
    }

    void PresentAll();
      
  private:
    std::weak_ptr<Window> RegisterAndGetWindow(const std::shared_ptr<Window>& new_window);
    std::unique_ptr<system::SDLEventHandler> sdlevent_handler_;

    std::weak_ptr<Window> primary_window_;
    std::unordered_set<std::shared_ptr<Window>> windows_;

    friend class DesktopSDLEventHandler;
};

}  // namespace desktop
}  // namespace ugdk

#endif // UGDK_DESKTOP_MANAGER_H_
