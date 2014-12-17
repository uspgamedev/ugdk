
#include <ugdk/action/3D/component/view.h>
#include <ugdk/action/3D/element.h>

#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreSceneManager.h>

namespace ugdk {
namespace action {
namespace mode3d {
namespace component {

void View::AddEntity(const std::string& meshName) {
    AddEntity("", meshName);
}

void View::AddEntity(const std::string& entName, const std::string& meshName) {
    if (auto parent = owner()) {
        auto sceneMgr = parent->node().getCreator();
        if (entName.empty())
            AddEntity(sceneMgr->createEntity(meshName));
        else
            AddEntity(sceneMgr->createEntity(entName, meshName));
    }
    else {
        pending_entities_.emplace_back("", meshName);
    }
}

void View::AddEntity(Ogre::Entity* ent) {
    if (auto parent = owner()) {
        parent->node().attachObject(ent);
    }
    entities_.push_back(ent);
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
