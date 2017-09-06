
#include <ugdk/debug/log.h>
#include <ugdk/desktop/manager.h>
#include <ugdk/desktop/window.h>
#include <ugdk/system/engine.h>

#include <system/sdleventhandler.h>

namespace ugdk {
namespace desktop {

using std::weak_ptr;
using std::shared_ptr;

class DesktopSDLEventHandler : public system::SDLEventHandler {
public:
    DesktopSDLEventHandler(Manager& manager) : manager_(manager) {}

    std::unordered_set<Uint32> TypesHandled() const override {
        return { SDL_WINDOWEVENT };
    }

    void Handle(const ::SDL_Event& sdlevent) const override {
        switch (sdlevent.window.event) {
            case SDL_WINDOWEVENT_RESIZED:
                // no-op
                break;

            case SDL_WINDOWEVENT_CLOSE:
                manager_.windows_.erase(sdlevent.window.windowID);
                break;

            default:
                break; // ignore
        }
    }

private:
    void operator=(const DesktopSDLEventHandler&);
    Manager& manager_;
};

Manager::Manager() {}
Manager::~Manager() {}

bool Manager::Initialize() {
    sdlevent_handler_.reset(new DesktopSDLEventHandler(*this));
    system::RegisterSDLHandler(sdlevent_handler_.get());
    return true;
}

void Manager::Release() {
    system::DeregisterSDLHandler(sdlevent_handler_.get());
    sdlevent_handler_.reset();
    windows_.clear();
}

weak_ptr<Window> Manager::CreateWindow(const WindowSettings& settings) {
    Uint32 flags = SDL_WINDOW_OPENGL;
    if(settings.fullscreen) flags |= SDL_WINDOW_FULLSCREEN;

    SDL_Window* window  = SDL_CreateWindow(settings.title.c_str(),
                                           SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                           settings.size.x, settings.size.y,
                                           flags);
    if(!window) {
       // Couldn't create the window.
       debug::Log(debug::LogLevel::ERROR, "UGDK",
                  "desktop::Manager - Failed to create the window: ", SDL_GetError());
        return std::shared_ptr<desktop::Window>();
    }
    return RegisterAndGetWindow(std::shared_ptr<desktop::Window>(new desktop::Window(window)));
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

void Manager::PresentAll() {
    for(const auto& window : windows_)
        window.second->Present();
}

}  // namespace desktop
}  // namespace ugdk
