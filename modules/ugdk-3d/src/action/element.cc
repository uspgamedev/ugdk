#include <ugdk/action/3D/element.h>
#include <ugdk/action/3D/ogrescene.h>

#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

namespace ugdk {
namespace action {
namespace mode3d {

Element::Element(OgreScene& scene) {
    node_ = scene.manager()->getRootSceneNode()->createChildSceneNode();
}

Element::~Element() {
    //TODO: delete attached objects/ogre::Entities from child nodes - this function only detaches them
    node_->removeAndDestroyAllChildren();
    node_->getParentSceneNode()->removeChild(node_);
    node_->getCreator()->destroySceneNode(node_);
}

void Element::AttachTo(Element& parent) {
    node_->getParentSceneNode()->removeChild(node_);
    parent.node().addChild(node_);
    onAttach();
}

void Element::AttachTo(OgreScene& scene) {
    node_->getParentSceneNode()->removeChild(node_);
    scene.manager()->getRootSceneNode()->addChild(node_);
    onAttach();
}

} // namespace mode3d
} // namespace action
} // namespace ugdk
