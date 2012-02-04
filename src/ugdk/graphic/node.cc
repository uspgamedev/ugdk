#include <algorithm>
#include <cfloat>
#include <cmath>
#include <ugdk/base/engine.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/modifier.h>
#include <ugdk/graphic/drawable.h>
#include <ugdk/graphic/light.h>
#include "node.h"

namespace ugdk {
namespace graphic {

bool Node::CompareByZIndex(const Node *a, const Node *b) {
    return a->zindex() < b->zindex();
}

Node::~Node() {
    if(modifier_) delete modifier_;
    if(drawable_) delete drawable_;
    if(light_)    delete light_;

    if(parent_) parent_->RemoveChild(this);

    NodeSet::iterator it;
    for(it = childs_.begin(); it != childs_.end(); ++it) {
        (*it)->parent_ = NULL;
        delete *it;
    }
}

void Node::Render(float dt) {
    if(!active_) return;
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
    if(!active_) return;
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
    if(parent_ && (fabs(zindex - zindex_) > FLT_EPSILON)) parent_->must_sort_ = true;
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
    // TODO: use a better sorting algorith? std::sort usually may be 
    std::sort(childs_.begin(), childs_.end(), Node::CompareByZIndex);
    must_sort_ = false;
}
    
}  // namespace graphic
}  // namespace ugdk
