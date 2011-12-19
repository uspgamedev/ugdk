#ifndef UGDK_GRAPHIC_NODE_H_
#define UGDK_GRAPHIC_NODE_H_

#include <list>
#include <ugdk/graphic/modifier.h>
#include <ugdk/graphic/drawable.h>
#include <ugdk/graphic/light.h>

namespace ugdk {

class Node {
  public:
    Node(Drawable* drawable = NULL, Modifier* modifier = new Modifier) 
        :   modifier_(modifier), 
            drawable_(drawable),
            light_(NULL),
            visible_(true) {}

    ~Node();

    void Render();
    void RenderLight();

    const Modifier* modifier() const { return modifier_; }
    Modifier* modifier() { return modifier_; }

    void set_light(Light* light) { light_ = light; }

    void set_visible(const bool visible) { visible_ = visible; }
    bool visible() const { return visible_; }

    void set_zindex(const float zindex) { zindex_ = zindex; }
    float zindex() const { return zindex_; }

    void AddChild(Node *child) { childs_.push_back(child); }
    void RemoveChild(Node *child) { childs_.remove(child); }


    static bool CompareByZIndex(Node *a, Node *b) { return a->zindex() < b->zindex(); }
    void SortChildren();

  private:
    Modifier* modifier_;
    Drawable* drawable_;
    Light* light_;
    bool visible_;
    float zindex_;

    std::list<Node*> childs_;
};

}  // namespace ugdk

#endif // UGDK_GRAPHIC_NODE_H_
