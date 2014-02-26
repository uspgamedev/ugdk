#ifndef UGDK_GRAPHIC_PRIMITIVE_H_
#define UGDK_GRAPHIC_PRIMITIVE_H_

#include <ugdk/graphic.h>

#include <memory>

namespace ugdk {
namespace graphic {

class Primitive {
  public:
    Primitive(const Texture* texture, const std::shared_ptr<const VertexData>& data);
    ~Primitive() {}

    void Draw(opengl::ShaderUse&) const;
    
    const Texture* texture() const { return texture_; }
    std::shared_ptr<const VertexData> vertexdata() const { return vertexdata_; }
    
    void set_texture(const Texture* texture) { texture_ = texture; }
    void set_vertexdata(const std::shared_ptr<const VertexData>& data) { vertexdata_ = data; }

  protected:
    const Texture* texture_;
    std::shared_ptr<const VertexData> vertexdata_;
};

}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_PRIMITIVE_H_
