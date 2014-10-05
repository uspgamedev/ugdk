#include <ugdk/action/3D/ogreentity.h>

#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

namespace ugdk {
namespace action {
namespace threed {

OgreEntity::~OgreEntity() {
    if (node_) {
        //TODO: delete attached objects/ogre::Entities from child nodes - this function only detaches them
        node_->removeAndDestroyAllChildren();
        node_->getParentSceneNode()->removeChild(node_);
        node_->getCreator()->destroySceneNode(node_);
    }
}

} // namespace threed
} // namespace action
} // namespace ugdk
