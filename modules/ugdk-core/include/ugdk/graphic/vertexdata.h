#ifndef UGDK_GRAPHIC_VERTEXDATA_H_
#define UGDK_GRAPHIC_VERTEXDATA_H_

#include <ugdk/graphic.h>
#include <ugdk/structure/types.h>
#include <ugdk/system/exceptions.h>

#include <cstddef>
#include <memory>
#include <typeinfo>

namespace ugdk {
namespace graphic {

class VertexBuffer;
struct VertexDataSpecification;

/// Holds vertex data for rendering
class VertexData {
  public:
    template <typename T>
    class Mapper;

    /// Contructs empty data
    VertexData();

    /// Move constructor
    VertexData(VertexData&& rhs);

    /// Constructs vertex data
    explicit VertexData(std::size_t num_vertices, std::size_t vertex_size, bool dynamic,
                        bool ignore_vbo = false);

    /// Constructs vertex data from specification
    explicit VertexData(const VertexDataSpecification&);

    /// Destructor
    ~VertexData();

    /// Access internal vertex buffer #FIXME
    const std::unique_ptr<VertexBuffer>& buffer() const { return buffer_; }

    /// Get total vertex count
    std::size_t num_vertices() const { return num_vertices_; }

    /// Get individual vertex size
    std::size_t vertex_size() const { return vertex_size_; }

    /// Check whether given amount of data fits
    void CheckSizes(const char* caller_name, std::size_t test_num_vertices,
                    std::size_t test_vertex_size) const;

  private:
    template <typename T>
    friend class Mapper;

    /// Returns an array mapping the contents of the bufffer
    ugdk::uint8* Map(bool read_from_buffer);

    /// Unmaps data acquired from VertexData::Map, flushing to buffer
    void Unmap();

    std::unique_ptr<VertexBuffer> buffer_;
    std::size_t num_vertices_;
    std::size_t vertex_size_;
};

/// Used to read and write vertices in a VertexData object
template <typename T>
class VertexData::Mapper {
  public:
    /// Constructs mapper, attaching it to given VertexData object with RAII
    Mapper(VertexData& data, bool read_from_buffer = true) 
        : data_(data), mapped_(data.Map(read_from_buffer)) {}

    /// RAII destruction
    ~Mapper() {
        data_.Unmap();
    }

    /// Deleted copy operator
    void operator=(Mapper&) = delete;

    /// Accesses a specific chunk of vertex data as a T object
    T& Get(std::size_t index) {
        Validate(typeid(T).name(), sizeof(T), index);
        return *reinterpret_cast<T*>(mapped_ + index * data_.vertex_size());
    }

  private:
    void Validate(const char* name, std::size_t size, std::size_t index) const {
        if (size > data_.vertex_size())
            throw system::BaseException(
                "Incompatible type '%s' with size %u exceeds vertex buffer size of %u.",
                name, size, data_.vertex_size()
            );
        if (index >= data_.num_vertices())
            throw system::BaseException("Vertex %u is out of range. (Buffer has %u vertices)",
                                        index, data_.num_vertices());
    }

    VertexData& data_;
    ugdk::uint8* mapped_;

};

struct VertexDataSpecification {
    VertexDataSpecification(std::size_t num, std::size_t size, bool is_dynamic)
        : num_vertices(num), vertex_size(size), dynamic(is_dynamic){}

    std::size_t num_vertices;
    std::size_t vertex_size;
    bool dynamic;
};

inline std::shared_ptr<VertexData> CreateVertexDataWithSpecification(const VertexDataSpecification& specification) {
    return std::make_shared<VertexData>(specification);
}

}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_VERTEXDATA_H_
