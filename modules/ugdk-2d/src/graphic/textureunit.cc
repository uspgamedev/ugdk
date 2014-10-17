#include <ugdk/graphic/textureunit.h>

#include <ugdk/graphic/module.h>
#include <ugdk/graphic/exceptions.h>
#include <ugdk/internal/opengl.h>
#include <ugdk/internal/gltexture.h>

namespace ugdk {
namespace graphic {

TextureUnit::TextureUnit()
: id_(-1)
, texture_(nullptr)
{}

TextureUnit::TextureUnit(unsigned int id)
: id_(id)
, texture_(nullptr)
{}

TextureUnit::TextureUnit(TextureUnit&& other)
: id_(other.id_)
, texture_(other.texture_)
{
    other.id_ = -1;
    other.texture_ = nullptr;
}

TextureUnit::~TextureUnit() {
    if (id_ > -1)
        graphic::manager()->ReleaseTextureUnitID(id_);
}

void TextureUnit::BindTexture(const internal::GLTexture* texture) {
    system::AssertCondition<system::InvalidOperation>(id_ > -1, "Attempting to bind texture to invalid TextureUnit.");
    texture_ = texture;

    glActiveTexture(GL_TEXTURE0 + id_);
    glBindTexture(GL_TEXTURE_2D, texture_->id());
    internal::AssertNoOpenGLError();

}

}  // namespace graphic
}  // namespace ugdk
