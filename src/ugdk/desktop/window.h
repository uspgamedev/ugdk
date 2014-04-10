#ifndef UGDK_DESKTOP_WINDOW_H_
#define UGDK_DESKTOP_WINDOW_H_

#include <ugdk/math/integer2D.h>
#include <ugdk/desktop.h>
#include <ugdk/graphic.h>
#include <ugdk/structure/types.h>

struct SDL_Window;

namespace ugdk {
namespace desktop {

class Window {
  private:
    Window(SDL_Window* sdl_window);

  public:
    ~Window();

    /// Presents the current canvas in the screen.
    void Present();

    /// Updates the settings and applies the changes.
    /** Warning: some fullscreen/resolution combinations may be unsupported by the display, resulting
        in unoperable windows. */
    void ChangeSettings(const math::Integer2D& size, bool fullscreen, bool vsync);

    uint32 id() const;
    const char* title() const;
    math::Integer2D size() const;
    bool fullscreen() const;
    bool vsync() const { return vsync_; }

  private:
    SDL_Window* sdl_window_;
    bool vsync_;

    friend class ::ugdk::desktop::Manager;
    friend class ::ugdk::graphic::Manager;
#ifdef UGDK_3D_ENABLED
    friend class ::ugdk::desktop::threed::Manager;
#endif
};

}  // namespace desktop
}  // namespace ugdk

#endif // UGDK_DESKTOP_WINDOW_H_
