
#ifndef BULLETWORKS_COMPONENT_H
#define BULLETWORKS_COMPONENT_H

#include <memory>
#include <typeindex>

namespace bulletworks {

class Object;

class Component {
  public:
    virtual ~Component() {}
    virtual std::type_index type() const = 0;
  protected:
    Component() {}
    /// TODO: Make this not accessible from Object
    virtual void OnTake() = 0;
    void Take(const std::weak_ptr<Object> &the_owner);
    std::shared_ptr<Object> owner() const;
    friend class Object;
  private:
    std::weak_ptr<Object> owner_;
};

inline std::shared_ptr<Object> Component::owner() const {
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
