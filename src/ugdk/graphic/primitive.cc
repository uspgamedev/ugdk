#include <ugdk/graphic/primitive.h>

#include <ugdk/graphic/opengl/shaderprogram.h>

namespace ugdk {
namespace graphic {

Primitive::Primitive(const Texture* texture, const std::shared_ptr<const VertexData>& data)
    :   texture_(texture)
    ,   vertexdata_(data)
{}

void Primitive::Draw(opengl::ShaderUse& use) const {
    
}
    
}  // namespace graphic
}  // namespace ugdk
