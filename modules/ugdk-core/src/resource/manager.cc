#include <ugdk/resource/manager.h>

namespace ugdk {
namespace resource {

Manager::Manager() {}

Manager::~Manager() {
    for (const auto& func : clear_functions_)
        func();
}

} // namespace resource
} // namespace ugdk
