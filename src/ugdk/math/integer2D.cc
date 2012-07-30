#include <cmath>
#include <ugdk/math/integer2D.h>

#define PI 3.14159265

namespace ugdk {
namespace math {

// Returns the norm-1.
int Integer2D::NormOne() const {
    return fabs(x) + fabs(y);
}

double Integer2D::Length() const {
    return sqrt(LengthSquared());
}

double Integer2D::Angle() const {
    return atan2(static_cast<double>(y), static_cast<double>(x));
}

void Integer2D::Rotate(RotDeg rotdeg) {
    double angle = 0.0;
    for (int i = rotdeg; i >= 0; i--) angle += PI;
    double ca = cos(angle), sa = sin(angle);   
    x = x * ca - y * sa;
    y = x * sa + y * ca;
}

Integer2D Integer2D::Rotated(RotDeg rotdeg) const {
    Integer2D ret = *this;
    ret.Rotate(rotdeg);
    return ret;
}

Integer2D Integer2D::HorizontalMirror() {
    return Integer2D(-x, y);
}

Integer2D Integer2D::VerticalMirror() {
    return Integer2D(x, -y);
}

Integer2D Integer2D::Scale(const Integer2D &scale) const {
    return Integer2D(x * scale.x, y * scale.y);
}

Integer2D& Integer2D::operator+=(const Integer2D &other) {
    x += other.x;  
    y += other.y;
    return *this;
}

Integer2D& Integer2D::operator-=(const Integer2D &other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

Integer2D& Integer2D::operator*=(int scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}
    
Integer2D& Integer2D::operator/=(int scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
}

Integer2D Integer2D::operator+(const Integer2D &right) const {
    Integer2D ret = *this;
    ret += right;
    return ret;
}

Integer2D Integer2D::operator-(const Integer2D &right) const {
    Integer2D ret = *this;
    ret -= right;
    return ret;
}

Integer2D Integer2D::operator-() const {
    return Integer2D(-x, -y);
}

Integer2D Integer2D::operator*(int scalar) const {
    Integer2D ret = *this;
    ret *= scalar;
    return ret;
}

Integer2D Integer2D::operator/(int scalar) const {
    Integer2D ret = *this;
    ret /= scalar;
    return ret;
}

int Integer2D::operator*(const Integer2D &right) const {
    return x * right.x + y * right.y;
}

///

Integer2D operator*(int scalar, const Integer2D &right) {
    return right * scalar;
}

}  // namespace math
}  // namespace ugdk
