#ifndef UGDK_SYSTEM_CONFIGURATION_H_
#define UGDK_SYSTEM_CONFIGURATION_H_

#include <string>
#include <ugdk/math/integer2D.h>

namespace ugdk {
namespace system {

typedef struct Configuration {
    std::string        base_path;
    std::string default_language;
    
    bool audio_enabled;
    
    bool graphic_enabled;
    std::string      window_icon;
    std::string      window_title;
    math::Integer2D  window_resolution;
    bool             fullscreen;

    bool input_enabled;

    bool time_enabled;

    Configuration() :
        base_path("./"),
        default_language("en_US"),

        audio_enabled(true),

        graphic_enabled(true),
        window_icon(""),
        window_title("UGDK Game"),
        window_resolution(800, 600),
        fullscreen(false),

        input_enabled(true),

        time_enabled(true)
    {}
} Configuration;

} // namespace system
} // namespace ugdk

#endif

