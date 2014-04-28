#ifndef UGDK_GRAPHIC_DRAWABLE_TEXTUREDRECTANGLE_H_
#define UGDK_GRAPHIC_DRAWABLE_TEXTUREDRECTANGLE_H_

#include <ugdk/internal.h>
#include <ugdk/graphic.h>
#include <ugdk/graphic/drawable.h>

namespace ugdk {
namespace graphic {

class TexturedRectangle : public Drawable {
  public:
    TexturedRectangle(const internal::GLTexture* texture);
    TexturedRectangle(const internal::GLTexture* texture, const math::Vector2D& _size);
    ~TexturedRectangle();

    void Draw(Canvas& canvas) const override;

    const ugdk::math::Vector2D& size() const override {
        return size_;
    }

  private:
    ugdk::math::Vector2D size_;
    const internal::GLTexture* texture_;
};

}  // namespace graphic
}  // namespace ugdk

#endif
