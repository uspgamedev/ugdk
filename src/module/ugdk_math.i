
%module ugdk_math

%include <module/export.swig>
%include "std_vector.i"

%{

#include <ugdk/math/vector2D.h>
#include <ugdk/math/frame.h>

%}

%ignore ugdk::math::Vector2D::Rotate(Vector2D &a, double angle);
%ignore ugdk::math::operator*(const double  &scalar, const Vector2D &right);

%newobject ugdk::math::Vector2D::Normalize() const;
%newobject ugdk::math::Vector2D::Rotate(const double angle) const;
%newobject ugdk::math::Vector2D::Scale(const Vector2D &scale) const;
%newobject ugdk::math::Add(const Vector2D &a, const Vector2D &b);
%newobject ugdk::math::Subtract(const Vector2D &a, const Vector2D &b);
%newobject ugdk::math::Multiply(const Vector2D &a, const double &scalar);
%newobject ugdk::math::Vector2D::operator+(const Vector2D &right) const;
%newobject ugdk::math::Vector2D::operator-() const;
%newobject ugdk::math::Vector2D::operator-(const Vector2D &right) const;
%newobject ugdk::math::Vector2D::operator*(const double   &scalar) const;
%newobject ugdk::math::Vector2D::operator/(const double   &scalar) const;
%newobject ugdk::math::Vector2D::operator*(const Vector2D &right) const;

%include <ugdk/math/vector2D.h>
%template(Vector2DList) std::vector<ugdk::math::Vector2D>;

%include <ugdk/math/frame.h>

namespace ugdk {
namespace math {
    export_class(Vector2D)
    export_class(Frame)
}
}
 
confirm_exports(ugdk_math)
 
