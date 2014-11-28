
#ifndef BULLETWORKS_COMPONENT_H
#define BULLETWORKS_COMPONENT_H

#include <memory>

namespace bulletworks {

class Object;

class Component {
  public:
    Component();
  protected:
    /// TODO: Make this not accessible from Object
    virtual void OnTake(const std::weak_ptr<Object> &owner) = 0;
    void Take(const std::weak_ptr<Object> &owner);
    friend class Object;
  private:
    std::weak_ptr<Object> owner_;
};

} // namespace bulletworks

#endif // COMPONENT_H
