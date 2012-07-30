#include <cmath>
#include <assert.h>
#include <ugdk/math/integer2D.h>
#include <ugdk/math/vector2D.h>

#define SWAPVARS(temp,var1,var2) temp = var1; var1 = var2; var2 = temp

namespace ugdk {
namespace math {

Integer2D::Integer2D(const ugdk::Vector2D& vec2d)
  : x(static_cast<int>(vec2d.x)), y(static_cast<int>(vec2d.y)) {}

// Returns the norm-1.
int Integer2D::NormOne() const {
    return abs(x) + abs(y);
}

double Integer2D::Length() const {
    return sqrt(LengthSquared());
}

double Integer2D::Angle() const {
    return atan2(static_cast<double>(y), static_cast<double>(x));
}

void Integer2D::Rotate(RotDeg rotdeg) {
    int temp;

    switch(rotdeg) {
        case NINETY: // (x, y) = (y, -x)
        SWAPVARS(temp,x,y);
        y = -y;
        break;

        case TWO_NINETY: // (x, y) = (-x, -y)
        x = -x;
        y = -y;
        break;

        case THREE_NINETY: // (x, y) = (-y, x)
        SWAPVARS(temp,x,y);
        x = -x;
        break;
    }
}

Integer2D Integer2D::Rotated(RotDeg rotdeg) const {
    Integer2D ret = *this;
    ret.Rotate(rotdeg);
    return ret;
}


void Integer2D::Mirror(const ugdk::Mirror& mirror) {
    switch(mirror) {
        case MIRROR_HFLIP:  y = -y;         break;
        case MIRROR_VFLIP:  x = -x;         break;
        case MIRROR_HVFLIP: x = -x; y = -y; break;
        case MIRROR_NONE: // do nothing.
        default: break;
    }
}

Integer2D Integer2D::Mirrored(const ugdk::Mirror& mirror) const {
    Integer2D ret = *this;
    ret.Mirror(mirror);
    return ret;
}


void Integer2D::Multiply(const Integer2D& multiplier) {
    x *= multiplier.x;
    y *= multiplier.y;
}

Integer2D Integer2D::Multiplied(const Integer2D& multiplier) const {
    Integer2D ret = *this;
    ret.Multiply(multiplier);
    return ret;
}

void Integer2D::Divide(const Integer2D& divisor) {
    assert(divisor.x != 0 && divisor.y != 0);

    x /= divisor.x;
    y /= divisor.y;
}

Integer2D Integer2D::Divided(const Integer2D& divisor) const {
    assert(divisor.x != 0 && divisor.y != 0);

    Integer2D ret = *this;
    ret.Divide(divisor);
    return ret;
}

void Integer2D::Scale(const Integer2D& multiplier, const Integer2D& divisor) {
    assert(divisor.x != 0 && divisor.y != 0);

    this->Multiply(multiplier);
    this->Divide(divisor);
}

Integer2D Integer2D::Scaled(const Integer2D& multiplier, const Integer2D& divisor) const {
    assert(divisor.x != 0 && divisor.y != 0);

    Integer2D ret = *this;
    ret.Scale(multiplier,divisor);
    return ret;
}

void Integer2D::Mod(const Integer2D& divisor) {
    assert(divisor.x != 0 && divisor.y != 0);

    x %= divisor.x;
    y %= divisor.y;
}

Integer2D Integer2D::Remainder(const Integer2D& divisor) const {
    assert(divisor.x != 0 && divisor.y != 0);

    Integer2D ret = *this;
    ret.Mod(divisor);
    return ret;
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
    assert( scalar != 0 );

    x /= scalar;
    y /= scalar;
    return *this;
}

Integer2D& Integer2D::operator%=(int scalar) {
    assert ( scalar != 0 );

    x %= scalar;
    y %= scalar;
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
    assert( scalar != 0 );

    Integer2D ret = *this;
    ret /= scalar;
    return ret;
}

Integer2D Integer2D::operator%(int scalar) const {
    assert( scalar != 0 );

    Integer2D ret = *this;
    ret %= scalar;
    return ret;
}

int Integer2D::operator*(const Integer2D &right) const {
    return x * right.x + y * right.y;
}

//-//

Integer2D operator*(int scalar, const Integer2D &right) {
    return right * scalar;
}

}  // namespace math
}  // namespace ugdk
