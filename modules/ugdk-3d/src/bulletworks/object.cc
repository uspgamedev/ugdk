#include <bulletworks/object.h>
#include <bulletworks/component.h>
#include <bulletworks/manager.h>
#include <bulletworks/physicscene.h>

#include <btBulletDynamicsCommon.h>

#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

#include <BtOgreExtras.h>
#include <BtOgrePG.h>


namespace bulletworks {

using std::shared_ptr;

Object::Object(PhysicScene& scene, Ogre::Entity* entity)
    : Element(scene), entity_(entity) {}

Object::~Object() {
    node_->getCreator()->destroyEntity(entity_);
    //Element takes care of destroying our node_
}

std::string Object::entity_name() {
    return entity_->getName();
}

void Object::AddToScene(bulletworks::PhysicScene* scene) {
    //node_ = scene->manager()->getRootSceneNode()->createChildSceneNode();
    node_->attachObject(entity_);
    //node_->setPosition(BtOgre::Convert::toOgre(physics_data_.initial.getOrigin()));
    //node_->setOrientation(BtOgre::Convert::toOgre(physics_data_.initial.getRotation()));
}

void Object::AddComponent(const std::shared_ptr<Component> &component) {
    components_[component->type()] = component;
    component->Take(shared_from_this());
}

}
