#ifndef UGDK_GRAPHIC_PRIMITIVE_H_
#define UGDK_GRAPHIC_PRIMITIVE_H_

#include <ugdk/internal.h>
#include <ugdk/graphic.h>

#include <ugdk/graphic/visualeffect.h>

#include <memory>
#include <functional>

namespace ugdk {
namespace graphic {

class Primitive {
  public:
    Primitive(const internal::GLTexture* texture, const std::shared_ptr<VertexData>& data);
    ~Primitive();

    const internal::GLTexture* texture() const { return texture_; }
    std::shared_ptr<VertexData> vertexdata() { return vertexdata_; }
    std::shared_ptr<const VertexData> vertexdata() const { return vertexdata_; }
    const std::function<void(const Primitive&, opengl::ShaderUse&)>& drawfunction() const { return drawfunction_; }
    std::unique_ptr<PrimitiveController>& controller() { return controller_; }
    const std::unique_ptr<PrimitiveController>& controller() const { return controller_; }
    const VisualEffect& visual_effect() const { return visual_effect_; }
    const opengl::ShaderProgram* shader_program() const { return shader_program_; }
    
    void set_texture(const internal::GLTexture* texture) { texture_ = texture; }
    void set_vertexdata(const std::shared_ptr<VertexData>& data) { vertexdata_ = data; }
    void set_drawfunction(const std::function<void(const Primitive&, opengl::ShaderUse&)>& function) { drawfunction_ = function; }
    void set_visualeffect(const VisualEffect& visual_effect) { visual_effect_ = visual_effect; }
    void set_shader_program(const opengl::ShaderProgram* shader_program) { shader_program_ = shader_program; }

#ifdef _MSC_VER
#if _MSC_VER <= 1800
    Primitive& operator=(Primitive&&);
#endif
#endif
#ifndef SWIG
    void set_controller(std::unique_ptr<PrimitiveController>&& controller);
#endif

  protected:
    const internal::GLTexture* texture_;
    std::shared_ptr<VertexData> vertexdata_;
    std::function<void(const Primitive&, opengl::ShaderUse&)> drawfunction_;
    std::unique_ptr<PrimitiveController> controller_;
    VisualEffect visual_effect_;
    const opengl::ShaderProgram* shader_program_;
};

}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_PRIMITIVE_H_