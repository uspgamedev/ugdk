#ifndef UGDK_ACTION_3D_OGREENTITY_H_
#define UGDK_ACTION_3D_OGREENTITY_H_

namespace Ogre {
class SceneNode;
}

namespace ugdk {
namespace action {
namespace mode3d {

/**
   @class OgreEntity
   @brief A 3D game entity.

   Represents an entity in the 3D world.
   Not to be confused with Ogre::Entity*, which is a Ogre class to represent a graphical entity.
*/
class OgreEntity {
  public:
   virtual ~OgreEntity();

    Ogre::SceneNode* node() { return node_; }

  protected:
    OgreEntity() : node_(nullptr) {}
    
    Ogre::SceneNode* node_;
    
}; // class OgreEntity.

} // namespace 3D
} /* namespace action */
} /* namespace ugdk */

#endif /* UGDK_ACTION_3D_OGREENTITY_H_ */
