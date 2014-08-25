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

namespace ugdk {
namespace graphic {

class Canvas {
  public:
    Canvas(RenderTarget* render_target);
    ~Canvas();

    void ChangeShaderProgram(const opengl::ShaderProgram* shader_program);

    bool IsActive() const;
    const Geometry& current_geometry() const { return geometry_stack_.back(); }
    const VisualEffect& current_visualeffect() const { return visualeffect_stack_.back(); }

    void PushAndCompose(const Geometry& geometry);
    void PushAndCompose(const VisualEffect& effect);

    void PopGeometry();
    void PopVisualEffect();

    void Clear(Color);

  private:
    void Bind();
    void Unbind();

    RenderTarget* render_target_;
    std::vector<Geometry> geometry_stack_;
    std::vector<VisualEffect> visualeffect_stack_;
    const opengl::ShaderProgram* shader_program_;
    Canvas* previous_canvas_;
    Canvas* next_canvas_;
};

}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_CANVAS_H_
