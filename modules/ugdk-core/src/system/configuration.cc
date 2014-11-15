#include <ugdk/system/configuration.h>

namespace ugdk {
namespace system {

Configuration::Configuration()
    : base_path("./")
    , default_language("en_US")

    , audio_enabled(true)

    , windows_list(1, desktop::WindowSettings())
    , canvas_size(800.0, 600.0)

    , input_enabled(true)

    , time_enabled(true)
    
    , ogre_enabled(false)
    {}

} // namespace system
} // namespace ugdk

