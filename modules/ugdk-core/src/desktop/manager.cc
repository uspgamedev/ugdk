
#include <ugdk/debug/log.h>
#include <ugdk/desktop/manager.h>
#include <ugdk/desktop/window.h>
#include <ugdk/system/engine.h>
#include <ugdk/system/exceptions.h>

#include <system/sdleventhandler.h>
#include <cassert>

namespace ugdk {
namespace desktop {

using std::weak_ptr;
using std::shared_ptr;
using std::vector;
using std::unordered_set;

class DesktopSDLEventHandler : public system::SDLEventHandler {
public:
    DesktopSDLEventHandler(Manager& manager) : manager_(manager) {}

    unordered_set<Uint32> TypesHandled() const override {
        return { SDL_WINDOWEVENT };
    }

    void Handle(const ::SDL_Event& sdlevent) const override {
        switch (sdlevent.window.event) {
            case SDL_WINDOWEVENT_RESIZED:
                // no-op
                break;

            case SDL_WINDOWEVENT_CLOSE:
                for (auto& window : manager_.windows_)
                    if (window->id() == sdlevent.window.windowID) {
                        manager_.DestroyWindow(window);
                        break;
                    }
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
    system::UnregisterSDLHandler(sdlevent_handler_.get());
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
        return shared_ptr<desktop::Window>();
    }

    return RegisterAndGetWindow(shared_ptr<desktop::Window>(new desktop::Window(window)));
}

void Manager::DestroyWindow(const weak_ptr<Window>& window) {
    auto window_ptr = window.lock();
    if (window_ptr)
        windows_.erase(window_ptr);
}

weak_ptr<Window> Manager::RegisterAndGetWindow(const shared_ptr<Window>& new_window) {
    assert(new_window); //Can't register non-existent window.

    windows_.insert(new_window);

    if(!primary_window_.lock()) //If we don't have a prim. win. then set it
        primary_window_ = new_window;
   
    return new_window;
}

uint32_t Manager::num_windows() {
    return windows_.size();
}
void Manager::PresentAll() {
    for(const auto& window : windows_)
        window->Present();
}

}  // namespace desktop
}  // namespace ugdk
