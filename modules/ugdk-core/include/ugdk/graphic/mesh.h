
#ifndef UGDK_GRAPHIC_MESH_H_
#define UGDK_GRAPHIC_MESH_H_

#include <ugdk/structure/vertex.h>

#include <glm/vec2.hpp>

#include <vector>

namespace ugdk {
namespace graphic {

class Canvas;

template <typename Vertex>
class Mesh {
  public:
    /// Alias
    using VertexArray = std::vector<Vertex>;

    /// Construct mesh with given draw mode
    Mesh(const DrawMode& the_mode);

    /// 
    void Fill(const VertexArray& vertices);
    
  private:
    friend Canvas& operator<<(Canvas &canvas, const Mesh<Position, Texel> &mesh);
    DrawMode            mode_;
    std::vector<Vertex> vtx_data_;
};

using Mesh2D = Mesh<structure::VertexXYUV>;

template <typename Position, typename Texel>
Canvas& operator<<(Canvas &canvas, const Mesh<Position, Texel> &mesh);

} // namespace ugdk
} // namespace graphic

#endif // UGDK_GRAPHIC_MESH_H_
