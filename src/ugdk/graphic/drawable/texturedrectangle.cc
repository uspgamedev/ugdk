#include <GL/glew.h>
#define NO_SDL_GLEXT
#include <ugdk/graphic/drawable/texturedrectangle.h>

//#define GLM_SWIZZLE 
#include <glm/glm.hpp>

#include <ugdk/graphic/drawable/functions.h>
#include <ugdk/graphic/texture.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/opengl/shaderprogram.h>
#include <ugdk/graphic/opengl/vertexbuffer.h>
#include <ugdk/math/integer2D.h>

namespace ugdk {
namespace graphic {
    
TexturedRectangle::TexturedRectangle(Texture* texture) 
    : size_(math::Integer2D(texture->width(), texture->height()))
    , texture_(texture) {}

TexturedRectangle::TexturedRectangle(Texture* texture, const math::Vector2D& _size) 
    : size_(_size)
    , texture_(texture) {}

TexturedRectangle::~TexturedRectangle() {}

void TexturedRectangle::Draw(const Geometry& geometry, const VisualEffect& effect) const {
    Geometry final_geometry(geometry);
    final_geometry.Compose(Geometry(math::Vector2D(-hotspot_), size_));
    
    if(draw_setup_function_) draw_setup_function_(this, geometry, effect);

    DrawSquare(final_geometry, effect, texture_ ? texture_ : graphic::manager()->white_texture());
}

}  // namespace graphic
}  // namespace ugdk
