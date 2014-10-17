#include <ugdk/graphic/module.h>

namespace ugdk {
namespace graphic {

static Manager* reference_ = nullptr;

bool Initialize(Manager* manager, const std::weak_ptr<desktop::Window> & window,
                const math::Vector2D &canvas_size) {
    if(manager && manager->Initialize(window, canvas_size)) {
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
    delete reference_;
    reference_ = nullptr;
}

Manager* manager() {
    return reference_;
}

} // namespace graphic
} // namespace ugdk
