#ifndef UGDK_GRAPHIC_PRIMITIVE_H_
#define UGDK_GRAPHIC_PRIMITIVE_H_

#include <ugdk/graphic.h>

#include <memory>
#include <functional>

namespace ugdk {
namespace graphic {

class Primitive {
  public:
    Primitive(const Texture* texture, const std::shared_ptr<VertexData>& data);
    ~Primitive() {}

    void Draw(opengl::ShaderUse&) const;
    
    const Texture* texture() const { return texture_; }
    std::shared_ptr<VertexData> vertexdata() { return vertexdata_; }
    std::shared_ptr<const VertexData> vertexdata() const { return vertexdata_; }
    
    void set_texture(const Texture* texture) { texture_ = texture; }
    void set_vertexdata(const std::shared_ptr<VertexData>& data) { vertexdata_ = data; }
    void set_drawfunction(const std::function<void(const Primitive&, opengl::ShaderUse&)>& function) { drawfunction_ = function; }

  protected:
    const Texture* texture_;
    std::shared_ptr<VertexData> vertexdata_;
    std::function<void(const Primitive&, opengl::ShaderUse&)> drawfunction_;
};

}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_PRIMITIVE_H_
