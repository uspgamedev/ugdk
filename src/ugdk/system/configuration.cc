#include <ugdk/system/configuration.h>

namespace ugdk {
namespace system {

Configuration::Configuration()
    : base_path("./")
    , default_language("en_US")

    , audio_enabled(true)

    , windows_list(1, desktop::WindowSettings())
    , canvas_size(1024.0, 768.0)

    , input_enabled(true)

    , time_enabled(true)
    {}

} // namespace system
} // namespace ugdk

