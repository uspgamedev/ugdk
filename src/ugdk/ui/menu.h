#ifndef UGDK_UI_MENU_H_
#define UGDK_UI_MENU_H_

#include <string>
#include <list>
#include <set>
#include <ugdk/util.h>
#include <ugdk/action/scene.h>
#include <ugdk/ui/uielement.h>
#include <ugdk/graphic.h>
#include <pyramidworks/collision.h>

namespace ugdk {
namespace ui {

typedef std::list<const UIElement *> UICollisionList;

class Menu: public action::Scene {
  typedef ikdtree::IntervalKDTree<const UIElement*, 2> ObjectTree;
  public:
    Menu(const ikdtree::Box<2>& tree_bounding_box);
    ~Menu();

    void CheckInteraction(const Vector2D &mouse_pos);

    void AddObject(const UIElement *obj);
    void RemoveObject(const UIElement *obj);
    void RefreshObject(const UIElement *obj);

  private:
    ObjectTree* objects_tree_;
};

} // namespace ui
} // namespace ugdk

#endif /* UGDK_UI_MENU_H_ */
