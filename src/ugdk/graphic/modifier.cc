#include <sstream>
#include <algorithm>
#include <cmath>

#include <ugdk/graphic/modifier.h>

namespace ugdk {

namespace graphic {

Modifier::Modifier() : offset_(), scale_(1.0, 1.0), rotation_(0.0) {}

void Modifier::Compose(const Modifier& rhs) {

}

void Modifier::AsMatrix4x4(double M[16]) const {
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
