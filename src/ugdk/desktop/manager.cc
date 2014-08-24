#include <ugdk/desktop/manager.h>


#include "SDL.h"
#include "SDL_image.h"

#include <ugdk/desktop/window.h>
#include <ugdk/desktop/windowsettings.h>
#include <ugdk/debug/log.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/system/engine.h>
#include <ugdk/internal/sdleventhandler.h>

#include <string>

namespace ugdk {
namespace desktop {

Manager::Manager() {}
    
Manager::~Manager() {}

class DesktopSDLEventHandler : public internal::SDLEventHandler {
public:
    DesktopSDLEventHandler(Manager& manager) : manager_(manager) {}

    bool CanHandle(const ::SDL_Event& sdlevent) const {
        return sdlevent.type == SDL_WINDOWEVENT;
    }

    void Handle(const ::SDL_Event& sdlevent) const {
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

bool Manager::Initialize() {
    if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
        debug::Log(debug::LogLevel::ERROR, "UGDK",
                   "desktop::Manager - Failed to initialize SDL_VIDEO: ", SDL_GetError());
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    sdlevent_handler_.reset(new DesktopSDLEventHandler(*this));
    return true;
}

void Manager::Release() {
    windows_.clear();
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}
    
std::weak_ptr<Window> Manager::CreateWindow(const WindowSettings& settings) {

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
        return std::weak_ptr<Window>();
    }

    if(settings.icon.length() > 0) {
        SDL_Surface* icon = IMG_Load(system::ResolvePath(settings.icon).c_str());
        SDL_SetWindowIcon(window, icon);
        SDL_FreeSurface(icon);
    }

    std::shared_ptr<Window> new_window(new Window(window));

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
