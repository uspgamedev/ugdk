#include <vector>
#include "menu.h"
#include <ugdk/util/intervalkdtree.h>

Menu::Menu(const ugdk::ikdtree::Box<2>& tree_bounding_box) 
  : objects_tree_(new ObjectTree(tree_bounding_box,5)) {}

Menu::~Menu() {delete objects_tree_;}

void Menu::CheckInteraction(ugdk::Vector2D &mouse_pos) {
    std::vector<const UIElement *> *intersecting_uielements
        = objects_tree_->getIntersectingItems( ugdk::ikdtree::Box<2> (mouse_pos.val, mouse_pos.val) );
    for(std::vector<const UIElement *>::const_iterator it
           = intersecting_uielements->begin(); it != intersecting_uielements->end(); ++it) {
        (*it)->Interact();
    }
}

void Menu::AddObject(const UIElement *obj) {
    objects_tree_->Insert(obj->GetBoundingBox(), obj);
    node_->AddChild(obj->node());
}

void Menu::RemoveObject(const UIElement *obj) { 
    objects_tree_->Remove(obj);
    node_->RemoveChild(obj->node());
}

void Menu::RefreshObject(const UIElement *obj) {
    objects_tree_->Update(obj->GetBoundingBox(), obj);
}

