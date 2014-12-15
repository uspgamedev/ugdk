#ifndef BULLETWORKS_OBJECT_H_
#define BULLETWORKS_OBJECT_H_

#include <ugdk/action/3D/element.h>
#include <ugdk/action.h>

#include <bulletworks/component.h>

#include <LinearMath/btTransform.h>

#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>

namespace Ogre {
class Entity;
class Vector3;
}

namespace bulletworks {
class PhysicScene;

class Object : public ugdk::action::mode3d::Element, public std::enable_shared_from_this<Object> {
  public:

    Object(PhysicScene& scene, Ogre::Entity* entity);
    ~Object();

    void AddToScene(bulletworks::PhysicScene* scene);

    void AddComponent(const std::shared_ptr<Component> &the_component);
    template <class T>
    T* component();

    Ogre::Entity* entity() const { return entity_; }
    std::string entity_name();

  protected:
    virtual void OnParentDestroyed() override {}
    Ogre::Entity* entity_;

  private:
    std::unordered_map<std::type_index, std::shared_ptr<Component>> components_;
};

template <class T>
T* Object::component() {
    auto check = components_.find(typeid(T));
    if (check == components_.end()) {
        return nullptr;
    }
    return dynamic_cast<T*>(check->second.get());
}

}
#endif
