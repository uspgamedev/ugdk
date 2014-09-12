#ifndef UGDK_UI_MENU_H_
#define UGDK_UI_MENU_H_

#include <ugdk/input.h>
#include <ugdk/action.h>
#include <ugdk/graphic.h>
#include <ugdk/ui.h>
#include <ugdk/util.h>
#include <pyramidworks/collision.h>

#include <ugdk/math/integer2D.h>
#include <ugdk/structure.h>
#include <ugdk/action/scene.h>
#include <ugdk/ui/hookpoint.h>

#include <string>
#include <functional>
#include <memory>
#include <map>

namespace ugdk {
namespace ui {
    
typedef std::function<void (Menu*)> MenuCallback;
typedef std::map<input::Keycode, MenuCallback> InputCallbacks;

class Menu: public ::ugdk::action::Scene {
  typedef ugdk::structure::ikdtree::IntervalKDTree<UIElement*, 2> ObjectTree;
  public:
    Menu(const structure::Box<2>& tree_bounding_box,
         const math::Vector2D& offset,
         HookPoint hook = HookPoint::TOP_LEFT);
    ~Menu();

    std::shared_ptr< std::vector<UIElement *> > GetMouseCollision();

    void AddCallback(const ugdk::input::Keycode& key, const MenuCallback& callback);

    void SetOptionDrawable(std::unique_ptr<Drawable>&& option_graphic, int index = 0);

    void AddObject(UIElement* obj);
    void RemoveObject(UIElement* obj);
    void RefreshObject(UIElement* obj);

    Node* node() { return node_.get(); }
    const UIElement* focused_element() const { return focused_element_; }

    const InputCallbacks& input_callbacks() const { return input_callbacks_; }
    
  private:
    void CheckInteraction(const math::Vector2D& mouse_pos);
    void InteractWithFocused();
    void SelectUIElement(UIElement* target);
    void FocusNextElement(int offset);
    void PositionSelectionDrawables();

    std::unique_ptr<Node> node_;
    std::shared_ptr<Node> option_node_[2];

    math::Integer2D last_mouse_position_;
    UIElement* focused_element_;
    std::list< UIElement* > uielements_;
    std::unique_ptr<ObjectTree> objects_tree_;
    InputCallbacks input_callbacks_;
    HookPoint hook_;
};

} // namespace ui
} // namespace ugdk

#endif // UGDK_UI_MENU_H_