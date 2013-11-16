#include <ugdk/system/configuration.h>

namespace ugdk {
namespace system {

Configuration::Configuration()
    : base_path("./")
    , default_language("en_US")

    , audio_enabled(true)

    , desktop_enabled(true)

    , input_enabled(true)

    , time_enabled(true)
    {}

} // namespace system
} // namespace ugdk

