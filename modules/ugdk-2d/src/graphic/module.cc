#include <ugdk/graphic/module.h>

namespace ugdk {
namespace graphic {

static std::unique_ptr<Manager> reference_;

bool Initialize(std::unique_ptr<Manager> manager, const std::weak_ptr<desktop::Window> & window,
                const math::Vector2D &canvas_size) {
    if(is_active() && manager->Initialize(window, canvas_size)) {
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

bool is_active() {
    return static_cast<bool>(reference_);
}

void Release() {
    if(is_active())
        reference_->Release();
    reference_.reset();
}

Manager& manager() {
    return *(reference_.get());
}

} // namespace graphic
} // namespace ugdk
