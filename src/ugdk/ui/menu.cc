#include <vector>
#include "menu.h"
#include <ugdk/action/generictask.h>
#include <ugdk/base/engine.h>
#include <ugdk/graphic/node.h>
#include <ugdk/input/inputmanager.h>
#include <ugdk/util/intervalkdtree.h>

namespace ugdk {
namespace ui {

using std::tr1::placeholders::_1;

static bool CheckMouse(Menu* menu, double dt) {
    input::InputManager* input = INPUT_MANAGER();
    if(input->MousePressed(input::M_BUTTON_LEFT)) {
        menu->CheckInteraction(input->GetMousePosition());
    }
    return true;
}

Menu::Menu(const ugdk::ikdtree::Box<2>& tree_bounding_box) 
  : objects_tree_(new ObjectTree(tree_bounding_box,5)) {

      std::tr1::function<void (double)> func = std::tr1::bind(&CheckMouse, this, _1);
      this->AddTask(new action::GenericTask(func));
}

Menu::~Menu() { delete objects_tree_; }

void Menu::CheckInteraction(const Vector2D &mouse_pos) {
    double min_coords[2], max_coords[2];
    min_coords[0] = mouse_pos.x - 0.5;
    min_coords[1] = mouse_pos.y - 0.5;
    max_coords[0] = min_coords[0] + 1;
    max_coords[1] = min_coords[1] + 1;
    ugdk::ikdtree::Box<2> box(min_coords, max_coords);

    std::vector<const UIElement *> *intersecting_uielements = objects_tree_->getIntersectingItems(box);
    if(intersecting_uielements->size() > 0)
        (*intersecting_uielements)[0]->Interact();
}

void Menu::AddObject(const UIElement *obj) {
    objects_tree_->Insert(obj->GetBoundingBox(), obj);
    interface_node()->AddChild(obj->node());
}

void Menu::RemoveObject(const UIElement *obj) { 
    objects_tree_->Remove(obj);
    interface_node()->RemoveChild(obj->node());
}

void Menu::RefreshObject(const UIElement *obj) {
    objects_tree_->Update(obj->GetBoundingBox(), obj);
}

} // namespace ui
} // namespace ugdk
