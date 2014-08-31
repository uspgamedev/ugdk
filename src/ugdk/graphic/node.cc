#include "node.h"

#include <algorithm>
#include <cfloat>
#include <cmath>

#include <ugdk/internal/opengl.h>
#include <ugdk/graphic/manager.h>
#include <ugdk/graphic/geometry.h>
#include <ugdk/graphic/drawable.h>
#include <ugdk/graphic/light.h>
#include <ugdk/graphic/canvas.h>

namespace ugdk {
namespace graphic {

bool Node::CompareByZIndex(const Node *a, const Node *b) {
    return a->zindex() < b->zindex();
}

Node::~Node() {
    if(drawable_) delete drawable_;
    if(light_)    delete light_;

    if(parent_) parent_->RemoveChild(this);

    NodeSet::iterator it;
    for(it = childs_.begin(); it != childs_.end(); ++it) {
        (*it)->parent_ = nullptr;
        delete *it;
    }
}

void Node::Render(Canvas& canvas) const {
    if(!active_) return;
    if(childs_.empty() && !drawable_) return; // optimization!

    if(must_sort_) // Lazyness usually breaks const-ness...
        const_cast<Node*>(this)->SortChildren();

    if(!ignores_effect_)
        canvas.PushAndCompose(effect_);
    
    if(!canvas.current_visualeffect().visible()) {
        if(!ignores_effect_)
            canvas.PopVisualEffect();
        return;
    }

    canvas.PushAndCompose(geometry_);

    if(drawable_)
        drawable_->Draw(canvas);

    for(Node* child : childs_)
        child->Render(canvas);
        
    canvas.PopGeometry();
    if(!ignores_effect_)
        canvas.PopVisualEffect();
}

void Node::RenderLight(Canvas& canvas) const {
    if(!active_) return;
    if(childs_.empty() && !light_) return; // optimization!

    if(!ignores_effect_)
        canvas.PushAndCompose(effect_);
    
    if(!canvas.current_visualeffect().visible()) {
        if(!ignores_effect_)
            canvas.PopVisualEffect();
        return;
    }
    
    canvas.PushAndCompose(geometry_);

    if(light_) 
        light_->Draw(canvas);

    for(Node* child : childs_)
        child->RenderLight(canvas);
        
    canvas.PopGeometry();
    if(!ignores_effect_)
        canvas.PopVisualEffect();
}

void Node::set_zindex(const double zindex) {
    if(parent_ && (fabs(zindex - zindex_) > FLT_EPSILON)) parent_->must_sort_ = true;
    zindex_ = zindex;
}

void Node::RemoveChild(Node *child) {
    auto it = std::find(childs_.begin(), childs_.end(), child);
    if(it != childs_.end())
        childs_.erase(it);
    child->parent_ = nullptr;
}

void Node::SortChildren() {
    // TODO: use a better sorting algorith? std::sort usually may be 
    std::sort(childs_.begin(), childs_.end(), Node::CompareByZIndex);
    must_sort_ = false;
}
    
}  // namespace graphic
}  // namespace ugdk
