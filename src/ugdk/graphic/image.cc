#include <ugdk/graphic/image.h>

#include <ugdk/internal/gltexture.h>

namespace ugdk {
namespace graphic {

int Image::width() const {
    return gltexture_->width();
}

int Image::height() const {
    return gltexture_->height();
}

}  // namespace graphic
}  // namespace ugdk
