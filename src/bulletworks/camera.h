
#ifndef BULLETWORKS_CAMERA_H_
#define BULLETWORKS_CAMERA_H_

#include <OgreVector3.h>

namespace Ogre {
class Camera;
class SceneManager;
class Quaternion;
}

namespace bulletworks {

class Object;

class Camera {
public:
    Camera(Ogre::SceneManager* sceneMgr, const std::string& camName="DefaultCam");
    ~Camera();

    void AttachTo(Object* object);

    void SetParameters(const Ogre::Vector3& parent_origin_offset = Ogre::Vector3::ZERO, double max_dist=7.5);

    void SetDistance(double dist);
    double GetDistance() { return dist_; }
    void Rotate(double yaw, double pitch);

    Ogre::Camera* camera() { return camera_; }
    Object* parent() { return parent_; }
    Ogre::Quaternion orientation(); //local camera orientation
    Ogre::Quaternion actual_orientation(); //world camera orientation

    //void injectMouseMoved( const OIS::MouseEvent &arg );

protected:
    Ogre::Camera* camera_;
	Object* parent_;

    Ogre::Vector3 offset_; // offset to parent origin
    double dist_;          // distance to origin (parent origin+offset)
    double max_dist_;
    double cumulative_pitch_;

    void setupTransform();
};

}
#endif
