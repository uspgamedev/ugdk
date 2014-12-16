
#include <ugdk/action/3D/component.h>

namespace ugdk {
namespace action {
namespace mode3d {

using std::weak_ptr;

void Component::Take(const weak_ptr<Element> &the_owner) {
    owner_ = the_owner;
    OnTaken();
}

} // namespace 3D
} // namespace action
} // namespace ugdk
