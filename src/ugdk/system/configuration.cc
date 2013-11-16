#include <ugdk/system/configuration.h>

namespace ugdk {
namespace system {

Configuration::Configuration()
    : base_path("./")
    , default_language("en_US")

    , audio_enabled(true)

    , window_configuration(nullptr)

    , input_enabled(true)

    , time_enabled(true)
    {}

Configuration::~Configuration() {
    delete window_configuration;
}

} // namespace system
} // namespace ugdk

