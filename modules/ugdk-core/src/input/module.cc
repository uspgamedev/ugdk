#include <ugdk/input/module.h>

namespace ugdk {
namespace input {

static std::unique_ptr<Manager> reference_;

bool is_active() {
    return static_cast<bool>(reference_);
}

bool Initialize(std::unique_ptr<Manager> manager) {
    if(manager && manager->Initialize()) {
        // The manager initialized correctly, so we can use it.
        reference_ = manager;
        return true;
    } else {
        // Error initializing the manager, delete it and don't activate the module.
        delete manager;
        // TODO: log the error.
        return false;
    }
}

void Release() {
    if(reference_)
        reference_->Release();
    reference_.reset();
}

input::Manager& manager() {
    return *(reference_.get());
}

} // namespace input
} // namespace ugdk
