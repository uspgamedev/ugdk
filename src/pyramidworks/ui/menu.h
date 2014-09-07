#ifndef PYRAMIDWORKS_UI_MENU_H_
#define PYRAMIDWORKS_UI_MENU_H_

#include <pyramidworks/ui.h>
#include <pyramidworks/collision.h>

#include <ugdk/math/vector2D.h>
#include <ugdk/math/integer2D.h>
#include <ugdk/action.h>
#include <ugdk/graphic.h>
#include <ugdk/util.h>
#include <ugdk/structure.h>
#include <ugdk/action/scene.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/drawable.h>
#include <ugdk/input.h>

#include <string>
#include <functional>
#include <memory>
#include <map>

namespace pyramidworks {
namespace ui {
    
typedef std::function<void (Menu*)> MenuCallback;
typedef std::map<ugdk::input::Keycode, MenuCallback> InputCallbacks;

class Menu: public ::ugdk::action::Scene {
  typedef ugdk::structure::ikdtree::IntervalKDTree<UIElement*, 2> ObjectTree;
  public:
    Menu(const ugdk::structure::Box<2>& tree_bounding_box,
         const ugdk::math::Vector2D& offset,
         const ugdk::graphic::Drawable::HookPoint& hook = ugdk::graphic::Drawable::TOP_LEFT);
    ~Menu();

    std::shared_ptr< std::vector<UIElement *> > GetMouseCollision();

    void AddCallback(const ugdk::input::Keycode& key, const MenuCallback& callback);

    void SetOptionDrawable(ugdk::graphic::Drawable* option_graphic, int index = 0) {
        if (!option_node_[index]) {
            option_node_[index] = new ugdk::graphic::Node;
            node_->AddChild(option_node_[index]);
        }
        option_node_[index]->set_drawable(option_graphic);
    }

    void AddObject(UIElement* obj);
    void RemoveObject(UIElement* obj);
    void RefreshObject(UIElement* obj);

    ugdk::graphic::Node* node() { return node_.get(); }
    const UIElement* focused_element() const { return focused_element_; }

    const InputCallbacks& input_callbacks() const { return input_callbacks_; }

    const static MenuCallback FINISH_MENU;
    const static MenuCallback INTERACT_MENU;
    
  private:
    void CheckInteraction(const ugdk::math::Vector2D& mouse_pos);
    void InteractWithFocused();
    void SelectUIElement(UIElement* target);
    void FocusNextElement(int offset);
    void PositionSelectionDrawables();

    std::unique_ptr<ugdk::graphic::Node> node_;

    // Deleted by node_ destructor
    ugdk::graphic::Node* option_node_[2];

    ugdk::math::Integer2D last_mouse_position_;
    UIElement* focused_element_;
    std::list< UIElement* > uielements_;
    std::unique_ptr<ObjectTree> objects_tree_;
    InputCallbacks input_callbacks_;
    ugdk::graphic::Drawable::HookPoint hook_;
};

} // namespace ui
} // namespace pyramidworks

#endif // PYRAMIDWORKS_UI_MENU_H_