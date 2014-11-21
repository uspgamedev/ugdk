
#ifndef UGDK_ACTION_3D_CAMERA_H_
#define UGDK_ACTION_3D_CAMERA_H_

#include <ugdk/action/3D/element.h>
#include <OgreVector3.h>

namespace Ogre {
class Camera;
class Quaternion;
}

namespace ugdk {
namespace action {
namespace mode3d {

class OgreScene;

class Camera : public Element {
public:
    Camera(OgreScene* scene, const std::string& camName="MainCam");
    ~Camera();

    void SetParameters(const Ogre::Vector3& parent_origin_offset = Ogre::Vector3::ZERO, double max_dist=7.5);

    void SetDistance(double dist);
    double GetDistance() { return dist_; }
    void Rotate(double yaw, double pitch);

    Ogre::Camera* camera() { return camera_; }
    Ogre::Quaternion orientation(); //local camera orientation
    Ogre::Quaternion actual_orientation(); //world camera orientation
    
protected:
    Ogre::Camera* camera_;

    Ogre::Vector3 offset_; // offset to parent origin
    double dist_;          // distance to origin (parent origin+offset)
    double max_dist_;
    double cumulative_pitch_;

    virtual void onAttach() override;
    void setupTransform();
};

} // namespace 3D
} // namespace action
} // namespace ugdk
#endif
