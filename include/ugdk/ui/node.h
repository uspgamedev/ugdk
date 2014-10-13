#ifndef UGDK_GRAPHIC_NODE_H_
#define UGDK_GRAPHIC_NODE_H_

#include <ugdk/graphic.h>
#include <ugdk/graphic/geometry.h>
#include <ugdk/graphic/visualeffect.h>
#include <ugdk/ui.h>

#include <vector>
#include <memory>

namespace ugdk {
namespace ui {

/// 
class Node {
  public:
    explicit Node();
    Node(std::unique_ptr<Drawable>&& _drawable);
    ~Node();

    /// Pushes the modifier to the Manager, renders 
    void Render(graphic::Canvas&) const;

    void set_drawable(std::unique_ptr<Drawable>&& _drawable);
    void set_active(const bool _active)     {   active_ =   _active; }
    void set_ignores_effect(const bool _on) {   ignores_effect_ = _on; }
    void set_zindex(const double zindex);
    
          graphic::Geometry& geometry()          { return geometry_; }
    const graphic::Geometry& geometry()    const { return geometry_; }
          graphic::VisualEffect& effect()        { return effect_; }
    const graphic::VisualEffect& effect()  const { return effect_; }
          Drawable* drawable()          { return drawable_.get(); }
    const Drawable* drawable()    const { return drawable_.get(); }
          bool   active()         const { return   active_; }
          bool   ignores_effect() const { return   ignores_effect_; }
          double zindex()         const { return   zindex_; }

    void AddChild(const std::shared_ptr<Node>& new_child);
    void RemoveChild(Node *child);

    static bool CompareByZIndex(const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b);
    void SortChildren();

  private:
    graphic::Geometry geometry_;
    graphic::VisualEffect effect_;
    std::unique_ptr<Drawable> drawable_;
    bool active_;
    bool ignores_effect_;
    double zindex_;

    std::vector<std::shared_ptr<Node>> childs_;
    Node* parent_;
    bool must_sort_;
};

}  // namespace ui
}  // namespace ugdk

#endif // UGDK_GRAPHIC_NODE_H_
