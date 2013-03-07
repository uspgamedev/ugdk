#include "node.h"

#include <algorithm>
#include <cfloat>
#include <cmath>
#include "SDL_opengl.h"

#include <ugdk/base/engine.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/geometry.h>
#include <ugdk/graphic/drawable.h>
#include <ugdk/graphic/light.h>

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
        (*it)->parent_ = NULL;
        delete *it;
    }
}

void Node::Update(double dt) {
    if(!active_) return;
    if(childs_.empty() && !drawable_) return; // optimization!

    if(drawable_) drawable_->Update(dt);

    if(must_sort_) SortChildren();
    NodeSet::const_iterator it;
    for(it = childs_.begin(); it != childs_.end(); ++it)
        (*it)->Update(dt);
}

void Node::Render(const Geometry& parent, const VisualEffect& parent_effect) const {
    if(!active_) return;
    if(childs_.empty() && !drawable_) return; // optimization!

    VisualEffect compose_effect = (ignores_effect_) ? effect_ : (parent_effect * effect_);
    if(!compose_effect.visible())
        return;

    Geometry compose(parent);
    compose.Compose(geometry_);

    if(drawable_)
        drawable_->Draw(compose, compose_effect);

    NodeSet::const_iterator it;
    for(it = childs_.begin(); it != childs_.end(); ++it)
        (*it)->Render(compose, compose_effect);
}

void Node::RenderLight(const Geometry& parent, const VisualEffect& parent_effect) const {
    if(!active_) return;
    if(childs_.empty() && !light_) return; // optimization!

    VisualEffect compose_effect = (ignores_effect_) ? effect_ : (parent_effect * effect_);
    if(!compose_effect.visible())
        return;
    
    Geometry compose(parent);
    compose.Compose(geometry_);

    if(light_) 
        light_->Draw(compose);

    NodeSet::const_iterator it;
    for(it = childs_.begin(); it != childs_.end(); ++it)
        (*it)->RenderLight(compose, compose_effect);
}

void Node::set_zindex(const double zindex) {
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
