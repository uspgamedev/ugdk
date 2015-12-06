#ifndef UGDK_GRAPHIC_RENDERTARGET_H_
#define UGDK_GRAPHIC_RENDERTARGET_H_

#include <ugdk/graphic.h>
#include <ugdk/math/geometry.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/structure/color.h>

namespace ugdk {
namespace graphic {

class RenderTarget {
  public:
    virtual ~RenderTarget();

    bool IsActive() const;
    const math::Geometry& projection_matrix() const;
    virtual math::Vector2D size() const = 0;

    /** Pre-requisite: must be active. */
    void Clear(structure::Color);

  protected:
    RenderTarget();
    virtual void Bind();
    virtual void Unbind();

    math::Geometry projection_matrix_;

  private:
    virtual void UpdateViewport() = 0;

    bool is_bound_;

    friend class ::ugdk::graphic::Canvas;
};

} // namespace graphic
} // namespace ugdk

#endif // UGDK_GRAPHIC_RENDERTARGET_H_
