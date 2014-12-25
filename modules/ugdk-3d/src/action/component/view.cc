
#include <ugdk/action/3D/component/view.h>
#include <ugdk/action/3D/element.h>

#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreSceneManager.h>

namespace ugdk {
namespace action {
namespace mode3d {
namespace component {

Ogre::Entity* View::AddEntity(const std::string& meshName) {
    return AddEntity("", meshName);
}

Ogre::Entity* View::AddEntity(const std::string& entName, const std::string& meshName) {
    if (auto parent = owner()) {
        auto sceneMgr = parent->node().getCreator();
        if (entName.empty())
            return AddEntity(sceneMgr->createEntity(meshName));
        else
            return AddEntity(sceneMgr->createEntity(entName, meshName));
    }
    else {
        pending_entities_.emplace_back("", meshName);
    }
    return nullptr;
}

Ogre::Entity* View::AddEntity(Ogre::Entity* ent) {
    if (auto parent = owner()) {
        parent->node().attachObject(ent);
    }
    entities_.push_back(ent);
    return ent;
}

Ogre::Entity* View::GetEntity(int index) {
    if (entities_.size() >= index) return nullptr;
    return entities_.at(index);
}
Ogre::Entity* View::GetEntity(const std::string& entName) {
    if (auto parent = owner()) {
        return static_cast<Ogre::Entity*>(parent->node().getAttachedObject(entName));
    }
    return nullptr;
}

void View::OnTaken() {
    auto parent = owner();
    auto sceneMgr = parent->node().getCreator();
    for (const auto& pent : pending_entities_) {
        if (pent.first.empty())
            entities_.push_back(sceneMgr->createEntity(pent.second));
        else
            entities_.push_back(sceneMgr->createEntity(pent.first, pent.second));
    }
    pending_entities_.clear();

    for (auto ent : entities_) {
        parent->node().attachObject(ent);
    }
}

} // namespace component
} // namespace mode3d
} // namespace action
} // namespace ugdk
