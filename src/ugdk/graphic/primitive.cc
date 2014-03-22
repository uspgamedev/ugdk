#include <ugdk/graphic/primitive.h>

#include <ugdk/graphic/vertexdata.h>
#include <ugdk/graphic/opengl/shaderuse.h>

namespace ugdk {
namespace graphic {

Primitive::Primitive(const Texture* texture, const std::shared_ptr<VertexData>& data)
    :   texture_(texture)
    ,   vertexdata_(data)
{}

void Primitive::Draw(opengl::ShaderUse& shader_use) const {
    shader_use.SendTexture(0, texture_);
    drawfunction_(*this, shader_use);
}
    
}  // namespace graphic
}  // namespace ugdk
