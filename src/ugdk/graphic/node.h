#ifndef UGDK_GRAPHIC_NODE_H_
#define UGDK_GRAPHIC_NODE_H_

#include <vector>
#include <ugdk/graphic/modifier.h>
#include <ugdk/graphic/drawable.h>
#include <ugdk/graphic/light.h>

namespace ugdk {

class Node {
  public:
    //typedef std::set<Node*, bool(*)(const Node*, const Node*)> NodeSet;
    typedef std::vector<Node*> NodeSet;

    Node(Drawable* drawable = NULL, Modifier* modifier = new Modifier) 
        :   modifier_(modifier), 
            drawable_(drawable),
            light_(NULL),
            visible_(true),
            //childs_(CompareByZIndex),
            parent_(NULL) {}

    ~Node();

    void Render();
    void RenderLight();

    const Modifier* modifier() const { return modifier_; }
    Modifier* modifier() { return modifier_; }

    void set_light(Light* light) { light_ = light; }

    void set_visible(const bool visible) { visible_ = visible; }
    bool visible() const { return visible_; }

    void set_zindex(const float zindex);
    float zindex() const { return zindex_; }

    void AddChild(Node *child) {
        printf("Inserting node %d into set of %d. Previously %d elements.", (int)child, (int)this, childs_.size());
        childs_.push_back(child);
        printf(" Size after: %d\n", childs_.size());
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
