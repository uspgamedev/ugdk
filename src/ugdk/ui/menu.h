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
#include <ugdk/graphic/node.h>
#include <ugdk/input.h>
#include <ugdk/input/keys.h>
#include <pyramidworks/collision.h>

namespace ugdk {
namespace ui {
//typedef std::list<const UIElement *> UICollisionList;
    
typedef std::tr1::function<void (Menu*)> MenuCallback;
typedef std::map<input::Key, MenuCallback > InputCallbacks;

class Menu: public action::Entity {
  typedef ikdtree::IntervalKDTree<UIElement*, 2> ObjectTree;
  public:
    Menu(const ikdtree::Box<2>& tree_bounding_box, const Vector2D& offset, action::Scene* owner_scene);
    ~Menu();

    void CheckInteraction(const Vector2D& mouse_pos);

    void AddCallback(input::Key key, MenuCallback callback) {
        input_callbacks_[key] = callback;
    }

    void Update(double dt);
    void OnSceneAdd(action::Scene* scene);

    std::vector<UIElement *>* Menu::GetMouseCollision();

    void FinishScene() { owner_scene_->Finish(); }
    void InteractWithFocused();
    void SetOptionDrawable(graphic::Drawable* option_graphic) { option_graphic_->set_drawable(option_graphic); }

    void AddObject(UIElement* obj);
    void RemoveObject(UIElement* obj);
    void RefreshObject(UIElement* obj);

    graphic::Node* node() { return node_; } 

    const InputCallbacks& input_callbacks() const { return input_callbacks_; }
    
  private:
    graphic::Node* node_;
    graphic::Node* option_graphic_;
    Vector2D last_mouse_position_;
    UIElement* focused_element_;
    std::list< const UIElement* > uielements_;
    action::Scene* owner_scene_;
    ObjectTree* objects_tree_;
    InputCallbacks input_callbacks_;
};

const static MenuCallback FINISH_MENU(&Menu::FinishScene);

} // namespace ui
} // namespace ugdk

#endif /* UGDK_UI_MENU_H_ */
