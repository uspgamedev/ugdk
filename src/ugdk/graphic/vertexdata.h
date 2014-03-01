#ifndef UGDK_GRAPHIC_VERTEXDATA_H_
#define UGDK_GRAPHIC_VERTEXDATA_H_

#include <ugdk/graphic.h>

namespace ugdk {
namespace graphic {

class VertexData {
  public:
    VertexData() {}
    ~VertexData() {}

    virtual void Draw(opengl::ShaderUse&) const = 0;
};

}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_VERTEXDATA_H_
