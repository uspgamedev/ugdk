#include "node.h"

#include <ugdk/internal/opengl.h>
#include <ugdk/graphic/manager.h>
#include <ugdk/graphic/geometry.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/ui/drawable.h>

#include <algorithm>
#include <cfloat>
#include <cmath>

namespace ugdk {
namespace ui {

bool Node::CompareByZIndex(const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) {
    return a->zindex() < b->zindex();
}

Node::Node()
: Node(nullptr) {}

Node::Node(std::unique_ptr<Drawable>&& _drawable)
:  drawable_(std::move(_drawable))
,  active_(true)
,  ignores_effect_(false)
,  zindex_(0.0)
,  parent_(nullptr)
,  must_sort_(false)
{}

Node::~Node() {
    if(parent_)
        parent_->RemoveChild(this);
}

void Node::Render(graphic::Canvas& canvas) const {
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

    for(const auto& child : childs_)
        child->Render(canvas);
        
    canvas.PopGeometry();
    if(!ignores_effect_)
        canvas.PopVisualEffect();
}

void Node::set_drawable(std::unique_ptr<Drawable>&& _drawable) {
    drawable_ = std::move(_drawable);
}

void Node::set_zindex(const double zindex) {
    if(parent_ && (fabs(zindex - zindex_) > FLT_EPSILON))
        parent_->must_sort_ = true;
    zindex_ = zindex;
}

void Node::AddChild(const std::shared_ptr<Node>& new_child) {
    if (new_child->parent_)
        new_child->parent_->RemoveChild(new_child.get());
    childs_.push_back(new_child);
    new_child->parent_ = this;
    must_sort_ = true;
}

void Node::RemoveChild(Node* child) {
    assert(child->parent_ == this);
    auto it = std::find_if(childs_.begin(), childs_.end(), [child](const std::shared_ptr<Node>& ptr) {
        return ptr.get() == child;
    });
    if(it != childs_.end())
        childs_.erase(it);
    child->parent_ = nullptr;
}

void Node::SortChildren() { 
    std::sort(childs_.begin(), childs_.end(), Node::CompareByZIndex);
    must_sort_ = false;
}
    
}  // namespace ui
}  // namespace ugdk
