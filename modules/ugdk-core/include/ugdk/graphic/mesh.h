
#ifndef UGDK_GRAPHIC_MESH_H_
#define UGDK_GRAPHIC_MESH_H_

#include <ugdk/structure/vertex.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/vertexdata.h>
#include <ugdk/graphic.h>

#include <glm/vec2.hpp>

#include <vector>

namespace ugdk {
namespace graphic {

class Canvas;

// TODO:
// + [x] Add fill methods
// + [x] Write stream method
// Remember:
// + VertexData will be used internally to group buffers
// + Mesh is used by user for easy access

template <typename Vertex>
class Mesh {
  public:
    /// Alias
    using VertexArray = std::vector<Vertex>;

    /// Default constructor (an invalid mesh)
    Mesh();

    /// Construct mesh with given draw mode
    Mesh(const DrawMode& the_mode, GLTexture* the_texture);

    /// Tells whether the Mesh is valid
    bool valid() const;

    /// 
    void Fill(const VertexArray& vertices);

    void StreamTo(Canvas &canvas) const;
    
  private:
    DrawMode            mode_;
    GLTexture           *texture_;
    std::vector<Vertex> vertices_;
};

using Mesh2D = Mesh<structure::VertexXYUV>;

template <typename Vertex>
inline Mesh<Vertex>::Mesh() : Mesh(DrawMode::TRIANGLES(), nullptr) {}

template <typename Vertex>
inline Mesh<Vertex>::Mesh(const DrawMode& the_mode, GLTexture* the_texture)
    : mode_(the_mode), texture_(the_texture) {}

template <typename Vertex>
inline bool Mesh<Vertex>::valid() const {
    return static_cast<bool>(this->texture_);
}

template <typename Vertex>
inline void Mesh<Vertex>::Fill(const Mesh<Vertex>::VertexArray& vertices) {
    vertices_ = vertices;
}

template <typename Vertex>
Canvas& operator<<(Canvas &canvas, const Mesh<Vertex> &mesh) {
    mesh.StreamTo(canvas);
    return canvas;
}

/// See https://stackoverflow.com/questions/8854286/how-to-enforce-use-of-template-specialization
template<typename T> struct fake_dependency: public std::false_type {};

template <typename Vertex>
void Mesh<Vertex>::StreamTo(Canvas &canvas) const {
    static_assert(fake_dependency<Vertex>::value, "must use specialization");
}

template <>
void Mesh<structure::VertexXYUV>::StreamTo(Canvas &canvas) const {
    VertexData vtxdata(this->vertices_.size(), sizeof(structure::VertexXYUV), false, true);
    {
        VertexData::Mapper<structure::VertexXYUV> map(vtxdata);
        for (size_t i = 0; i < this->vertices_.size(); i++)
            map.Get(i) = this->vertices_[i];
    }
    graphic::TextureUnit unit = graphic::manager().ReserveTextureUnit(this->texture_);
    canvas.SendUniform("drawable_texture", unit);
    canvas.SendVertexData(vtxdata, graphic::VertexType::VERTEX, 0, 2);
    canvas.SendVertexData(vtxdata, graphic::VertexType::TEXTURE, 2 * sizeof(F32), 2);
    canvas.DrawArrays(this->mode_, 0, this->vertices_.size());
}

} // namespace ugdk
} // namespace graphic

#endif // UGDK_GRAPHIC_MESH_H_
