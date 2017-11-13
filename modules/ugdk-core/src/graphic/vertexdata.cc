#include <ugdk/graphic/vertexdata.h>
#include <ugdk/system/exceptions.h>

#include "vertexbuffer.h"

namespace ugdk {
namespace graphic {

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

ugdk::uint8* VertexData::Map(bool read_from_buffer) {
    buffer_->bind();
    ugdk::uint8* mapped = static_cast<uint8*>(buffer_->map(read_from_buffer));
    internal::AssertNoOpenGLError();
    return mapped;
}

void VertexData::Unmap() {
    buffer_->unmap();
    buffer_->unbind();
}

}  // namespace graphic
}  // namespace ugdk
