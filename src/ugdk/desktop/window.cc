#include <ugdk/desktop/window.h>

#include "SDL_video.h"

namespace ugdk {
namespace desktop {

Window::Window(SDL_Window* sdl_window, const std::string& title, const math::Integer2D& size, bool fullscreen)
    : sdl_window_(sdl_window)
    , title_(title)
    , size_(size)
    , fullscreen_(fullscreen)
    {}
    
Window::~Window() {
    SDL_DestroyWindow(sdl_window_);
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
