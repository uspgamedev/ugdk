
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

namespace {
    struct VertexXYUV {
        GLfloat x, y, u, v;
    };
}

void PrepareVertexDataAsRectangle(VertexData& data, const math::Vector2D& size) {
    data.CheckSizes("PrepareVertexDataAsRectangle", 4, sizeof(VertexXYUV));

    VertexData::Mapper mapper(data);

    VertexXYUV* v1 = mapper.Get<VertexXYUV>(0);
    v1->x = 0.0f;
    v1->y = 0.0f;
    v1->u = 0.0f;
    v1->v = 0.0f;

    VertexXYUV* v2 = mapper.Get<VertexXYUV>(1);
    v2->x = 0.0f;
    v2->y = float(size.y);
    v2->u = 0.0f;
    v2->v = 1.0f;

    VertexXYUV* v3 = mapper.Get<VertexXYUV>(2);
    v3->x = float(size.x);
    v3->y = 0.0f;
    v3->u = 1.0f;
    v3->v = 0.0f;

    VertexXYUV* v4 = mapper.Get<VertexXYUV>(3);
    v4->x = float(size.x);
    v4->y = float(size.y);
    v4->u = 1.0f;
    v4->v = 1.0f;
}

void RenderPrimitiveAsRectangle(const Primitive& primitive, opengl::ShaderUse& shader_use) {
    auto data = primitive.vertexdata();
    shader_use.SendVertexBuffer(data->buffer().get(), opengl::VERTEX , 0                , 2, data->vertex_size());
    shader_use.SendVertexBuffer(data->buffer().get(), opengl::TEXTURE, 2*sizeof(GLfloat), 2, data->vertex_size());
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

std::shared_ptr<VertexData> CreateRectangleCompatibleVertexData() {
    return std::make_shared<VertexData>(4, sizeof(VertexXYUV), false);
}

void PreparePrimitiveForRectangle(Primitive& primitive, const Texture* texture, const math::Vector2D& size) {
    primitive.set_texture(texture);
    primitive.set_vertexdata(CreateRectangleCompatibleVertexData());
    primitive.set_drawfunction(ugdk::graphic::opengl::RenderPrimitiveAsRectangle);
    PrepareVertexDataAsRectangle(*primitive.vertexdata(), size);
}

void PreparePrimitiveForRectangle(Primitive& p, const Texture* texture) {
    PreparePrimitiveForRectangle(p, texture, math::Vector2D(texture->width(), texture->height()));
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
