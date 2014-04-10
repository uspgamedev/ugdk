#include <ugdk/action/3D/ogreentity.h>

#include <OgreSceneNode.h>

namespace ugdk {
namespace action {
namespace threed {

OgreEntity::~OgreEntity() {
    if (node_) {
        //TODO: delete attached objects/ogre::Entities from child nodes - this function only detaches them
        node_->removeAndDestroyAllChildren();
        node_->getParentSceneNode()->removeChild(node_);
        delete node_;
    }
}

} // namespace threed
} // namespace action
} // namespace ugdk
