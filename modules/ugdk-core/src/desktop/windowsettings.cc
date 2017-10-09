#include <ugdk/desktop/windowsettings.h>

namespace ugdk {
namespace desktop {

WindowSettings::WindowSettings()
    : title("UGDK Game")
    , icon()
    , size(800, 600)
    , canvas_size(800.0, 600.0)
    , fullscreen(false)
    , vsync(false)
{}

}  // namespace desktop
}  // namespace ugdk
