#include <ugdk/action/3D/camera.h>
#include <ugdk/action/3D/ogreentity.h>
#include <ugdk/action/3D/ogrescene.h>

#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

namespace ugdk {
namespace action {
namespace threed {

using Ogre::Vector3;
using Ogre::Quaternion;

Camera::Camera(OgreScene* scene, const std::string& camName) {
    parent_ = nullptr;
    camera_ = scene->manager()->createCamera(camName);
    camera_->setNearClipDistance(1);
    dist_ = 0;
    offset_ = Vector3::ZERO;
    max_dist_ = 7.5;
    cumulative_pitch_ = 0;
}

Camera::~Camera() {
    camera_->getSceneManager()->destroyCamera(camera_);
}

void Camera::AttachTo(OgreEntity* ent) {
    if (parent_ != nullptr) {
        Ogre::SceneNode* oldCamNode = camera_->getParentSceneNode();
        parent_->node()->removeChild( oldCamNode );
        camera_->detachFromParent();
        delete oldCamNode;
    }
    parent_ = ent;
    parent_->node()->createChildSceneNode()->attachObject(camera_);

    setupTransform();
}

void Camera::SetParameters(const Vector3& parent_origin_offset, double max_dist) {
    offset_ = parent_origin_offset;
    if (max_dist_ > 0)
        max_dist_ = max_dist;
    setupTransform();
}

Quaternion Camera::orientation() {
    if (parent_ != nullptr)
        return camera_->getParentSceneNode()->getOrientation();
    return Quaternion::IDENTITY;
}
Quaternion Camera::actual_orientation() {
    if (parent_ != nullptr)
        return parent_->node()->getOrientation() * camera_->getParentSceneNode()->getOrientation();
    return Quaternion::IDENTITY;
}

/*void Camera::injectMouseMoved( const OIS::MouseEvent &arg ) {
    Rotate(-0.13 * arg.state.X.rel, -0.13 * arg.state.Y.rel);
    SetDistance(dist_ + ((arg.state.Z.rel > 0) ? -1 : 1) * 0.225);
}*/
void Camera::SetDistance(double dist) {
    dist_ = dist;
    if (dist_ <= 0) dist_ = 0;
    if (dist_ > max_dist_) dist_ = max_dist_;
    if (parent_ == nullptr) return;
    Vector3 pos = parent_->node()->getOrientation() * Vector3::UNIT_Z * dist_;
    camera_->setPosition( pos );
}
void Camera::Rotate(double yaw, double pitch) {
    if (parent_ == nullptr) return;
    Ogre::SceneNode* node = camera_->getParentSceneNode();
    node->yaw(Ogre::Degree( yaw ), Ogre::Node::TS_WORLD);

    cumulative_pitch_ += pitch;
    if ( Ogre::Math::Abs(cumulative_pitch_) <= 90 )
         node->pitch(Ogre::Degree( pitch ), Ogre::Node::TS_LOCAL);
    else
        cumulative_pitch_ -= pitch;
}

void Camera::setupTransform() {
    if (parent_ == nullptr) return;
    Ogre::SceneNode* node = camera_->getParentSceneNode();

    node->setPosition( offset_ );
    node->setOrientation( Quaternion::IDENTITY );
    camera_->setPosition( parent_->node()->getOrientation() * Vector3::UNIT_Z * dist_ );
}

} // namespace 3D
} // namespace action
} // namespace ugdk
