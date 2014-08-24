#ifndef UGDK_GRAPHIC_RENDERTEXTURE_H_
#define UGDK_GRAPHIC_RENDERTEXTURE_H_

#include <ugdk/graphic/rendertarget.h>

#include <ugdk/internal.h>
#include <ugdk/graphic.h>
#include <ugdk/math.h>
#include <ugdk/system/compatibility.h>

#include <memory>

namespace ugdk {
namespace graphic {

class RenderTexture : public RenderTarget {
  public:
    RenderTexture(std::unique_ptr<internal::GLTexture>&& texture);
    RenderTexture(const math::Integer2D& size);
    ~RenderTexture();

    virtual math::Vector2D size() const override;
    internal::GLTexture* texture() const { return texture_.get(); }

    void set_projection_matrix(const Geometry&);

  protected:
    virtual void Bind() override;
    virtual void Unbind() override;

  private:
    virtual void UpdateViewport() override;

    unsigned int gl_buffer_;
    std::unique_ptr<internal::GLTexture> texture_;
};

}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_RENDERTEXTURE_H_
