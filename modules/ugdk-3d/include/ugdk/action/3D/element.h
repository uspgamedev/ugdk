#ifndef UGDK_ACTION_3D_ELEMENT_H_
#define UGDK_ACTION_3D_ELEMENT_H_

namespace Ogre {
class SceneNode;
}

namespace ugdk {
namespace action {
namespace mode3d {

class OgreScene;

/**
   @class Element
   @brief A 3D Element.

   Represents an Element in the 3D world.
*/
class Element {
public:
    virtual ~Element();

    void AttachTo(Element& parent);
    void AttachTo(OgreScene& scene);
    void Destroy();

    Ogre::SceneNode& node() { return *node_; }

protected:
    Element(OgreScene& scene);
    virtual void OnAttach() {}
    virtual void OnParentDestroyed() = 0;
    
    Ogre::SceneNode* node_;
    
}; // class Element.

} // namespace 3D
} /* namespace action */
} /* namespace ugdk */

#endif /* UGDK_ACTION_3D_ELEMENT_H_ */
