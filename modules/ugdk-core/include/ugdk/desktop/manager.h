
#ifndef UGDK_DESKTOP_MANAGER_H_
#define UGDK_DESKTOP_MANAGER_H_

#include <ugdk/desktop.h>
#include <ugdk/structure/types.h>
#include <ugdk/system.h>

#include <map>
#include <memory>

namespace ugdk {
namespace desktop {

class Manager {
  public:
    virtual ~Manager();

    bool Initialize();
    void Release();

    std::weak_ptr<Window> CreateWindow(const WindowSettings& settings);
    std::weak_ptr<Window> CreateWindow(unsigned long hwnd);
    // TODO: DestroyWindow

    void set_primary_window(const std::weak_ptr<Window>& window) {
        primary_window_ = window;
    }
    std::shared_ptr<Window> primary_window() const { return primary_window_.lock(); }
    std::shared_ptr<Window> window(uint32 index) const;

    virtual void PresentAll() = 0;

  protected:
    Manager();

    virtual std::shared_ptr<Window> DoCreateWindow(unsigned long hwnd) = 0;

    std::weak_ptr<Window> primary_window_;
    std::map< uint32, std::shared_ptr<Window> > windows_;

  private:
    std::weak_ptr<Window> RegisterAndGetWindow(const std::shared_ptr<Window>& new_window);
    std::unique_ptr<system::SDLEventHandler> sdlevent_handler_;

    friend class DesktopSDLEventHandler;
};

}  // namespace desktop
}  // namespace ugdk

#endif // UGDK_DESKTOP_MANAGER_H_
