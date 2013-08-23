#include <ugdk/event/module.h>

namespace ugdk {
namespace event {

static Manager* reference_ = nullptr;

bool Initialize(Manager* manager) {
    if(manager) {
        // The manager initialized correctly, so we can use it.
        reference_ = manager;
        return true;
    } else {
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

} // namespace event
} // namespace ugdk
