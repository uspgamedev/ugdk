#ifndef UGDK_GRAPHIC_VERTEXDATA_H_
#define UGDK_GRAPHIC_VERTEXDATA_H_

#include <ugdk/graphic.h>
#include <cstddef>
#include <memory>

namespace ugdk {
namespace graphic {

class VertexData {
  public:
    VertexData(std::size_t num_vertices, std::size_t vertex_size, bool dynamic);
    ~VertexData();

    const std::unique_ptr<opengl::VertexBuffer>& buffer() const { return buffer_; }
    std::size_t num_vertices() const { return num_vertices_; }
    std::size_t vertex_size() const { return vertex_size_; }

    void CheckSizes(const char* caller_name, std::size_t test_num_vertices, std::size_t test_vertex_size) const;

  private:
    std::unique_ptr<opengl::VertexBuffer> buffer_;
    std::size_t num_vertices_;
    std::size_t vertex_size_;
};

}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_VERTEXDATA_H_
