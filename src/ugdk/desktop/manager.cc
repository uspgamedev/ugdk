#include <ugdk/desktop/manager.h>

#include <cstdio>
#include <string>

#include "SDL.h"
#include "SDL_image.h"

#include <ugdk/desktop/window.h>
#include <ugdk/desktop/windowsettings.h>
#include <ugdk/system/engine.h>

namespace ugdk {
namespace desktop {

namespace {
bool errlog(const std::string& msg) {
    fprintf(stderr, "ugdk::desktop::Manager Error Log - %s\n", msg.c_str());
    return false;
}
}

Manager::Manager() {}
    
Manager::~Manager() {
}

bool Manager::Initialize() {
    if(SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
        return errlog("Failed to initialize SDL_VIDEO: " + std::string(SDL_GetError()));

    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
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
        // TODO: Log the error
        fprintf(stderr, "Failed to create the window: %s\n", SDL_GetError());
        return std::weak_ptr<Window>();
    }

    if(settings.icon.length() > 0) {
        SDL_Surface* icon = IMG_Load(system::ResolvePath(settings.icon).c_str());
        SDL_SetWindowIcon(window, icon);
        SDL_FreeSurface(icon);
    }

    windows_.emplace_back(new Window(window, settings.title, settings.size, settings.fullscreen));
    if(!primary_window_.lock())
        primary_window_ = windows_.back();

    return windows_.back();
}

}  // namespace desktop
}  // namespace ugdk
