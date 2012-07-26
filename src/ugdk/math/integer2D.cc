#include <math.h>
#include <ugdk/math/integer2D.h>

#define PI 3.14159265

namespace ugdk {

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

void Integer2D::Rotate(RotDeg rotdeg) const {
    double angle = 0.0;
    for (int i = rotdeg; i >= 0; i--) angle += PI;
    double ca = cos(angle), sa = sin(angle);   
    x = x * ca - y * sa;
    y = x * sa + y * ca;
}

Integer2D Integer2D::Rotated(RotDeg rotdeg) const {
    return Integer2D(x, y).rotate(rotdeg); 
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

Integer2D Integer2D::operator+(const Integer2D &right) const {
    return Integer2D::Add(*this, right);
}
Integer2D Integer2D::operator-(const Integer2D &right) const {
    return Integer2D::Subtract(*this, right);
}
Integer2D Integer2D::operator-() const {
    return Integer2D(-this->x, -this->y);
}

Integer2D Integer2D::operator*(const int   &scalar) const {
    return Integer2D::Multiply(*this, scalar);
}
Integer2D Integer2D::operator/(const int   &scalar) const {
    return Integer2D::Multiply(*this, 1.0 / scalar);
}
int Integer2D::operator*(const Integer2D &right) const {
    return Integer2D::InnerProduct(*this, right);
}

///

Integer2D operator*(const int  &scalar, const Integer2D &right) {
    return Integer2D::Multiply(right, scalar);
}

}  // namespace ugdk
