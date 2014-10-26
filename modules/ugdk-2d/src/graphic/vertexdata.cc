#include <ugdk/graphic/vertexdata.h>
#include <ugdk/system/LoveException.h>

#include "vertexbuffer.h"

namespace ugdk {
namespace graphic {

VertexData::Mapper::Mapper(VertexData& data)
: data_(data) 
{
    data.buffer()->bind();
    mapped_ = static_cast<uint8*>(data.buffer()->map());
    internal::AssertNoOpenGLError();
}

VertexData::Mapper::~Mapper() {
    data_.buffer()->unmap();
    data_.buffer()->unbind();
}
        
void VertexData::Mapper::Validate(const char* name, std::size_t size, std::size_t index) {
    if (size > data_.vertex_size())
        throw love::Exception("Incompatible type '%s' with size %u exceeds vertex buffer size of %u.", name, size, data_.vertex_size());
    
    if (index >= data_.num_vertices())
        throw love::Exception("Vertex %u is out of range. (Buffer has %u vertices)", index, data_.num_vertices());
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

VertexData::~VertexData() {}
    
void VertexData::CheckSizes(const char* caller_name, std::size_t test_num_vertices, std::size_t test_vertex_size) const {
    if (test_num_vertices > 0 && num_vertices_ < test_num_vertices)
        throw love::Exception("Unsufficient vertices for %s. Needs %u, found %u.", caller_name, test_num_vertices, num_vertices_);

    if (test_vertex_size > 0 && vertex_size_ < test_vertex_size)
        throw love::Exception("Unsufficient vertex size for %s. Needs %u bytes, found %u.", caller_name, test_vertex_size, vertex_size_);
}

std::shared_ptr<VertexData> CreateVertexDataWithSpecification(const VertexDataSpecification& specification) {
    return std::make_shared<VertexData>(specification.num_vertices, specification.vertex_size, specification.dynamic);
}

}  // namespace graphic
}  // namespace ugdk
