#ifndef UGDK_GRAPHIC_RENDERTEXTURE_H_
#define UGDK_GRAPHIC_RENDERTEXTURE_H_

#include <ugdk/graphic.h>
#include <ugdk/graphic/rendertarget.h>

#include <ugdk/math.h>
#include <ugdk/system/compatibility.h>

#include <memory>

namespace ugdk {
namespace graphic {

class RenderTexture : public RenderTarget {
  public:
    RenderTexture(std::unique_ptr<graphic::GLTexture>&& texture);
    RenderTexture(const math::Integer2D& size);
    ~RenderTexture();

    bool IsValid() const override;

    math::Vector2D size() const;
    void Resize(const math::Vector2D&) override;
    graphic::GLTexture* texture() const { return texture_.get(); }

    void Use() override;
    void Present() override;
  protected:

  private:
    void Bind  () override;
    void Unbind() override;
    void UpdateViewport() override;

    UGDKGLuint *gl_buffer_ptr;
    std::unique_ptr<graphic::GLTexture> texture_;
};

}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_RENDERTEXTURE_H_
