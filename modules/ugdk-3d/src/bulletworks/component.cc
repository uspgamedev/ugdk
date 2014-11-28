
#include <bulletworks/component.h>

namespace bulletworks {

using std::weak_ptr;

void Component::Take(const weak_ptr<Object> &the_owner) {
    owner_ = the_owner;
    OnTake();
}

}
