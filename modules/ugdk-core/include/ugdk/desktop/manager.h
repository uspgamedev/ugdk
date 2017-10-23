
#ifndef UGDK_DESKTOP_MANAGER_H_
#define UGDK_DESKTOP_MANAGER_H_

#include <ugdk/desktop.h>
#include <ugdk/structure/types.h>
#include <ugdk/system.h>

#include <vector>
#include <unordered_map>
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
    std::weak_ptr<Window> primary_window() const { return primary_window_; }
    std::weak_ptr<Window> window(uint32 index) const;

    void PresentAll();
    uint32_t MapIdToIndex(uint32_t id);
    std::weak_ptr<Window> WindowById(uint32_t id);      
    uint32_t num_windows();
      
  private:
    std::weak_ptr<Window> RegisterAndGetWindow(const std::shared_ptr<Window>& new_window);
    std::unique_ptr<system::SDLEventHandler> sdlevent_handler_;

    std::weak_ptr<Window> primary_window_;
    std::vector< std::shared_ptr<Window> > windows_;
    std::unordered_map<uint32_t, uint32_t> map_;

    friend class DesktopSDLEventHandler;
};

}  // namespace desktop
}  // namespace ugdk

#endif // UGDK_DESKTOP_MANAGER_H_
