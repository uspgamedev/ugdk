#ifndef UGDK_SYSTEM_CONFIGURATION_H_
#define UGDK_SYSTEM_CONFIGURATION_H_

#include <string>
#include <ugdk/math/integer2D.h>
#include <ugdk/window.h>

namespace ugdk {
namespace system {

struct Configuration {
    std::string        base_path;
    std::string default_language;
    
    bool audio_enabled;
    
    window::WindowSettings* window_configuration;

    bool input_enabled;

    bool time_enabled;

    Configuration();
    ~Configuration();
};

} // namespace system
} // namespace ugdk

#endif

