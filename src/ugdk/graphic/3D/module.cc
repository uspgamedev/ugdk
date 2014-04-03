#include <ugdk/graphic/3D/module.h>

namespace ugdk {
namespace graphic {
namespace threed {

static Manager* reference_ = nullptr;

bool Initialize(Manager* manager) {
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
    delete reference_;
    reference_ = nullptr;
}

Manager* manager() {
    return reference_;
}

} // namespace threed
} // namespace graphic
} // namespace ugdk
