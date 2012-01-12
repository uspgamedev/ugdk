#include <algorithm>
#include <ugdk/base/engine.h>
#include <ugdk/graphic/videomanager.h>
#include "node.h"

namespace ugdk {

bool Node::CompareByZIndex(const Node *a, const Node *b) {
    return a->zindex() < b->zindex();
}

Node::~Node() {
    if(modifier_) delete modifier_;
    if(parent_) parent_->RemoveChild(this);

    NodeSet::iterator it;
    for(it = childs_.begin(); it != childs_.end(); ++it) {
        (*it)->parent_ = NULL;
        delete *it;
    }
}

void Node::Render(float dt) {
    if(!visible_) return;
    if(childs_.empty() && !drawable_) return; // optimization!

    Modifier* modifier = modifier_;
    if(modifier) VIDEO_MANAGER()->PushAndApplyModifier(modifier);

    if(drawable_) drawable_->Draw(dt);

    if(must_sort_) SortChildren();
    NodeSet::iterator it;
    for(it = childs_.begin(); it != childs_.end(); ++it)
        (*it)->Render(dt);
    
    if(modifier) VIDEO_MANAGER()->PopModifier();
}

void Node::RenderLight() {
    if(!visible_) return;
    if(childs_.empty() && !light_) return; // optimization!

    Modifier* modifier = modifier_;
    if(modifier) VIDEO_MANAGER()->PushAndApplyModifier(modifier);

    if(light_)
        light_->Draw();

    NodeSet::iterator it;
    for(it = childs_.begin(); it != childs_.end(); ++it)
        (*it)->RenderLight();
    
    if(modifier) VIDEO_MANAGER()->PopModifier();
}

void Node::set_zindex(const float zindex) {
    if(parent_ && (zindex - zindex_ > 1.0e-6)) parent_->must_sort_ = true;
    zindex_ = zindex;
}

void Node::RemoveChild(Node *child) {
    NodeSet::iterator it = childs_.begin();
    while(it != childs_.end() && *it != child) ++it;
    if(it != childs_.end())
        childs_.erase(it);
    child->parent_ = NULL;
}

void Node::SortChildren() {
    std::sort(childs_.begin(), childs_.end(), Node::CompareByZIndex);
    must_sort_ = false;
    //childs_.sort(Node::CompareByZIndex);
}
    
}  // namespace ugdk
