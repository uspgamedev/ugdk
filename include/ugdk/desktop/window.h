
#ifndef UGDK_DESKTOP_WINDOW_H_
#define UGDK_DESKTOP_WINDOW_H_

#include <ugdk/math/integer2D.h>
#include <ugdk/desktop.h>
#include <ugdk/structure/types.h>

namespace ugdk {
namespace desktop {

class Window {
  public:
    virtual ~Window() {}

    /// Presents the current canvas in the screen.
    virtual void Present() = 0;

    /// Updates the settings and applies the changes.
    /** Warning: some fullscreen/resolution combinations may be unsupported by the display, resulting
        in unoperable windows. */
    virtual void ChangeSettings(const math::Integer2D& size, bool fullscreen, bool vsync) = 0;

    virtual uint32 id() const = 0;
    virtual const char* title() const = 0;
    virtual math::Integer2D size() const = 0;
    virtual bool fullscreen() const = 0;
    virtual bool vsync() const = 0;

  protected:
    Window() {}
};

}  // namespace desktop
}  // namespace ugdk

#endif // UGDK_DESKTOP_WINDOW_H_
