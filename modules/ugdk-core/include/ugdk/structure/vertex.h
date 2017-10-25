
#ifndef UGDK_STRUCTURE_VERTEX_H_
#define UGDK_STRUCTURE_VERTEX_H_

#include <ugdk/structure/types.h>

#include <glm/vec2.hpp>

namespace ugdk {
namespace structure {

struct VertexXYUV {

    F32 x;
    F32 y;
    F32 u;
    F32 v;
    
    void set_xyuv(float _x, float _y, float _u, float _v) {
        x = _x;
        y = _y;
        u = _u;
        v = _v;
    }
    void set(const glm::vec2& xy) {
        x = xy.x;
        y = xy.y;
    }
    void set(const glm::vec2& xy, const glm::vec2& uv) {
        set(xy);
        u = uv.x;
        v = uv.y;
    }
};

} // namespace structure
} // namespace ugdk

#endif // UGDK_STRUCTURE_VERTEX_H_
