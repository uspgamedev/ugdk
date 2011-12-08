#ifndef UGDK_GRAPHIC_NODE_H_
#define UGDK_GRAPHIC_NODE_H_

#include <list>
#include <ugdk/graphic/modifier.h>
#include <ugdk/graphic/drawable.h>

namespace ugdk {

class Node {
  public:
    Node(Drawable* drawable, const Node* parent = NULL) 
        :   modifier_(new Modifier), 
            drawable_(drawable),
            visible_(true),
            parent_(parent) {}

    ~Node() { if(modifier_) delete modifier_; }

    bool Render();

    const Modifier* modifier() const { return modifier_; }
    Modifier* modifier() { return modifier_; }

    void set_visible(const bool visible) { visible_ = visible; }
    bool visible() const { return visible_; }

    void set_parent(const Node* parent) { parent_ = parent; }
    const Node* parent() const { return parent_; }

    void AddChild(Node *child) { childs_.push_back(child); }
    void RemoveChild(Node *child) { childs_.remove(child); }

  private:
    Modifier* modifier_;
    Drawable* drawable_;
    bool visible_;

    const Node* parent_;
    std::list<Node*> childs_;
};

}  // namespace ugdk

#endif // UGDK_GRAPHIC_NODE_H_
