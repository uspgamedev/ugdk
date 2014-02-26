#ifndef UGDK_GRAPHIC_OPENGL_VERTEXDATARECTANGLE_H_
#define UGDK_GRAPHIC_OPENGL_VERTEXDATARECTANGLE_H_

#include <ugdk/graphic/vertexdata.h>

#include <ugdk/math.h>
#include <ugdk/system/compatibility.h>
#include <memory>

namespace ugdk {
namespace graphic {
namespace opengl {

class VertexDataRectangle : public VertexData {
  public:
    /// 1x1 square.
    VertexDataRectangle();

    explicit VertexDataRectangle(const math::Vector2D& size);

    ~VertexDataRectangle();

    virtual void Draw(opengl::ShaderUse&) const override;

private:
    std::shared_ptr<const VertexBuffer> position_, uv_;
};

}  // namespace opengl
}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_OPENGL_VERTEXDATARECTANGLE_H_
