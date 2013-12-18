#ifndef UGDK_DESKTOP_WINDOWSETTINGS_H_
#define UGDK_DESKTOP_WINDOWSETTINGS_H_

#include <string>
#include <ugdk/math/integer2D.h>

namespace ugdk {
namespace desktop {

struct WindowSettings {
    std::string title;
    std::string icon;
    math::Integer2D size;
    bool fullscreen;

    WindowSettings();

    WindowSettings(const std::string& _title,
                  const std::string& _icon,
                  const math::Integer2D& _size, 
                  bool _fullscreen)

        : title(_title)
        , icon(_icon)
        , size(_size)
        , fullscreen(_fullscreen)
        {}
};

}  // namespace desktop
}  // namespace ugdk

#endif // UGDK_DESKTOP_WINDOWSETTINGS_H_
