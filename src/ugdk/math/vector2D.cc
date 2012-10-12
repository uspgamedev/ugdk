
#include <ugdk/math/vector2D.h>

#include <cmath>
#include <assert.h>
#include <ugdk/math/integer2D.h>

#define SWAPVARS(temp,var1,var2) temp = var1; var1 = var2; var2 = temp

using namespace ugdk::enums;
using mirroraxis::MirrorAxis;

namespace ugdk {
namespace math {

Vector2D::Vector2D(const ugdk::math::Integer2D& int2d)
  : x(static_cast<double>(int2d.x)), y(static_cast<double>(int2d.y)) {}

// Returns the norm-1.
double Vector2D::NormOne() const {
    return fabs(x) + fabs(y);
}

double Vector2D::Length() const {
    return sqrt(LengthSquared());
}

double Vector2D::Angle() const {
    return atan2(y, x);
}

Vector2D Vector2D::Normalize() const {
    double norm = Length();
    if (norm == 0) {
        return *this;
    }
    return (*this) / norm;
}

Vector2D Vector2D::Rotate(const double angle) const {
    double ca = cos(angle), sa = sin(angle);   
    return Vector2D(x * ca - y * sa, x * sa + y * ca);
}

void Vector2D::Mirror(const MirrorAxis mirror) {
    double temp;

    switch(mirror) {
        case mirroraxis::HORZ:      y = -y;                             break;
        case mirroraxis::DIAG_UP:   SWAPVARS(temp,x,y);                 break;
        case mirroraxis::VERT:      x = -x;                             break;
        case mirroraxis::DIAG_DOWN: SWAPVARS(temp,x,y); x = -x; y = -y; break;
        default: assert(false); break;
    }
}

Vector2D Vector2D::Mirrored(const MirrorAxis mirror) const {
    Vector2D ret = *this;
    ret.Mirror(mirror);
    return ret;
}

Vector2D Vector2D::Scale(const Vector2D &scale) const {
    return Vector2D(x * scale.x, y * scale.y);
}

Vector2D& Vector2D::operator+=(const Vector2D &other) {
    x += other.x;  
    y += other.y;
    return *this;
}

Vector2D& Vector2D::operator-=(const Vector2D &other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

Vector2D Vector2D::operator+(const Vector2D &right) const {
    return Vector2D::Add(*this, right);
}
Vector2D Vector2D::operator-(const Vector2D &right) const {
    return Vector2D::Subtract(*this, right);
}
Vector2D Vector2D::operator-() const {
    return Vector2D(-this->x, -this->y);
}

Vector2D Vector2D::operator*(const double   &scalar) const {
    return Vector2D::Multiply(*this, scalar);
}
Vector2D Vector2D::operator/(const double   &scalar) const {
    return Vector2D::Multiply(*this, 1.0 / scalar);
}
double Vector2D::operator*(const Vector2D &right) const {
    return Vector2D::InnerProduct(*this, right);
}

///

Vector2D operator*(const double  &scalar, const Vector2D &right) {
    return Vector2D::Multiply(right, scalar);
}

} // namespace math
} // namespace ugdk
