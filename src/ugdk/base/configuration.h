#ifndef UGDK_BASE_CONFIGURATION_H_
#define UGDK_BASE_CONFIGURATION_H_

#include <string>
#include <ugdk/math/vector2D.h>

namespace ugdk {

typedef struct Configuration {
    std::string     base_path;
    std::string   window_icon;
    std::string  window_title;
    Vector2D      window_size;
    bool           fullscreen;

    Configuration() :
        base_path("./"),
        window_icon(""),
        window_title("UGDK Game"),
        window_size(800.0, 600.0),
        fullscreen(false) {}
} Configuration;

} // namespace ugdk

#endif

