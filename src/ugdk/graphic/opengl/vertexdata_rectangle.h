#ifndef UGDK_GRAPHIC_OPENGL_VERTEXDATARECTANGLE_H_
#define UGDK_GRAPHIC_OPENGL_VERTEXDATARECTANGLE_H_

#include <ugdk/graphic.h>
#include <ugdk/math.h>
#include <memory>

namespace ugdk {
namespace graphic {
namespace opengl {

void PrepareVertexDataAsRectangle(VertexData& data, const math::Vector2D& size);
void RenderPrimitiveAsRectangle(const Primitive&, opengl::ShaderUse& shader_use);

std::shared_ptr<Primitive> CreateTexturedRectanglePrimitive(const Texture*, const math::Vector2D& size);
std::shared_ptr<Primitive> CreateTexturedRectanglePrimitive(const Texture*);

}  // namespace opengl
}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_OPENGL_VERTEXDATARECTANGLE_H_
