#include <ugdk/base/engine.h>
#include <ugdk/graphic/videomanager.h>
#include "node.h"

namespace ugdk {

bool Node::Render() {
    if(modifier_) {
        VIDEO_MANAGER()->PushModifier();
        VIDEO_MANAGER()->ApplyModifier(modifier_);
    }
    if(drawable_)
        drawable_->Draw();
    std::list<Node*>::iterator it;
    for(it = childs_.begin(); it != childs_.end(); ++it)
        (*it)->Render();
    if(modifier_)
        VIDEO_MANAGER()->PopModifier();
    return true;
}
    
}  // namespace ugdk
