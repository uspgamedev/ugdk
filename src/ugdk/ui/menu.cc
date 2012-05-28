#include <vector>
#include <functional>

#include <ugdk/ui/menu.h>
#include <ugdk/ui/uielement.h>

#include <ugdk/action/generictask.h>
#include <ugdk/action/scene.h>
#include <ugdk/base/engine.h>
#include <ugdk/graphic/node.h>
#include <ugdk/input/inputmanager.h>
#include <ugdk/util/intervalkdtree.h>

using std::tr1::placeholders::_1;

namespace ugdk {
namespace ui {

class CallbackCheckTask : public action::Task {
public:
    CallbackCheckTask(Menu* menu) : menu_(menu) {}
    void operator()(double dt) {
        input::InputManager* input = INPUT_MANAGER();
        const InputCallbacks& callbacks = menu_->input_callbacks();
        for(InputCallbacks::const_iterator it = callbacks.begin(); it != callbacks.end(); ++it) {
            if(input->KeyPressed(it->first))
                it->second(menu_);
        }
    }
private:
    Menu* menu_;
};

Menu::Menu(const ugdk::ikdtree::Box<2>& tree_bounding_box, const Vector2D& offset, action::Scene* owner_scene) 
  : objects_tree_(new ObjectTree(tree_bounding_box,5)),
    owner_scene_(owner_scene),
    node_(new graphic::Node()),
    option_graphic_(new graphic::Node()) {
      node_->modifier()->set_offset(offset);
      //std::tr1::function<bool (double)> func = std::tr1::bind(&CheckMouse, this, _1);
      //this->AddTask(new action::GenericTask(func));
      //this->AddTask(new CallbackCheckTask(this));
}

Menu::~Menu() { delete objects_tree_; }

void Menu::Update(double dt) {
    input::InputManager* input = INPUT_MANAGER();
    Vector2D mouse_pos = input->GetMousePosition();
    if((mouse_pos - last_mouse_position_).NormOne() > 10e-10) {
        std::vector<UIElement *> *intersecting_uielements = GetMouseCollision();
        if(intersecting_uielements->size() > 0) {
            focused_element_ = (*intersecting_uielements)[0];
            focused_element_->node()->AddChild(option_graphic_);
        }
        delete intersecting_uielements;
    }
    last_mouse_position_ = mouse_pos;
    if(input->MouseUp(input::M_BUTTON_LEFT))
        this->CheckInteraction(mouse_pos);
}

void Menu::OnSceneAdd(action::Scene* scene) {
    scene->AddTask(new CallbackCheckTask(this));
}

std::vector<UIElement *>* Menu::GetMouseCollision() {
    Vector2D mouse_pos = INPUT_MANAGER()->GetMousePosition();
    double min_coords[2], max_coords[2];
    min_coords[0] = mouse_pos.x - 0.5;
    min_coords[1] = mouse_pos.y - 0.5;
    max_coords[0] = min_coords[0] + 1;
    max_coords[1] = min_coords[1] + 1;
    ugdk::ikdtree::Box<2> box(min_coords, max_coords);

    return objects_tree_->getIntersectingItems(box);
}

void Menu::InteractWithFocused() { focused_element_->Interact(); }

void Menu::CheckInteraction(const Vector2D &mouse_pos) {
    std::vector<UIElement *>* intersecting_uielements = GetMouseCollision();
    if(intersecting_uielements->size() > 0)
        (*intersecting_uielements)[0]->Interact();
    delete intersecting_uielements;
}

void Menu::AddObject(UIElement *obj) {
    objects_tree_->Insert(obj->GetBoundingBox(), obj);
    node_->AddChild(obj->node());
    uielements_.push_back(obj);
}

void Menu::RemoveObject(UIElement *obj) { 
    objects_tree_->Remove(obj);
    node_->RemoveChild(obj->node());
    uielements_.remove(obj);
}

void Menu::RefreshObject(UIElement *obj) {
    objects_tree_->Update(obj->GetBoundingBox(), obj);
}

//static MenuCallback FINISH_MENU(&Menu::FinishScene);


} // namespace ui
} // namespace ugdk
