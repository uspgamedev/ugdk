
#ifndef UGDK_GRAPHIC_MESH_H_
#define UGDK_GRAPHIC_MESH_H_

#include <ugdk/structure/vertex.h>
#include <ugdk/graphic/vertexdata.h>
#include <ugdk/graphic.h>

#include <glm/vec2.hpp>

#include <vector>

namespace ugdk {
namespace graphic {

class Canvas;

// TODO:
// + [x] Add fill methods
// + [ ] Write stream method
// Remember:
// + VertexData will be used internally to group buffers
// + Mesh is used by user for easy access

template <typename Vertex>
class Mesh {
  public:
    /// Alias
    using VertexArray = std::vector<Vertex>;

    /// Construct mesh with given draw mode
    Mesh(const DrawMode& the_mode, GLTexture* the_texture);

    /// 
    void Fill(const VertexArray& vertices);
    
  private:
    template <typename T>
    friend Canvas& operator<<(Canvas &canvas, const Mesh<T> &mesh);
    DrawMode            mode_;
    GLTexture           *texture_;
    std::vector<Vertex> vtx_data_;
};

using Mesh2D = Mesh<structure::VertexXYUV>;

inline Mesh::Mesh(const DrawMode& the_mode, GLTexture* the_texture)
    : mode_(the_mode), texture_(the_texture) {}

inline void Mesh::Fill(const VertexArray& vertices) {
    vtx_data_ = vertices;
}

template <typename Vertex>
Canvas& operator<<(Canvas &canvas, const Mesh<Vertex> &mesh);

} // namespace ugdk
} // namespace graphic

#endif // UGDK_GRAPHIC_MESH_H_
