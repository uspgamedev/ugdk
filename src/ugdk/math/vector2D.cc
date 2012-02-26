#include <math.h>
#include <ugdk/math/vector2D.h>

namespace ugdk {

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

}  // namespace ugdk
