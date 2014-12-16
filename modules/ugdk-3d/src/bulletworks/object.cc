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

using std::unique_ptr;

Object::Object(PhysicScene& scene, Ogre::Entity* entity)
    : Element(scene), entity_(entity) {}

Object::~Object() {
    //Element takes care of destroying our node_
}

std::string Object::entity_name() {
    return entity_->getName();
}

void Object::AddToScene(bulletworks::PhysicScene* scene) {
    //node_ = scene->manager()->getRootSceneNode()->createChildSceneNode();
    //node_->attachObject(entity_);
    //scene->AddObject(shared_from_this());
    //node_->setPosition(BtOgre::Convert::toOgre(physics_data_.initial.getOrigin()));
    //node_->setOrientation(BtOgre::Convert::toOgre(physics_data_.initial.getRotation()));
}

void Object::AddComponent(const std::unique_ptr<Component> &the_component) {
    //components_[the_component->type()] = the_component;
    //the_component->Take(shared_from_this());
}

}
