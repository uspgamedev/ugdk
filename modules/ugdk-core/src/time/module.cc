#include <ugdk/time/module.h>
#include <memory>

namespace ugdk {
namespace time {

static std::unique_ptr<Manager> reference_;

bool Initialize(std::unique_ptr<Manager> manager) {
    if(manager) {
        // The manager initialized correctly, so we can use it.
        reference_ = std::move(manager);
        return true;
    } else {
        // Error initializing the manager, delete it and don't activate the module.
        manager.reset();
        // TODO: log the error.
        return false;
    }
}

void Release() {
    reference_.reset();
}

time::Manager& manager() {
    return *(reference_.get());
}

} // namespace audio
} // namespace ugdk
