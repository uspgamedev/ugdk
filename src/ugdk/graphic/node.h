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

    Node(Drawable* drawable = NULL, Modifier* modifier = NULL) 
        :   modifier_(modifier), 
            drawable_(drawable),
            light_(NULL),
            visible_(true),
            zindex_(0.0f),
            parent_(NULL) {}

    ~Node();

    void Render(float dt);
    void RenderLight();

    void set_drawable(Drawable* drawable) { drawable_ = drawable; }
    void set_light(Light* light)          {    light_ =    light; }
    void set_visible(const bool visible)  {  visible_ =  visible; }
    void set_zindex(const float zindex);
    
          Modifier* modifier() { return (modifier_ != NULL) ? modifier_ : (modifier_ = new Modifier); }
    const Modifier* modifier() const { return modifier_; }
          Drawable* drawable()       { return drawable_; }
    const Drawable* drawable() const { return drawable_; }
          Light* light()             { return    light_; }
    const Light* light()       const { return    light_; }
          bool   visible()     const { return  visible_; }
          float  zindex()      const { return   zindex_; }

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
