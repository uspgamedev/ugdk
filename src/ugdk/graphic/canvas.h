#ifndef UGDK_GRAPHIC_CANVAS_H_
#define UGDK_GRAPHIC_CANVAS_H_

#include <ugdk/graphic/geometry.h>
#include <ugdk/graphic/visualeffect.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/structure/types.h>

#include <ugdk/internal.h>
#include <ugdk/desktop.h>
#include <ugdk/graphic.h>

#include <memory>
#include <vector>

typedef void* SDL_GLContext;

namespace ugdk {
namespace graphic {

class Canvas : public std::enable_shared_from_this<Canvas> {
  public:
    static std::shared_ptr<Canvas> Create(const std::weak_ptr<desktop::Window>&, const math::Vector2D& size);
    ~Canvas();

    const math::Vector2D& size() const { return size_; }
    const Geometry& current_geometry() const { return geometry_stack_.back(); }
    const VisualEffect& current_visualeffect() const { return visualeffect_stack_.back(); }

    /** Can only be resized when the geometry stack stacks is empty. */
    void Resize(const math::Vector2D& size);

    void AttachTo(const std::weak_ptr<desktop::Window>&);
    void UpdateViewport();

    void SaveToTexture(internal::GLTexture* texture);

    void PushAndCompose(const Geometry& geometry);
    void PushAndCompose(const VisualEffect& effect);

    void PopGeometry();
    void PopVisualEffect();

    void Clear();
    void Clear(Color);

  protected:
    Canvas(SDL_GLContext);
    
  private:
    SDL_GLContext context_;
    math::Vector2D size_;
    std::weak_ptr<desktop::Window> attached_window_;
    std::vector<Geometry> geometry_stack_;
    std::vector<VisualEffect> visualeffect_stack_;
};

}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_CANVAS_H_
