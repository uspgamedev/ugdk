#include <ugdk/action/3D/camera.h>
#include <ugdk/action/3D/element.h>
#include <ugdk/action/3D/ogrescene.h>

#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

namespace ugdk {
namespace action {
namespace mode3d {

using Ogre::Vector3;
using Ogre::Quaternion;

Camera::Camera(OgreScene* scene, const std::string& camName) : Element(*scene) {
    camera_ = scene->manager()->createCamera(camName);
    camera_->setNearClipDistance(1);
    node().attachObject(camera_);
    dist_ = 0;
    offset_ = Vector3::ZERO;
    max_dist_ = 7.5;
    cumulative_pitch_ = 0;
}

Camera::~Camera() {
    camera_->getSceneManager()->destroyCamera(camera_);
}

void Camera::SetParameters(const Vector3& parent_origin_offset, double max_dist) {
    offset_ = parent_origin_offset;
    if (max_dist_ > 0)
        max_dist_ = max_dist;
    setupTransform();
}

Quaternion Camera::orientation() {
    return node().getOrientation();
}
Quaternion Camera::actual_orientation() {
    return node().getParentSceneNode()->getOrientation() * node().getOrientation();
}

/*void Camera::injectMouseMoved( const OIS::MouseEvent &arg ) {
    Rotate(-0.13 * arg.state.X.rel, -0.13 * arg.state.Y.rel);
    SetDistance(dist_ + ((arg.state.Z.rel > 0) ? -1 : 1) * 0.225);
}*/
void Camera::SetDistance(double dist) {
    dist_ = dist;
    if (dist_ <= 0) dist_ = 0;
    if (dist_ > max_dist_) dist_ = max_dist_;
    Vector3 pos = node().getParentSceneNode()->getOrientation() * Vector3::UNIT_Z * dist_;
    camera_->setPosition( pos );
}
void Camera::Rotate(double yaw, double pitch) {
    node().yaw(Ogre::Degree( yaw ), Ogre::Node::TS_WORLD);

    cumulative_pitch_ += pitch;
    if ( Ogre::Math::Abs(cumulative_pitch_) <= 90 )
         node().pitch(Ogre::Degree( pitch ), Ogre::Node::TS_LOCAL);
    else
        cumulative_pitch_ -= pitch;
}

void Camera::OnAttach() {
    setupTransform();
}

void Camera::OnParentDestroyed() {
    Ogre::SceneManager *mgr = node().getCreator();
    node().getParentSceneNode()->removeChild(&node());
    mgr->getRootSceneNode()->addChild(&node());
}

void Camera::setupTransform() {
    node().setPosition( offset_ );
    node().setOrientation( Quaternion::IDENTITY );
    camera_->setPosition( node().getParentSceneNode()->getOrientation() * Vector3::UNIT_Z * dist_);
}

} // namespace 3D
} // namespace action
} // namespace ugdk
