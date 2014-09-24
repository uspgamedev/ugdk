
#include <ugdk/desktop/manager.h>
#include <ugdk/desktop/window.h>

namespace ugdk {
namespace desktop {

using std::weak_ptr;
using std::shared_ptr;

weak_ptr<Window> Manager::CreateWindow(const WindowSettings& settings) {
    return RegisterAndGetWindow(this->DoCreateWindow(settings));
}

std::weak_ptr<Window> Manager::CreateWindow(unsigned long hwnd) {
    return RegisterAndGetWindow(this->DoCreateWindow(hwnd));
}

std::weak_ptr<Window> Manager::RegisterAndGetWindow(const shared_ptr<Window>& new_window) {
    windows_[new_window->id()] = new_window;

    if(!primary_window_.lock())
        primary_window_ = new_window;

    return new_window;
}

std::shared_ptr<Window> Manager::window(uint32 index) const {
    auto it = windows_.find(index);
    if (it == windows_.end())
        return nullptr;
    return it->second;
}

}  // namespace desktop
}  // namespace ugdk

