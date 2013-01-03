#include <ugdk/graphic/geometry.h>

#include <cmath>
#include <cstring>

namespace ugdk {
namespace graphic {

Geometry::Geometry() : offset_(), scale_(1.0, 1.0), rotation_(0.0) {}

void Geometry::Compose(const Geometry& rhs) {
    offset_ += rhs.offset_.Rotate(rotation_).Scale(scale_);
    rotation_ += rhs.rotation_;
    scale_ = scale_.Scale(rhs.scale_); 
}

void Geometry::AsMatrix4x4(double M[16]) const {
    double tx = offset_.x, ty = offset_.y;
    double sx = scale_.x, sy = scale_.y;
    double s = sin(rotation_), c = cos(rotation_);

    double R[16] = { sx*c, -sx*s, 0.0, 0.0, // First column
                     sy*s,  sy*c, 0.0, 0.0,
                      0.0,   0.0, 1.0, 0.0,
                       tx,    ty, 0.0, 1.0 };
    //glTranslated(tx, ty, 0.0);
    //glRotated(apply->rotation() * 57.2957795, 0.0, 0.0, 1.0);
    //glScaled(sx, sy, 0.0);
    memcpy(M, R, sizeof(R));
}

}  // namespace graphic
}  // namespace ugdk
