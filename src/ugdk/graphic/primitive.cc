#include <ugdk/graphic/primitive.h>

#include <ugdk/graphic/vertexdata.h>
#include <ugdk/graphic/opengl/shaderuse.h>
#include <ugdk/graphic/primitivecontroller.h>

namespace ugdk {
namespace graphic {

Primitive::Primitive(const Texture* texture, const std::shared_ptr<VertexData>& data)
    : texture_(texture)
    , vertexdata_(data)
    , controller_(nullptr)
    , shader_program_(nullptr)
{}

Primitive::~Primitive() {}

void Primitive::set_controller(std::unique_ptr<PrimitiveControllerPosition>&& controller) {
    controller_ = std::move(controller);
    controller_->set_owner(this);
}

}  // namespace graphic
}  // namespace ugdk
