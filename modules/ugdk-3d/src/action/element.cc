#include <ugdk/action/3D/element.h>
#include <ugdk/action/3D/ogrescene.h>

#include <OgreAny.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

namespace ugdk {
namespace action {
namespace mode3d {

using Ogre::Any;

Element::Element(OgreScene& scene) {
    node_ = scene.manager()->getRootSceneNode()->createChildSceneNode();
    node_->getUserObjectBindings().setUserAny("owner", Any(this));
}

Element::~Element() {}

void Element::AttachTo(Element& parent) {
    node_->getParentSceneNode()->removeChild(node_);
    parent.node().addChild(node_);
    this->OnAttach();
}

void Element::AttachTo(OgreScene& scene) {
    node_->getParentSceneNode()->removeChild(node_);
    scene.manager()->getRootSceneNode()->addChild(node_);
    this->OnAttach();
}

void Element::Destroy() {
    for (auto child : node_->getChildIterator()) {
        Element *element = child.second->getUserObjectBindings().getUserAny("owner").get<Element*>();
        element->OnParentDestroyed();
    }
    node_->removeAllChildren();
    //TODO: delete attached objects/ogre::Entities from child nodes - this function only detaches them
    if (Ogre::SceneNode *parent = node_->getParentSceneNode())
        parent->removeChild(node_);
    node_->getCreator()->destroySceneNode(node_);
}

} // namespace mode3d
} // namespace action
} // namespace ugdk
