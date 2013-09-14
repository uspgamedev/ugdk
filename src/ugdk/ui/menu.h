#ifndef UGDK_UI_MENU_H_
#define UGDK_UI_MENU_H_

#include <string>
#include <functional>
#include <map>

#include <ugdk/math/vector2D.h>
#include <ugdk/action.h>
#include <ugdk/graphic.h>
#include <ugdk/util.h>
#include <ugdk/structure.h>
#include <ugdk/ui.h>
#include <ugdk/action/entity.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/drawable.h>
#include <ugdk/input.h>
#include <ugdk/input/keys.h>
#include <pyramidworks/collision.h>

namespace ugdk {
namespace ui {
//typedef std::list<const UIElement *> UICollisionList;
    
typedef std::function<void (Menu*)> MenuCallback;
typedef std::map<input::Keycode, MenuCallback > InputCallbacks;

class Menu: public action::Entity {
  typedef structure::ikdtree::IntervalKDTree<UIElement*, 2> ObjectTree;
  public:
    Menu(const structure::Box<2>& tree_bounding_box, const ugdk::math::Vector2D& offset, const graphic::Drawable::HookPoint& hook = graphic::Drawable::TOP_LEFT);
    ~Menu();

    void Update(double dt);
    void OnSceneAdd(action::Scene* scene);

    std::vector<UIElement *>* GetMouseCollision();

    void AddCallback(const input::Keycode& key, const MenuCallback& callback);

    void SetOptionDrawable(graphic::Drawable* option_graphic, int index = 0) {
        if(!option_node_[index]) option_node_[index] = new graphic::Node;
        option_node_[index]->set_drawable(option_graphic);
    }

    void AddObject(UIElement* obj);
    void RemoveObject(UIElement* obj);
    void RefreshObject(UIElement* obj);

    graphic::Node* node() { return node_; }
    const UIElement* focused_element() const { return focused_element_; }

    const InputCallbacks& input_callbacks() const { return input_callbacks_; }

    void FinishScene() const;

    const static MenuCallback FINISH_MENU;
    const static MenuCallback INTERACT_MENU;
    
  private:
    void CheckInteraction(const ugdk::math::Vector2D& mouse_pos);
    void InteractWithFocused();
    void SelectUIElement(UIElement* target);
    void FocusNextElement(int offset);
    void PositionSelectionDrawables();

    graphic::Node* node_;
    graphic::Node* option_node_[2];
    action::Scene* owner_scene_;
    ugdk::math::Vector2D last_mouse_position_;
    UIElement* focused_element_;
    std::list< UIElement* > uielements_;
    ObjectTree* objects_tree_;
    InputCallbacks input_callbacks_;
    graphic::Drawable::HookPoint hook_;
};

} // namespace ui
} // namespace ugdk

#endif /* UGDK_UI_MENU_H_ */
