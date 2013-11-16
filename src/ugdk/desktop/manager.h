#ifndef UGDK_DESKTOP_MANAGER_H_
#define UGDK_DESKTOP_MANAGER_H_

#include <ugdk/desktop.h>
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
    std::weak_ptr<Window> primary_window() const { return primary_window_; }
    std::weak_ptr<Window> window(size_t index) const { return windows_[index]; }

    void PresentAll();

  private:
    std::weak_ptr<Window> primary_window_;
    std::vector<std::shared_ptr<Window>> windows_;
};

}  // namespace desktop
}  // namespace ugdk

#endif // UGDK_DESKTOP_MANAGER_H_
