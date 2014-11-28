
#include <bulletworks/component.h>

namespace bulletworks {

using std::weak_ptr;

void Component::Take(const weak_ptr<Object> &owner) {
    owner_ = owner;
    OnTake(owner);
}

}
