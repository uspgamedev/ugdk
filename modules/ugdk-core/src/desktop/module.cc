#include <ugdk/desktop/module.h>

namespace ugdk {
namespace desktop {

namespace {

static std::unique_ptr<Manager> reference_;

}

bool Initialize(std::unique_ptr<Manager> manager) {
    if(manager && manager->Initialize()) {
        // The manager initialized correctly, so we can use it.
        reference_ = std::move(manager);
        return true;
    } else {
        reference_.reset();
        // TODO: log the error.
        return false;
    }
}
bool is_active() {
    return static_cast<bool>(reference_);
}

void Release() {
    if(reference_)
        reference_->Release();
    reference_.reset();
}

desktop::Manager& manager() {
    return *(reference_.get());
}

} // namespace desktop
} // namespace ugdk
