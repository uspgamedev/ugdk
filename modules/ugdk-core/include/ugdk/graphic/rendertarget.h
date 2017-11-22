#ifndef UGDK_GRAPHIC_RENDERTARGET_H_
#define UGDK_GRAPHIC_RENDERTARGET_H_

#include <ugdk/graphic.h>
#include <ugdk/math/geometry.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/structure/color.h>
#include <ugdk/graphic/manager.h>
#include <ugdk/graphic/module.h>

namespace ugdk {
namespace graphic {

class RenderTarget {
  public:
    virtual ~RenderTarget();

    bool IsActive() const;
    virtual bool IsValid() const;
    const math::Geometry& projection_matrix() const;

    virtual math::Vector2D size() const = 0;
    virtual void Use() = 0;

    /** Pre-requisite: must be active. */
    void Clear(structure::Color);
    
  protected:
    virtual void Bind();
    virtual void Unbind();
    
    RenderTarget();
    
    math::Geometry projection_matrix_;
    
  private:
    friend class ::ugdk::graphic::Canvas;
    friend class ::ugdk::graphic::Manager;

    virtual void UpdateViewport() = 0;
    
    bool is_bound_;
    
};

} // namespace graphic
} // namespace ugdk

#endif // UGDK_GRAPHIC_RENDERTARGET_H_
