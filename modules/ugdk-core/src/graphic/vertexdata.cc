#include <ugdk/graphic/vertexdata.h>
#include <ugdk/system/exceptions.h>

#include "vertexbuffer.h"

namespace ugdk {
namespace graphic {

VertexData::Mapper::Mapper(VertexData& data, bool read_from_buffer)
: data_(data)
{
    data.buffer()->bind();
    mapped_ = static_cast<uint8*>(data.buffer()->map(read_from_buffer));
    internal::AssertNoOpenGLError();
}

VertexData::Mapper::~Mapper() {
    data_.buffer()->unmap();
    data_.buffer()->unbind();
}

void VertexData::Mapper::Validate(const char* name, std::size_t size, std::size_t index) const {
    if (size > data_.vertex_size())
        throw system::BaseException("Incompatible type '%s' with size %u exceeds vertex buffer size of %u.", name, size, data_.vertex_size());

    if (index >= data_.num_vertices())
        throw system::BaseException("Vertex %u is out of range. (Buffer has %u vertices)", index, data_.num_vertices());
}

VertexData::VertexData() : num_vertices_(0u), vertex_size_(0u) {}

VertexData::VertexData(VertexData&& rhs)
    : buffer_(std::move(rhs.buffer_)), num_vertices_(rhs.num_vertices_), vertex_size_(rhs.vertex_size_) {
    rhs.num_vertices_ = 0u;
    rhs.vertex_size_ = 0u;
}

VertexData::VertexData(std::size_t num_vertices, std::size_t vertex_size, bool dynamic, bool ignore_vbo)
:   buffer_(ignore_vbo ?
            (new VertexArray(num_vertices * vertex_size,
                                        GL_ARRAY_BUFFER,
                                        dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW)) :
            (VertexBuffer::Create(num_vertices * vertex_size,
                                        GL_ARRAY_BUFFER,
                                        dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW)))
,   num_vertices_(num_vertices)
,   vertex_size_(vertex_size)
{
    assert(num_vertices > 0);
    assert(vertex_size > 0);
}

VertexData::VertexData(const VertexDataSpecification& specification)
    : VertexData(specification.num_vertices, specification.vertex_size, specification.dynamic)
{}

VertexData::~VertexData() {}

void VertexData::CheckSizes(const char* caller_name, std::size_t test_num_vertices, std::size_t test_vertex_size) const {
    if (test_num_vertices > 0 && num_vertices_ < test_num_vertices)
        throw system::BaseException("Unsufficient vertices for %s. Needs %u, found %u.", caller_name, test_num_vertices, num_vertices_);

    if (test_vertex_size > 0 && vertex_size_ < test_vertex_size)
        throw system::BaseException("Unsufficient vertex size for %s. Needs %u bytes, found %u.", caller_name, test_vertex_size, vertex_size_);
}

}  // namespace graphic
}  // namespace ugdk
