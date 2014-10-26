#include <ugdk/ui/drawable/texturedrectangle.h>

//#define GLM_SWIZZLE 
#include <glm/glm.hpp>

#include <ugdk/graphic/opengl.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/textureunit.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/primitivesetup.h>
#include <ugdk/math/integer2D.h>
#include "graphic/gltexture.h"

namespace ugdk {
namespace ui {

using namespace graphic;

namespace {
    struct VertexXYUV {
        GLfloat x, y, u, v;
    };
}
    
TexturedRectangle::TexturedRectangle(const graphic::GLTexture* texture)
    : TexturedRectangle(texture, math::Integer2D(texture->width(), texture->height()))
{}

TexturedRectangle::TexturedRectangle(const graphic::GLTexture* texture, const math::Vector2D& _size)
    : size_(_size)
    , data_(4, sizeof(VertexXYUV), false)
    , texture_(texture)
{
    assert(texture_);

    VertexDataManipulation::SetToRectangleAtOrigin(data_, size_);
}

TexturedRectangle::~TexturedRectangle() {}

void TexturedRectangle::Draw(graphic::Canvas& canvas) const {
    if(draw_setup_function_) draw_setup_function_(this, canvas);

    canvas.PushAndCompose(Geometry(math::Vector2D(-hotspot_)));

    TextureUnit unit = manager()->ReserveTextureUnit(texture_);
    canvas.SendUniform("drawable_texture", unit);

    canvas.SendVertexData(data_, VertexType::VERTEX, 0, 2);
    canvas.SendVertexData(data_, VertexType::TEXTURE, 2 * sizeof(GLfloat), 2);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    canvas.PopGeometry();
}

}  // namespace ui
}  // namespace ugdk
