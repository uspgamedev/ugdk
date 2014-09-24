
#include <ugdk/desktop/2D/manager.h>

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
namespace mode2d {

using std::shared_ptr;

Manager::Manager() {}

Manager::~Manager() {}

class DesktopSDLEventHandler : public internal::SDLEventHandler {
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

bool Manager::Initialize() {
    if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
        debug::Log(debug::LogLevel::ERROR, "UGDK",
                   "desktop::Manager - Failed to initialize SDL_VIDEO: ", SDL_GetError());
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,   8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,  8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    sdlevent_handler_.reset(new DesktopSDLEventHandler(*this));
    system::RegisterSDLHandler(sdlevent_handler_.get());
    return true;
}

void Manager::Release() {
    system::DeregisterSDLHandler(sdlevent_handler_.get());
    windows_.clear();
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}
    
std::shared_ptr<Window> Manager::DoCreateWindow(const WindowSettings& settings) {

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
        return std::shared_ptr<Window>();
    }

    if(settings.icon.length() > 0) {
        SDL_Surface* icon = IMG_Load(system::ResolvePath(settings.icon).c_str());
        SDL_SetWindowIcon(window, icon);
        SDL_FreeSurface(icon);
    }

    return shared_ptr<Window>(new Window(window));
}
std::shared_ptr<Window> Manager::DoCreateWindow(unsigned long hwnd) {
    SDL_Window* window = SDL_CreateWindowFrom((void*)hwnd);
    if(!window) {
        // Couldn't create the window.
        // TODO: Log the error
        fprintf(stderr, "Failed to create the window: %s\n", SDL_GetError());
        return std::shared_ptr<Window>();
    }
    return shared_ptr<Window>(new Window(window));
}
    
void Manager::PresentAll() {
    for(const auto& window : windows_)
        window.second->Present();
}

}  // namespace mode2d
}  // namespace desktop
}  // namespace ugdk
