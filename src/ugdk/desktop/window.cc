#include <ugdk/desktop/window.h>

#include "SDL_video.h"

#include <ugdk/graphic/canvas.h>
#include <ugdk/debug/profiler.h>

namespace ugdk {
namespace desktop {

Window::Window(SDL_Window* sdl_window)
    : sdl_window_(sdl_window)
{}
    
Window::~Window() {
    SDL_DestroyWindow(sdl_window_);
}

void Window::Present() {
    debug::ProfileSection section("Present window '" + std::string(this->title()) + "'");

    // Swap the buffers to show the backbuffer to the user.
    SDL_GL_SwapWindow(sdl_window_);
}

void Window::ChangeSettings(const math::Integer2D& size, bool fullscreen, bool vsync) {
    vsync_ = vsync;

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
    
    if(auto canvas = this->attached_canvas_.lock())
        canvas->UpdateViewport();
}
    
uint32 Window::id() const {
    return SDL_GetWindowID(sdl_window_);
}
    
const char* Window::title() const {
    return SDL_GetWindowTitle(sdl_window_);
}

math::Integer2D Window::size() const {
    math::Integer2D result;
    SDL_GetWindowSize(sdl_window_, &result.x, &result.y);
    return result;
}

bool Window::fullscreen() const {
    return SDL_GetWindowFlags(sdl_window_) & SDL_WINDOW_FULLSCREEN != 0;
}

}  // namespace desktop
}  // namespace ugdk
