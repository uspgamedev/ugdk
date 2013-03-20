#include <GL/glew.h>
#define NO_SDL_GLEXT
#include <ugdk/graphic/drawable/texturedrectangle.h>

//#define GLM_SWIZZLE 
#include <glm/glm.hpp>

#include <ugdk/base/engine.h>
#include <ugdk/graphic/texture.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/opengl/shaderprogram.h>
#include <ugdk/graphic/opengl/vertexbuffer.h>
#include <ugdk/math/integer2D.h>

namespace ugdk {
namespace graphic {
    
TexturedRectangle::TexturedRectangle(Texture* texture) : size_(math::Integer2D(texture->width(), texture->height())), texture_(texture) {
    vertexbuffer_ = opengl::VertexBuffer::CreateDefault();
    uvbuffer_ = opengl::VertexBuffer::CreateDefault();
}

TexturedRectangle::TexturedRectangle(Texture* texture, const math::Vector2D& _size) : size_(_size), texture_(texture) {
    vertexbuffer_ = opengl::VertexBuffer::CreateDefault();
    uvbuffer_ = opengl::VertexBuffer::CreateDefault();
}

TexturedRectangle::~TexturedRectangle() {
}

void TexturedRectangle::Draw(const Geometry& geometry, const VisualEffect& effect) const {
    Geometry final_geometry(geometry * Geometry(math::Vector2D(-hotspot_), size_));
    glm::mat4 mat = final_geometry.AsMat4();
    glm::vec4 left = mat * glm::vec4(0.0, 0.0, 0.0, 1.0), right = mat * glm::vec4(1.0, 1.0, 0.0, 1.0);

    if(left.x > 1 || left.y > 1 || right.x < -1 || right.y < -1)
        return;
    // Use our shader
    opengl::ShaderProgram::Use shader_use(VIDEO_MANAGER()->default_shader());

    // Send our transformation to the currently bound shader, 
    // in the "MVP" uniform
    shader_use.SendGeometry(mat);
    shader_use.SendEffect(effect);

    // Bind our texture in Texture Unit 0
    shader_use.SendTexture(0, texture_);

    // 1rst attribute buffer : vertices
    shader_use.SendVertexBuffer(vertexbuffer_, opengl::VERTEX, 0);

    // 2nd attribute buffer : UVs
    shader_use.SendVertexBuffer(uvbuffer_, opengl::TEXTURE, 0);

    // Draw the triangle !
    glDrawArrays(GL_QUADS, 0, 4); // 12*3 indices starting at 0 -> 12 triangles
}

}  // namespace graphic
}  // namespace ugdk
