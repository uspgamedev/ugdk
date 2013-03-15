#include <GL/glew.h>
#define NO_SDL_GLEXT
#include <ugdk/graphic/drawable/texturedrectangle.h>

#include <ugdk/base/engine.h>
#include <ugdk/graphic/texture.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/opengl/shaderprogram.h>
#include <ugdk/graphic/opengl/vertexbuffer.h>
#include <ugdk/math/integer2D.h>

namespace ugdk {
namespace graphic {

TexturedRectangle::TexturedRectangle(Texture* texture) : size_(math::Integer2D(texture->width(), texture->height())), texture_(texture) {
    vertexbuffer_ = createBuffer();
    uvbuffer_ = createBuffer();
}

TexturedRectangle::TexturedRectangle(Texture* texture, const math::Vector2D& _size) : size_(_size), texture_(texture) {
    vertexbuffer_ = createBuffer();
    uvbuffer_ = createBuffer();
}

TexturedRectangle::~TexturedRectangle() {
    delete vertexbuffer_;
    delete uvbuffer_;
}

void TexturedRectangle::Draw(const Geometry& geometry, const VisualEffect&) const {
    // Use our shader
    opengl::ShaderProgram::Use shader_use(VIDEO_MANAGER()->default_shader());

    // Send our transformation to the currently bound shader, 
    // in the "MVP" uniform
    shader_use.SendGeometry(geometry * Geometry(math::Vector2D(-hotspot_), size_));

    // Bind our texture in Texture Unit 0
    shader_use.SendTexture(0, texture_);

    // 1rst attribute buffer : vertices
    shader_use.SendVertexBuffer(vertexbuffer_, opengl::VERTEX, 0);

    // 2nd attribute buffer : UVs
    shader_use.SendVertexBuffer(uvbuffer_, opengl::TEXTURE, 0);

    // Draw the triangle !
    glDrawArrays(GL_QUADS, 0, 4); // 12*3 indices starting at 0 -> 12 triangles
}

opengl::VertexBuffer* TexturedRectangle::createBuffer() {
    static const GLfloat buffer_data[] = { 
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
    };
    opengl::VertexBuffer* buffer = opengl::VertexBuffer::Create(sizeof(buffer_data), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    {
        opengl::VertexBuffer::Bind bind(*buffer);
        opengl::VertexBuffer::Mapper mapper(*buffer);

        GLfloat *indices = static_cast<GLfloat*>(mapper.get());
        if (indices)
            memcpy(indices, buffer_data, sizeof(buffer_data));
    }
    return buffer;
}

}  // namespace graphic
}  // namespace ugdk
