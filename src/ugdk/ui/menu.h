#ifndef UGDK_UI_MENU_H_
#define UGDK_UI_MENU_H_

#include <string>
#include <functional>
#include <map>
#include <ugdk/math/vector2D.h>
#include <ugdk/action.h>
#include <ugdk/graphic.h>
#include <ugdk/util.h>
#include <ugdk/ui.h>
#include <ugdk/action/scene.h>
#include <ugdk/action/entity.h>
#include <ugdk/input.h>
#include <ugdk/input/keys.h>
#include <pyramidworks/collision.h>

namespace ugdk {
namespace ui {
//typedef std::list<const UIElement *> UICollisionList;

class Menu: public action::Entity {
  typedef ikdtree::IntervalKDTree<const UIElement*, 2> ObjectTree;
  public:
    typedef std::tr1::function<void (Menu*)> MenuCallback;
    typedef std::map<input::Key, MenuCallback > InputCallbacks;

    Menu(const ikdtree::Box<2>& tree_bounding_box, const Vector2D& offset);
    ~Menu();

    void CheckInteraction(const Vector2D& mouse_pos);

    void AddCallback(input::Key key, MenuCallback callback) {
        input_callbacks_[key] = callback;
    }

    void Update(double dt);
    void OnSceneAdd(action::Scene* scene);

    void AddObject(const UIElement* obj);
    void RemoveObject(const UIElement* obj);
    void RefreshObject(const UIElement* obj);

    graphic::Node* node() { return node_; } 

    const InputCallbacks& input_callbacks() const { return input_callbacks_; }

    //static MenuCallback FINISH_MENU;

  private:
    ObjectTree* objects_tree_;
    InputCallbacks input_callbacks_;
    graphic::Node* node_;
};

} // namespace ui
} // namespace ugdk

#endif /* UGDK_UI_MENU_H_ */
