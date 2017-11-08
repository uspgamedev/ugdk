#ifndef UGDK_DESKTOP_WINDOWSETTINGS_H_
#define UGDK_DESKTOP_WINDOWSETTINGS_H_

#include <string>
#include <ugdk/math/integer2D.h>
#include <ugdk/math/vector2D.h>

namespace ugdk {
namespace desktop {

struct WindowSettings {
    std::string title;
    std::string icon;
    math::Integer2D size;
    math::Vector2D canvas_size;
    bool fullscreen;
    bool vsync;
    WindowSettings();

    WindowSettings(const std::string& _title,
                  const std::string& _icon,
                  const math::Integer2D& _size,
                  const math::Vector2D& _c_size, 
                  bool _fullscreen,
                  bool _vsync)

        : title(_title)
        , icon(_icon)
        , size(_size)
        , canvas_size(_c_size)
        , fullscreen(_fullscreen)
        , vsync(_vsync)
        {}
};

}  // namespace desktop
}  // namespace ugdk

#endif // UGDK_DESKTOP_WINDOWSETTINGS_H_
