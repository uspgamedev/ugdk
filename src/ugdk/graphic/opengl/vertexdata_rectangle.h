#ifndef UGDK_GRAPHIC_OPENGL_VERTEXDATARECTANGLE_H_
#define UGDK_GRAPHIC_OPENGL_VERTEXDATARECTANGLE_H_

#include <ugdk/graphic.h>
#include <ugdk/math.h>

namespace ugdk {
namespace graphic {
namespace opengl {

void PrepareVertexDataAsRectangle(VertexData& data, const math::Vector2D& size);
void RenderPrimitiveAsRectangle(const Primitive&, opengl::ShaderUse& shader_use);

}  // namespace opengl
}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_OPENGL_VERTEXDATARECTANGLE_H_
