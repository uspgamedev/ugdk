/*
 * =====================================================================================
 *
 *       Filename:  BtOgrePG.h
 *
 *    Description:  The part of BtOgre that handles information transfer from Bullet to
 *                  Ogre (like updating graphics object positions).
 *
 *        Version:  1.0
 *        Created:  27/12/2008 03:40:56 AM
 *
 *         Author:  Nikhilesh (nikki)
 *
 * =====================================================================================
 */

#ifndef _BtOgreGP_H_
#define _BtOgreGP_H_

#include "btBulletDynamicsCommon.h"
#include "OgreSceneNode.h"
#include "BtOgreExtras.h"

namespace BtOgre {

//A MotionState is Bullet's way of informing you about updates to an object.
//Pass this MotionState to a btRigidBody to have your SceneNode updated automaticaly.
class RigidBodyState : public btMotionState {

  public:
    RigidBodyState (Ogre::SceneNode *node, const btTransform &transform,
                    const btTransform &offset = btTransform::getIdentity(),
                    bool only_position = false)
        : transform_(transform), center_of_mass_offset_(offset), node_(node),
          only_position_(only_position) {}

    RigidBodyState (Ogre::SceneNode *node, bool only_position = false)
        : transform_(((node != NULL) ? BtOgre::Convert::toBullet(node->getOrientation())
                                     : btQuaternion(0,0,0,1)),
                     ((node != NULL) ? BtOgre::Convert::toBullet(node->getPosition())
                                     : btVector3(0,0,0))),
          center_of_mass_offset_(btTransform::getIdentity()),
          node_(node), only_position_(only_position) {}

    virtual void getWorldTransform(btTransform &ret) const {
        ret = transform_;
    }

    virtual void setWorldTransform(const btTransform &in) {
        if (node_ == NULL)
            return;

        transform_ = in;
        btTransform transform = in * center_of_mass_offset_;

        if (!only_position_) {
            btQuaternion rot = transform.getRotation();
            node_->setOrientation(rot.w(), rot.x(), rot.y(), rot.z());
        }
        btVector3 pos = transform.getOrigin();
        node_->setPosition(pos.x(), pos.y(), pos.z());
    }

    void setNode(Ogre::SceneNode *node) {
        node_ = node;
    }

  protected:

    btTransform     transform_;
    btTransform     center_of_mass_offset_;

    Ogre::SceneNode *node_;
    bool            only_position_;
};

//Softbody-Ogre connection goes here!

}

#endif
