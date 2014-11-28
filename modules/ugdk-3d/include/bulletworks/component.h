
#ifndef BULLETWORKS_COMPONENT_H
#define BULLETWORKS_COMPONENT_H

#include <memory>
#include <typeindex>

namespace bulletworks {

class Object;

class Component {
  public:
    virtual ~Component() {}
    virtual std::type_index GetType() const = 0;
  protected:
    Component() {}
    /// TODO: Make this not accessible from Object
    virtual void OnTake(const std::weak_ptr<Object> &owner) = 0;
    void Take(const std::weak_ptr<Object> &owner);
    friend class Object;
  private:
    std::weak_ptr<Object> owner_;
};

template <class T>
class NullComponent {
  public:
    const static bool is_valid = false;
};

} // namespace bulletworks

#endif // COMPONENT_H
