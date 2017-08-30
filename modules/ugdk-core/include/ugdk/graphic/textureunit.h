#ifndef UGDK_GRAPHIC_TEXTUREUNIT_H_
#define UGDK_GRAPHIC_TEXTUREUNIT_H_

#include <ugdk/graphic.h>
#include <ugdk/structure/types.h>

namespace ugdk {
namespace graphic {

class TextureUnit final {
  public:
    TextureUnit(const TextureUnit&) = delete;
    TextureUnit(TextureUnit&&);
    ~TextureUnit();

    void BindTexture(const graphic::GLTexture* texture);

    int id() const { return id_; }
    const graphic::GLTexture* texture() const { return texture_; }

  private:
    TextureUnit(unsigned int);

    int id_;
    const graphic::GLTexture* texture_;

    friend class ::ugdk::graphic::Manager;
};

} // namespace graphic
} // namespace ugdk

#endif // UGDK_GRAPHIC_TEXTUREUNIT_H_
