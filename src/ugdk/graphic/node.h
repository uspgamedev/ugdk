#ifndef UGDK_GRAPHIC_NODE_H_
#define UGDK_GRAPHIC_NODE_H_

#include <vector>
#include <ugdk/graphic.h>
#include <ugdk/graphic/geometry.h>
#include <ugdk/graphic/visualeffect.h>

namespace ugdk {
namespace graphic {

/// 
class Node {
  public:
    typedef std::vector<Node*> NodeSet;

    Node(Drawable* _drawable = nullptr) 
        :   drawable_(_drawable),
            light_(nullptr),
            active_(true),
            ignores_effect_(false),
            zindex_(0.0),
            parent_(nullptr),
            must_sort_(false) {}

    ~Node();

    /// Pushes the modifier to the Manager, renders 
    void Render(Canvas&) const;
    void RenderLight(Canvas&) const;

    void set_drawable(Drawable* _drawable)  { drawable_ = _drawable; }
    void set_light(Light* _light)           {    light_ =    _light; }
    void set_active(const bool _active)     {   active_ =   _active; }
    void set_ignores_effect(const bool _on) {   ignores_effect_ = _on; }
    void set_zindex(const double zindex);
    
          Geometry& geometry()          { return geometry_; }
    const Geometry& geometry()    const { return geometry_; }
          VisualEffect& effect()        { return effect_; }
    const VisualEffect& effect()  const { return effect_; }
          Drawable* drawable()          { return drawable_; }
    const Drawable* drawable()    const { return drawable_; }
          Light* light()                { return    light_; }
    const Light* light()          const { return    light_; }
          bool   active()         const { return   active_; }
          bool   ignores_effect() const { return   ignores_effect_; }
          double zindex()         const { return   zindex_; }

    void AddChild(Node *new_child) {
        if(new_child->parent_) new_child->parent_->RemoveChild(new_child);
        childs_.push_back(new_child);
        new_child->parent_ = this;
        must_sort_ = true;
    }
    void RemoveChild(Node *child);


    static bool CompareByZIndex(const Node *a, const Node *b);
    void SortChildren();

  private:
    Geometry geometry_;
    VisualEffect effect_;
    Drawable* drawable_;
    Light* light_;
    bool active_;
    bool ignores_effect_;
    double zindex_;

    NodeSet childs_;
    Node* parent_;
    bool must_sort_;
};

}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_NODE_H_
