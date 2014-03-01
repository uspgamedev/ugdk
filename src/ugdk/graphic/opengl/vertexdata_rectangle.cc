
#include <ugdk/graphic/opengl/vertexdata_rectangle.h>

#include <ugdk/graphic/opengl/vertexbuffer.h>
#include <ugdk/graphic/opengl/shaderuse.h>
#include <ugdk/math/vector2D.h>

#include <cstring>

namespace ugdk {
namespace graphic {
namespace opengl {

VertexDataRectangle::VertexDataRectangle()
    : position_(VertexBuffer::CreateDefaultShared())
    , uv_(VertexBuffer::CreateDefaultShared())
{}

VertexDataRectangle::VertexDataRectangle(const math::Vector2D& size)
    : uv_(VertexBuffer::CreateDefaultShared())
{
    GLfloat buffer_data[] = {
          0.0f,   0.0f,
          0.0f, float(size.y),
        float(size.x),   0.0f,
        float(size.x), float(size.y)
    };
    std::shared_ptr<VertexBuffer> ptr(opengl::VertexBuffer::Create(sizeof(buffer_data), GL_ARRAY_BUFFER, GL_STATIC_DRAW));
    position_ = ptr;
    {
        opengl::VertexBuffer::Bind bind(*ptr);
        opengl::VertexBuffer::Mapper mapper(*ptr);

        GLfloat *indices = static_cast<GLfloat*>(mapper.get());
        if (indices)
            memcpy(indices, buffer_data, sizeof(buffer_data));
    }
}

VertexDataRectangle::~VertexDataRectangle() {}

void VertexDataRectangle::Draw(opengl::ShaderUse& shader_use) const {
    shader_use.SendVertexBuffer(position_.get(), opengl::VERTEX, 0);
    shader_use.SendVertexBuffer(uv_.get(), opengl::TEXTURE, 0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

}  // namespace opengl
}  // namespace graphic
}  // namespace ugdk
