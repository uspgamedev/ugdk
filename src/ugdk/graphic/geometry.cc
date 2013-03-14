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

void Geometry::AsMatrix4x4(float M[16]) const {
    double s = sin(rotation_), c = cos(rotation_);

    float R[16] = { float(scale_.x*c), -float(scale_.x*s), 0.0f, 0.0f, // First column
                    float(scale_.y*s),  float(scale_.y*c), 0.0f, 0.0f,
                          0.0f,        0.0f, 1.0f, 0.0f,
                    float(offset_.x),   float(offset_.y), 0.0f, 1.0f };
    //glTranslated(tx, ty, 0.0);
    //glRotated(apply->rotation() * 57.2957795, 0.0, 0.0, 1.0);
    //glScaled(sx, sy, 0.0);
    memcpy(M, R, sizeof(R));
}

}  // namespace graphic
}  // namespace ugdk
