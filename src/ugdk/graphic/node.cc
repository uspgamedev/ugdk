#include <ugdk/base/engine.h>
#include <ugdk/graphic/videomanager.h>
#include "node.h"

namespace ugdk {

Node::~Node() {
    if(modifier_) delete modifier_;
    std::list<Node*>::iterator it;
    for(it = childs_.begin(); it != childs_.end(); ++it)
        delete *it;
}

void Node::Render() {
    if(!visible_) return;

    if(modifier_) VIDEO_MANAGER()->PushAndApplyModifier(modifier_);

    if(drawable_) drawable_->Draw();

    std::list<Node*>::iterator it;
    for(it = childs_.begin(); it != childs_.end(); ++it)
        (*it)->Render();
    
    if(modifier_) VIDEO_MANAGER()->PopModifier();
}

void Node::RenderLight() {
    if(!visible_) return;

    if(modifier_) VIDEO_MANAGER()->PushAndApplyModifier(modifier_);

    Vector2D off;
    if(light_) light_->Render(off);

    std::list<Node*>::iterator it;
    for(it = childs_.begin(); it != childs_.end(); ++it)
        (*it)->RenderLight();
    
    if(modifier_) VIDEO_MANAGER()->PopModifier();
}
    
}  // namespace ugdk
