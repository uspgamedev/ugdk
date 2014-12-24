#ifndef UGDK_SYSTEM_CONFIGURATION_H_
#define UGDK_SYSTEM_CONFIGURATION_H_

#include <ugdk/math/integer2D.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/desktop/windowsettings.h>
#include <string>
#include <vector>

namespace ugdk {
namespace system {

struct Configuration {
    std::string        base_path;
    std::string default_language;
    
    bool audio_enabled;

    std::vector<desktop::WindowSettings> windows_list;
    math::Vector2D canvas_size;
    std::vector<std::string> ogre_plugins;
    std::string ogre_renderer;

    bool input_enabled;

    bool time_enabled;

    Configuration();
};

} // namespace system
} // namespace ugdk

#endif

