#include <ugdk/filesystem/module.h>

namespace ugdk {
namespace filesystem {

static std::unique_ptr<Manager> reference_;

bool Initialize(std::unique_ptr<Manager> manager) {
    if(manager.get() && manager->Initialize()) {
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
    if(reference_)
        reference_->Release();
    reference_.reset();
}

filesystem::Manager& manager() {
    return *(reference_.get());
}

} // namespace filesystem
} // namespace ugdk
