
#ifndef UGDK_ACTION_3D_COMPONENT_H
#define UGDK_ACTION_3D_COMPONENT_H

#include <memory>
#include <typeindex>

namespace ugdk {
namespace action {
namespace mode3d {

class Element;

class Component {
  public:
    virtual ~Component() {}
    virtual std::type_index type() const = 0;
    std::shared_ptr<Element> owner() const;
  protected:
    Component() {}
    /// TODO: Make this not accessible from Object
    virtual void OnTaken() = 0;
    void Take(const std::weak_ptr<Element> &the_owner);
    friend class Element;
  private:
    std::weak_ptr<Element> owner_;
};

inline std::shared_ptr<Element> Component::owner() const {
    return owner_.lock();
}

namespace component {

template <class T>
class NullComponent {
  public:
    const static bool is_valid = false;
};

} // namespace component
} // namespace mode3d
} // namespace action
} // namespace ugdk

#endif // UGDK_ACTION_3D_COMPONENT_H
