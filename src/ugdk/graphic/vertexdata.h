#ifndef UGDK_GRAPHIC_VERTEXDATA_H_
#define UGDK_GRAPHIC_VERTEXDATA_H_

#include <ugdk/graphic.h>
#include <ugdk/structure/types.h>

#include <cstddef>
#include <memory>
#include <typeinfo>

namespace ugdk {
namespace graphic {

class VertexData {
  public:
    class Mapper {
    public:
        Mapper(VertexData& data);
        ~Mapper();

        void operator=(Mapper&) = delete;

        template<class T>
        T* Get(std::size_t index) {
            Validate(typeid(T).name(), sizeof(T), index);
            return reinterpret_cast<T*>(mapped_ + index * data_.vertex_size());
        }

    private:
        VertexData& data_;
        ugdk::uint8* mapped_;

        void Validate(const char* name, std::size_t size, std::size_t index);
    };

    VertexData(std::size_t num_vertices, std::size_t vertex_size, bool dynamic, bool ignore_vbo = false);
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

struct VertexDataSpecification {
    VertexDataSpecification(std::size_t num, std::size_t size, bool is_dynamic)
        : num_vertices(num), vertex_size(size), dynamic(is_dynamic){}

    std::size_t num_vertices;
    std::size_t vertex_size;
    bool dynamic;
};

std::shared_ptr<VertexData> CreateVertexDataWithSpecification(const VertexDataSpecification&);

}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_VERTEXDATA_H_
