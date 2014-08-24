#ifndef UGDK_GRAPHIC_RENDERTARGET_H
#define UGDK_GRAPHIC_RENDERTARGET_H

#include <ugdk/graphic.h>
#include <ugdk/graphic/geometry.h>
#include <ugdk/math/vector2D.h>

namespace ugdk {
namespace graphic {

class RenderTarget {
  public:
    virtual ~RenderTarget();

    bool IsActive() const;
    const Geometry& projection_matrix() const;
    virtual math::Vector2D size() const = 0;

    /** Pre-requisite: must be active. */
    void Clear(Color);

  protected:
    RenderTarget();
    virtual void Bind();
    virtual void Unbind();

    Geometry projection_matrix_;

  private:
    bool is_bound_;

    friend class ::ugdk::graphic::Canvas;
};

} // namespace graphic
} // namespace ugdk

#endif // UGDK_GRAPHIC_RENDERTARGET_H
