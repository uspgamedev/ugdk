#include <ugdk/desktop/window.h>

#include <cmath>
#include <cassert>

#include "SDL.h"
#include "SDL_image.h"

#include <ugdk/action/scene.h>
#include <ugdk/system/engine.h>


namespace ugdk {
namespace desktop {

namespace {
bool errlog(const std::string& msg) {
    fprintf(stderr, "ugdk::desktop::Manager Error Log - %s\n", msg.c_str());
    return false;
}
}

WindowSettings::WindowSettings()
    : title("UGDK Game")
    , icon()
    , size(800, 600)
    , fullscreen(false)
{}

Window::Window(SDL_Window* sdl_window, const std::string& title, const math::Integer2D& size, bool fullscreen)
    : sdl_window_(sdl_window)
    , title_(title)
    , size_(size)
    , fullscreen_(fullscreen)
    {}
    
Window::~Window() {
    SDL_DestroyWindow(sdl_window_);
}

Window* Window::Create(const WindowSettings& settings) {

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
        return nullptr;
    }

    if(settings.icon.length() > 0) {
        SDL_Surface* icon = IMG_Load(system::ResolvePath(settings.icon).c_str());
        SDL_SetWindowIcon(window, icon);
        SDL_FreeSurface(icon);
    }

    return new Window(window, settings.title, settings.size, settings.fullscreen);
}

void Window::Present() {
    // Swap the buffers to show the backbuffer to the user.
    SDL_GL_SwapWindow(sdl_window_);
}

void Window::ChangeSettings(const math::Integer2D& size, bool fullscreen) {
    size_ = size;
    fullscreen_ = fullscreen;

    // SDL_SetWindowSize fails silently when the window is fullscreen.
    // So, first remove fullscreen.
    SDL_SetWindowFullscreen(sdl_window_, 0);

    // Now resize the window.
    SDL_SetWindowSize(sdl_window_, size.x, size.y);

    // And set it to fullscreen again, if needed.
    // This may leave the window in a broken state if the display doesn't support
    // the requested resolution, but SDL_SetWindowFullscreen reports no errors in such cases.
    if(fullscreen)
        SDL_SetWindowFullscreen(sdl_window_, SDL_WINDOW_FULLSCREEN);
}

}  // namespace desktop
}  // namespace ugdk
