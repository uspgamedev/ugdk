
#ifndef BULLETWORKS_COMPONENT_H
#define BULLETWORKS_COMPONENT_H

#include <memory>
#include <typeindex>

namespace ugdk {
namespace action {
namespace mode3d {
class Element;
}
}
}

namespace bulletworks {

class Component {
  public:
    virtual ~Component() {}
    virtual std::type_index type() const = 0;
  protected:
    Component() {}
    /// TODO: Make this not accessible from Object
    virtual void OnTaken() = 0;
    void Take(const std::weak_ptr<ugdk::action::mode3d::Element> &the_owner);
    std::shared_ptr<ugdk::action::mode3d::Element> owner() const;
    friend class ugdk::action::mode3d::Element;
  private:
    std::weak_ptr<ugdk::action::mode3d::Element> owner_;
};

inline std::shared_ptr<ugdk::action::mode3d::Element> Component::owner() const {
    return owner_.lock();
}

namespace component {

template <class T>
class NullComponent {
  public:
    const static bool is_valid = false;
};

} // namespace component

} // namespace bulletworks

#endif // COMPONENT_H
