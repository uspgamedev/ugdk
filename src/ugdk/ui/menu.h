#ifndef UGDK_UI_MENU_H_
#define UGDK_UI_MENU_H_

#include <string>
#include <functional>
#include <map>
#include <ugdk/util.h>
#include <ugdk/action/scene.h>
#include <ugdk/input.h>
#include <ugdk/input/keys.h>
#include <ugdk/ui/uielement.h>
#include <ugdk/graphic.h>
#include <pyramidworks/collision.h>

namespace ugdk {
namespace ui {

//typedef std::list<const UIElement *> UICollisionList;

class Menu: public action::Scene {
  typedef ikdtree::IntervalKDTree<const UIElement*, 2> ObjectTree;
  public:
    typedef std::map<input::Key, std::tr1::function<void (Menu*)> > InputCallbacks;

    Menu(const ikdtree::Box<2>& tree_bounding_box);
    ~Menu();

    void CheckInteraction(const Vector2D &mouse_pos);

    void AddCallback(input::Key key, std::tr1::function<void (Menu*)> callback) {
        input_callbacks_[key] = callback;
    }

    void AddObject(const UIElement *obj);
    void RemoveObject(const UIElement *obj);
    void RefreshObject(const UIElement *obj);

    const InputCallbacks& input_callbacks() const { return input_callbacks_; }

  private:
    ObjectTree* objects_tree_;
    InputCallbacks input_callbacks_;
};

} // namespace ui
} // namespace ugdk

#endif /* UGDK_UI_MENU_H_ */
