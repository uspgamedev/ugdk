#ifndef UGDK_GRAPHIC_FRAMEBUFFER_H_
#define UGDK_GRAPHIC_FRAMEBUFFER_H_

#include <ugdk/math/vector2D.h>
#include <ugdk/structure/types.h>
#include <ugdk/system/compatibility.h>

#include <ugdk/internal.h>
#include <ugdk/graphic.h>

#include <memory>

namespace ugdk {
namespace graphic {

class Framebuffer : public std::enable_shared_from_this<Framebuffer> {
  public:
    static std::shared_ptr<Framebuffer> Create(const math::Integer2D& size);

    Framebuffer(const math::Integer2D& size);
    ~Framebuffer();

    const math::Vector2D& size() const;
    internal::GLTexture* texture() const { return texture_.get(); }

    /** Can only be resized when not bound. */
    void Resize(const math::Vector2D& size);

    void Clear();
    void Clear(Color);
    void Bind();
    void Unbind();
    
  private:
    unsigned int gl_buffer_;
    std::unique_ptr<internal::GLTexture> texture_;
    bool is_bound_;
};

}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_CANVAS_H_
