#ifndef UGDK_GRAPHIC_NODE_H_
#define UGDK_GRAPHIC_NODE_H_

#include <list>
#include <ugdk/graphic/modifier.h>
#include <ugdk/graphic/drawable.h>
#include <ugdk/graphic/light.h>

namespace ugdk {

class Node {
  public:
    Node(Drawable* drawable, Light* light = NULL) 
        :   modifier_(new Modifier), 
            drawable_(drawable),
            light_(light),
            visible_(true) {}

    ~Node() { if(modifier_) delete modifier_; }

    void Render();
    void RenderLight();

    const Modifier* modifier() const { return modifier_; }
    Modifier* modifier() { return modifier_; }

    void set_visible(const bool visible) { visible_ = visible; }
    bool visible() const { return visible_; }

    void AddChild(Node *child) { childs_.push_back(child); }
    void RemoveChild(Node *child) { childs_.remove(child); }

  private:
    Modifier* modifier_;
    Drawable* drawable_;
    Light* light_;
    bool visible_;

    std::list<Node*> childs_;
};

}  // namespace ugdk

#endif // UGDK_GRAPHIC_NODE_H_
