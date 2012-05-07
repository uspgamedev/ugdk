#ifndef UGDK_MENU_H_
#define UGDK_MENU_H_

#include <string>
#include <list>
#include <set>
#include <ugdk/util.h>
#include <ugdk/action/scene.h>
#include <ugdk/ui/uielement.h>
#include <ugdk/graphic/node.h>
#include <pyramidworks/collision.h>

typedef std::list<const UIElement *> UICollisionList;

class Menu: public ugdk::action::Scene {
  typedef ugdk::ikdtree::IntervalKDTree<const UIElement*, 2> ObjectTree;
  public:
    Menu(const ugdk::ikdtree::Box<2>& tree_bounding_box);
    ~Menu();

    void CheckInteraction(ugdk::Vector2D &mouse_pos);

    void AddObject(const UIElement *obj);
    void RemoveObject(const UIElement *obj);
    void RefreshObject(const UIElement *obj);

  private:
    ugdk::graphic::Node* node_;

    ObjectTree* objects_tree_;
};

#endif /* UGDK_MENU_H_ */
