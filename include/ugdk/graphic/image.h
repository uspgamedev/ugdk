#ifndef UGDK_GRAPHIC_IMAGE_H_
#define UGDK_GRAPHIC_IMAGE_H_

#include <ugdk/internal.h>

#include <string>

namespace ugdk {
namespace graphic {

class Image {
  public:
    Image(const internal::GLTexture* texture)
      : gltexture_(texture) {}
    ~Image() {}

    const internal::GLTexture* gltexture() const { return gltexture_; }
    int width() const;
    int height() const;

  private:
    const internal::GLTexture* gltexture_;
};

}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_IMAGE_H_
