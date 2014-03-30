
#include <ugdk/graphic/opengl/vertexdata_rectangle.h>

#include <ugdk/graphic/vertexdata.h>
#include <ugdk/graphic/primitive.h>
#include <ugdk/graphic/texture.h>
#include <ugdk/graphic/opengl/vertexbuffer.h>
#include <ugdk/graphic/opengl/shaderuse.h>
#include <ugdk/graphic/opengl/Exception.h>
#include <ugdk/math/vector2D.h>

#include <cstring>

namespace ugdk {
namespace graphic {
namespace opengl {

void PrepareVertexDataAsRectangle(VertexData& data, const math::Vector2D& size) {
    data.CheckSizes("PrepareVertexDataAsRectangle", 4, 2 * 2 * sizeof(GLfloat));

    opengl::VertexBuffer::Bind bind(*data.buffer());
    opengl::VertexBuffer::Mapper mapper(*data.buffer());

    uint8* ptr = static_cast<uint8*>(mapper.get());

    GLfloat* v1 = reinterpret_cast<GLfloat*>(ptr + 0 * data.vertex_size());
    v1[0] = 0.0f;
    v1[1] = 0.0f;
    v1[2] = 0.0f;
    v1[3] = 0.0f;

    GLfloat* v2 = reinterpret_cast<GLfloat*>(ptr + 1 * data.vertex_size());
    v2[0] = 0.0f;
    v2[1] = float(size.y);
    v2[2] = 0.0f;
    v2[3] = 1.0f;
    
    GLfloat* v3 = reinterpret_cast<GLfloat*>(ptr + 2 * data.vertex_size());
    v3[2 * 4 + 0] = float(size.x);
    v3[2 * 4 + 1] = 0.0f;
    v3[2 * 4 + 2] = 1.0f;
    v3[2 * 4 + 3] = 0.0f;
    
    GLfloat* v4 = reinterpret_cast<GLfloat*>(ptr + 3 * data.vertex_size());
    v4[0] = float(size.x);
    v4[1] = float(size.y);
    v4[2] = 1.0f;
    v4[3] = 1.0f;
}

void RenderPrimitiveAsRectangle(const Primitive& primitive, opengl::ShaderUse& shader_use) {
    auto data = primitive.vertexdata();
    shader_use.SendVertexBuffer(data->buffer().get(), opengl::VERTEX , 0                , 2, data->vertex_size());
    shader_use.SendVertexBuffer(data->buffer().get(), opengl::TEXTURE, 2*sizeof(GLfloat), 2, data->vertex_size());
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

std::shared_ptr<Primitive> CreateTexturedRectanglePrimitive(const Texture* texture, const math::Vector2D& size) {

    std::shared_ptr<VertexData> data(new VertexData(4, 2 * 2 * sizeof(GLfloat), false));
    PrepareVertexDataAsRectangle(*data, size);

    std::shared_ptr<Primitive> primitive(new Primitive(texture, data));
    primitive->set_drawfunction(RenderPrimitiveAsRectangle);

    return primitive;
}

std::shared_ptr<Primitive> CreateTexturedRectanglePrimitive(const Texture* texture) {
    return CreateTexturedRectanglePrimitive(texture, math::Vector2D(texture->width(), texture->height()));
}

}  // namespace opengl
}  // namespace graphic
}  // namespace ugdk
