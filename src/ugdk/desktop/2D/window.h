#ifndef UGDK_DESKTOP_2D_WINDOW_H_
#define UGDK_DESKTOP_2D_WINDOW_H_

#include <ugdk/math/integer2D.h>
#include <ugdk/desktop.h>
#include <ugdk/graphic.h>
#include <ugdk/structure/types.h>

struct SDL_Window;

namespace ugdk {
namespace desktop {
namespace mode2d {

class Window final : public desktop::Window {
  private:
    Window(SDL_Window* sdl_window);

  public:
    ~Window();

    /// Presents the current canvas in the screen.
    void Present() override;

    /// Updates the settings and applies the changes.
    /** Warning: some fullscreen/resolution combinations may be unsupported by the display, resulting
        in unoperable windows. */
    void ChangeSettings(const math::Integer2D& size, bool fullscreen, bool vsync) override;

    uint32 id() const override;
    const char* title() const override;
    math::Integer2D size() const override;
    bool fullscreen() const override;
    bool vsync() const override { return vsync_; }

  private:
    SDL_Window* sdl_window_;
    bool vsync_;

    friend class ::ugdk::desktop::mode2d::Manager;
};

)  // namespace mode2d
}  // namespace desktop
}  // namespace ugdk

#endif // UGDK_DESKTOP_2D_WINDOW_H_
