#ifndef UGDK_ACTION_3D_COMPONENT_VIEW_H
#define UGDK_ACTION_3D_COMPONENT_VIEW_H

#include <ugdk/action/3D/component.h>
#include <OgreVector3.h>
#include <vector>
#include <utility>

namespace Ogre {
class Vector3;
class Entity;
}

namespace ugdk {
namespace action {
namespace mode3d {
namespace component {

class View : public Component {

  public:
    View() {}

    virtual std::type_index type() const override;

    void AddEntity(const std::string& meshName);
    void AddEntity(const std::string& entName, const std::string& meshName);
    void AddEntity(Ogre::Entity* ent);
    
  protected:
    void OnTaken() override;

    std::vector<std::pair<std::string, std::string>> pending_entities_;
    std::vector<Ogre::Entity*> entities_;
};

inline std::type_index View::type() const {
    return typeid(View);
}

} // namespace component
} // namespace mode3d
} // namespace action
} // namespace ugdk

#endif // UGDK_ACTION_3D_COMPONENT_VIEW_H
