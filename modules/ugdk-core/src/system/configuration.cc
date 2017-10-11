#include <ugdk/system/configuration.h>

namespace ugdk {
namespace system {

Configuration::Configuration()
    : base_path("./")
    , default_language("en_US")

    , audio_enabled(true)

    , windows_list(1, desktop::WindowSettings())
    , input_enabled(true)

    , time_enabled(true)

    , limit_frame_time_with_sleep(false)
    , target_frame_time(1000 / 60)
    {}

} // namespace system
} // namespace ugdk

