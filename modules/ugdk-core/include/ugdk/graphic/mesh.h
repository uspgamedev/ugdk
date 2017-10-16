
#ifndef UGDK_GRAPHIC_MESH_H_
#define UGDK_GRAPHIC_MESH_H_

#include <ugdk/graphic/vertexdata.h>

#include <glm/vec2.hpp>

#include <vector>

namespace ugdk {
namespace graphic {

class Canvas;

// TODO
// Make VertexData::Mapper template, delegating bind/unbind to VertexData

template <typename Position, typename Texel>
class Mesh {
  public:
    struct Vertex {
      Position  pos;
      Texel     tex;
    };
    Mesh(const DrawMode& the_mode, const std::vector<Vertex>& the_vtx_data);
    Mesh(const Mesh& rhs) = delete;
    Mesh& operator=(const Mesh& rhs) = delete;
    Mesh(Mesh&& rhs);
    Mesh& operator=(Mesh&& rhs);
    
  private:
    friend Canvas& operator<<(Canvas &canvas, const Mesh<Position, Texel> &mesh);
    DrawMode    mode_;
    VertexData  vtx_data_;
};

using MeshXYUV = Mesh<glm::vec2, glm::vec2>;

template <typename Position, typename Texel>
Canvas& operator<<(Canvas &canvas, const Mesh<Position, Texel> &mesh);

} // namespace ugdk
} // namespace graphic

#endif // UGDK_GRAPHIC_MESH_H_
