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

    std::weak_ptr<Window> primary_window() const { return primary_window_; }

  private:
    std::weak_ptr<Window> primary_window_;
    std::vector<std::shared_ptr<Window>> windows_;
};

}  // namespace desktop
}  // namespace ugdk

#endif // UGDK_DESKTOP_MANAGER_H_
