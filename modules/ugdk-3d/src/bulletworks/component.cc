
#include <bulletworks/component.h>

namespace bulletworks {

using std::weak_ptr;
using ugdk::action::mode3d::Element;

void Component::Take(const weak_ptr<Element> &the_owner) {
    owner_ = the_owner;
    OnTaken();
}

}
