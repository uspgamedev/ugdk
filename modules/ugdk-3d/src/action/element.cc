#include <ugdk/action/3D/element.h>
#include <ugdk/action/3D/scene3d.h>
#include <ugdk/action/3D/component.h>

#include <OgreAny.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

namespace ugdk {
namespace action {
namespace mode3d {

using Ogre::Any;

Element::Element(Scene3D& scene, const std::string& name) : scene_(scene), name_(name) {
    node_ = scene.manager()->getRootSceneNode()->createChildSceneNode(name);
    node_->getUserObjectBindings().setUserAny("owner", Any(this));
}

Element::~Element() {}

void Element::AddComponent(const std::shared_ptr<Component> &the_component) {
    components_[the_component->type()] = the_component;
    the_component->Take(shared_from_this());
}

void Element::AttachTo(Element& parent) {
    node_->getParentSceneNode()->removeChild(node_);
    parent.node().addChild(node_);
    this->OnAttach();
}

void Element::AttachTo(Scene3D& scene) {
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
