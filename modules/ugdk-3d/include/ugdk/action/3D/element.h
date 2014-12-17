#ifndef UGDK_ACTION_3D_ELEMENT_H_
#define UGDK_ACTION_3D_ELEMENT_H_

#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>

namespace Ogre {
class SceneNode;
}

namespace ugdk {
namespace action {
namespace mode3d {

class Scene3D;
class Component;

/**
   @class Element
   @brief A 3D Element.

   Represents an Element in the 3D world.
*/
class Element : public std::enable_shared_from_this<Element> {
  public:
    Element(Scene3D& scene);
    virtual ~Element();

    void AddComponent(const std::shared_ptr<Component> &the_component);
    template <class T>
    T* component();

    void AttachTo(Element& parent);
    void AttachTo(Scene3D& scene);
    void Destroy();

    Ogre::SceneNode& node() { return *node_; }

  protected:
    virtual void OnAttach() {}
    virtual void OnParentDestroyed() {}
    
  private:
    std::unordered_map<std::type_index, std::shared_ptr<Component>>    components_;
    Ogre::SceneNode                                                         *node_;
    
}; // class Element.

template <class T>
T* Element::component() {
    auto check = components_.find(typeid(T));
    if (check == components_.end()) {
        return nullptr;
    }
    return dynamic_cast<T*>(check->second.get());
}

} // namespace 3D
} // namespace action
} // namespace ugdk

#endif /* UGDK_ACTION_3D_ELEMENT_H_ */
