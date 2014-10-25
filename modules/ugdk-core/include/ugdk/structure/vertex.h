#ifndef UGDK_STRUCTURE_VERTEX_H_
#define UGDK_STRUCTURE_VERTEX_H_

namespace ugdk {
namespace structure {

struct VertexXYUV {
    float x;
    float y;
    float u;
    float v;
    
    void set_xyuv(float _x, float _y, float _u, float _v) {
        x = _x;
        y = _y;
        u = _u;
        v = _v;
    }
};

} // namespace structure
} // namespace ugdk

#endif // UGDK_STRUCTURE_VERTEX_H_
