
#ifndef UGDK_DESKTOP_WINDOW_H_
#define UGDK_DESKTOP_WINDOW_H_

#include <ugdk/desktop.h>
#include <ugdk/graphic/manager.h>
#include <ugdk/desktop/manager.h>
#include <ugdk/math/integer2D.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/structure/types.h>

struct SDL_Window;

namespace ugdk {
namespace desktop {

class Window {
  public:
    ~Window();

    /// Presents the current canvas in the screen.
    void Present();

    /// Updates the settings and applies the changes.
    /** Warning: some fullscreen/resolution combinations may be unsupported by the display, resulting
        in unoperable windows. */
    virtual void ChangeSettings(const math::Integer2D& size, const math::Integer2D& canvas_size, 
                                bool fullscreen, bool vsync);

    virtual uint32 id() const;
    virtual const char* title() const;
    virtual math::Integer2D size() const;
    virtual math::Vector2D canvas_size() const;
    virtual bool fullscreen() const;
    bool vsync() const { return vsync_; }

  protected:
    friend class ::ugdk::graphic::Manager;
    friend class ::ugdk::desktop::Manager;
    Window(SDL_Window* sdl_window);

  private:
    SDL_Window* sdl_window_;
    bool vsync_;
};

}  // namespace desktop
}  // namespace ugdk

#endif // UGDK_DESKTOP_WINDOW_H_
