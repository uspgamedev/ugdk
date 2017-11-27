#ifndef UGDK_GRAPHIC_RENDERTARGET_H_
#define UGDK_GRAPHIC_RENDERTARGET_H_

#include <ugdk/math/geometry.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/structure/color.h>
#include <ugdk/graphic.h>
#include <ugdk/graphic/renderer.h>
#include <ugdk/graphic/manager.h>
#include <ugdk/graphic/module.h>

namespace ugdk {
namespace graphic {

class RenderTarget {
  public:
    virtual ~RenderTarget();

    virtual bool IsActive() const;
    virtual bool IsValid() const;
    const math::Geometry& projection_matrix() const;
    
    virtual void Resize(const math::Vector2D&) = 0;
    void set_projection_matrix(const math::Geometry&);

    virtual math::Vector2D size() const = 0;
    virtual void Use() = 0;
    void Render();
    /** Pre-requisite: must be active. */
    void Clear(structure::Color);
  protected:

    RenderTarget();
    math::Geometry projection_matrix_;
    
    virtual void Bind();
    virtual void Unbind();

  private:
    friend class ::ugdk::graphic::Canvas;
    friend class ::ugdk::graphic::Manager;
    friend class ::ugdk::graphic::Renderer;

    virtual void UpdateViewport() = 0;
    
    bool is_bound_;
    Renderer my_renderer_;
    
};

} // namespace graphic
} // namespace ugdk

#endif // UGDK_GRAPHIC_RENDERTARGET_H_
