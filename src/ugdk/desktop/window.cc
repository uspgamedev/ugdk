#include <ugdk/desktop/window.h>

#include "SDL_video.h"

#include <ugdk/graphic/canvas.h>
#include <ugdk/debug/profiler.h>

namespace ugdk {
namespace desktop {

Window::Window(SDL_Window* sdl_window)
    : sdl_window_(sdl_window)
    , title_(SDL_GetWindowTitle(sdl_window))
    , fullscreen_((SDL_GetWindowFlags(sdl_window) & SDL_WINDOW_FULLSCREEN) != 0)
{
    SDL_GetWindowSize(sdl_window, &size_.x, &size_.y);
}
    
Window::~Window() {
    SDL_DestroyWindow(sdl_window_);
}

void Window::Present() {
    debug::ProfileSection section("Present window '" + title_ + "'");

    // Swap the buffers to show the backbuffer to the user.
    SDL_GL_SwapWindow(sdl_window_);
}

void Window::ChangeSettings(const math::Integer2D& size, bool fullscreen) {
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
    
    // Update the stored size from the actual window and not the requested size.
    SDL_GetWindowSize(sdl_window_, &size_.x, &size_.y);

    if(auto canvas = this->attached_canvas_.lock())
        canvas->UpdateViewport();
}

}  // namespace desktop
}  // namespace ugdk
