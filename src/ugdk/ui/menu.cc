
#include <ugdk/ui/menu.h>
#include <ugdk/ui/uielement.h>

#include <ugdk/action/scene.h>
#include <ugdk/graphic/drawable.h>
#include <ugdk/graphic/node.h>
#include <ugdk/input/module.h>
#include <ugdk/input/scancode.h>
#include <ugdk/input/events.h>
#include <ugdk/structure/intervalkdtree.h>

#include <vector>
#include <algorithm>
#include <iterator>

using std::mem_fn;
using std::placeholders::_1;

namespace ugdk {
namespace ui {

const MenuCallback Menu::FINISH_MENU(mem_fn(&Menu::Finish));
const MenuCallback Menu::INTERACT_MENU(mem_fn(&Menu::InteractWithFocused));

Menu::Menu(const ugdk::structure::Box<2>& tree_bounding_box, const ugdk::math::Vector2D& offset, const graphic::Drawable::HookPoint& hook) 
  : node_(new graphic::Node)
  , focused_element_(nullptr)
  , objects_tree_(new ObjectTree(tree_bounding_box,5))
  , hook_(hook) 
{
    node_->geometry().ChangeOffset(offset);
    option_node_[0] = nullptr;
    option_node_[1] = nullptr;

    AddTask([this](double) {
        input::Manager* input = input::manager();
        ugdk::math::Vector2D mouse_pos = input->mouse().position();
        if ((mouse_pos - last_mouse_position_).NormOne() > 10e-10) {
            auto intersecting_uielements = GetMouseCollision();
            if (intersecting_uielements->size() > 0)
                SelectUIElement((*intersecting_uielements)[0]);
        }
        last_mouse_position_ = mouse_pos;
        if (input->mouse().IsReleased(input::MouseButton::LEFT))
            this->CheckInteraction(mouse_pos);
        if (input->keyboard().IsReleased(input::Scancode::DOWN))
            this->FocusNextElement(1);
        if (input->keyboard().IsReleased(input::Scancode::UP))
            this->FocusNextElement(-1);

        for (const auto& it : input_callbacks()) {
            if (input->keyboard().IsPressed(it.first))
                it.second(this);
        }
    });

    set_render_function(std::bind(&graphic::Node::Render, node_.get(), _1));
}

Menu::~Menu() {
}

void Menu::AddCallback(const input::Keycode& key, const MenuCallback& callback) {
    input_callbacks_[key] = callback;
}

void Menu::SelectUIElement(UIElement* target) {
    focused_element_ = target;
    if(option_node_[0]) focused_element_->node()->AddChild(option_node_[0]);
    if(option_node_[1]) focused_element_->node()->AddChild(option_node_[1]);
    PositionSelectionDrawables();
}

void Menu::FocusNextElement(int offset) {
    auto current_element = std::find(uielements_.begin(), uielements_.end(), focused_element_);

    if(current_element == uielements_.end())
        current_element = uielements_.begin();

    for(; offset > 0; --offset)
        if(++current_element == uielements_.end())
            current_element = uielements_.begin();
    for(; offset < 0; ++offset) {
        if(current_element == uielements_.begin())
            current_element = uielements_.end();
        --current_element;
    }

    SelectUIElement(*current_element);
}

void Menu::PositionSelectionDrawables() {
    double focus_x = focused_element_ ? focused_element_->node()->drawable()->hotspot().x : 0.0,
        size_x = focused_element_ ? focused_element_->node()->drawable()->size().x : 0.0;
    if(option_node_[0]) {
        double draw0_x = option_node_[0]->drawable()->hotspot().x;
        option_node_[0]->geometry().ChangeOffset(ugdk::math::Vector2D(-focus_x - draw0_x, 0.0));
    }
    if(option_node_[1]) {
        double draw1_x = option_node_[1]->drawable()->hotspot().x;
        option_node_[1]->geometry().ChangeOffset(ugdk::math::Vector2D(size_x - focus_x + draw1_x, 0.0));
    }
}

std::shared_ptr< std::vector<UIElement *> > Menu::GetMouseCollision() {
    math::Vector2D mouse_pos = input::manager()->mouse().position();
    std::array<double, 2> min_coords, max_coords;
    min_coords[0] = mouse_pos.x - 0.5;
    min_coords[1] = mouse_pos.y - 0.5;
    max_coords[0] = min_coords[0] + 1;
    max_coords[1] = min_coords[1] + 1;
    ugdk::structure::Box<2> box(min_coords, max_coords);

    auto results = std::make_shared< std::vector<UIElement *> >();
    objects_tree_->FindIntersectingItems(box, std::back_inserter(*results));
    return results;
}

void Menu::InteractWithFocused() { 
    if(focused_element_)
        focused_element_->Interact();
}

void Menu::CheckInteraction(const ugdk::math::Vector2D &mouse_pos) {
    auto intersecting_uielements = GetMouseCollision();
    if(intersecting_uielements->size() > 0)
        (*intersecting_uielements)[0]->Interact();
}

void Menu::AddObject(UIElement *obj) {
    obj->set_owner(this);
    obj->node()->drawable()->set_hotspot(hook_);
    objects_tree_->Insert(obj->GetBoundingBox(), obj);
    node_->AddChild(obj->node());
    uielements_.push_back(obj);
    if(!focused_element_) SelectUIElement(obj);
}

void Menu::RemoveObject(UIElement *obj) { 
    obj->set_owner(nullptr);
    objects_tree_->Remove(obj);
    node_->RemoveChild(obj->node());
    uielements_.remove(obj);
}

void Menu::RefreshObject(UIElement *obj) {
    objects_tree_->Update(obj->GetBoundingBox(), obj);
}

} // namespace ui
} // namespace ugdk
