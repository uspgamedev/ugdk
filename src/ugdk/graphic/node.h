#ifndef UGDK_GRAPHIC_NODE_H_
#define UGDK_GRAPHIC_NODE_H_

#include <vector>
#include <ugdk/graphic/modifier.h>
#include <ugdk/graphic/drawable.h>
#include <ugdk/graphic/light.h>

namespace ugdk {

class Node {
  public:
    typedef std::vector<Node*> NodeSet;

    Node(Drawable* drawable = NULL, Modifier* modifier = new Modifier) 
        :   modifier_(modifier), 
            drawable_(drawable),
            light_(NULL),
            visible_(true),
            parent_(NULL) {}

    ~Node();

    void Render();
    void RenderLight();

    const Modifier* modifier() const { return modifier_; }
    Modifier* modifier() { return modifier_; }

    void set_drawable(Drawable* drawable) { drawable_ = drawable; }

    void set_light(Light* light) { light_ = light; }
    Light* light() { return light_; }
    const Light* light() const { return light_; }

    void set_visible(const bool visible) { visible_ = visible; }
    bool visible() const { return visible_; }

    void set_zindex(const float zindex);
    float zindex() const { return zindex_; }

    void AddChild(Node *child) {
        childs_.push_back(child);
        child->parent_ = this;
    }
    void RemoveChild(Node *child);


    static bool CompareByZIndex(const Node *a, const Node *b);
    void SortChildren();

  private:
    Modifier* modifier_;
    Drawable* drawable_;
    Light* light_;
    bool visible_;
    float zindex_;

    NodeSet childs_;
    Node* parent_;
    bool must_sort_;
};

}  // namespace ugdk

#endif // UGDK_GRAPHIC_NODE_H_
