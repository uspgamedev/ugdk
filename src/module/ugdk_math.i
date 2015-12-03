
%module ugdk_math

%include <module/util_ugdk.swig>

%include "std_vector.i"

%{

#include <ugdk/math/vector2D.h>
#include <ugdk/math/integer2D.h>
#include <ugdk/math/frame.h>
#include <ugdk/math/geometry.h>

%}

%ignore ugdk::math::Vector2D::Rotate(Vector2D &a, double angle);
%ignore ugdk::math::operator*(const double  &scalar, const ugdk::math::Vector2D &right);

%newobject ugdk::math::Vector2D::Normalize() const;
%newobject ugdk::math::Vector2D::Rotate(const double angle) const;
%newobject ugdk::math::Vector2D::Scale(const ugdk::math::Vector2D &scale) const;
%newobject ugdk::math::Add(const ugdk::math::Vector2D &a, const ugdk::math::Vector2D &b);
%newobject ugdk::math::Subtract(const ugdk::math::Vector2D &a, const ugdk::math::Vector2D &b);
%newobject ugdk::math::Multiply(const ugdk::math::Vector2D &a, const double &scalar);
%newobject ugdk::math::Vector2D::operator+(const ugdk::math::Vector2D &right) const;
%newobject ugdk::math::Vector2D::operator-() const;
%newobject ugdk::math::Vector2D::operator-(const ugdk::math::Vector2D &right) const;
%newobject ugdk::math::Vector2D::operator*(const double   &scalar) const;
%newobject ugdk::math::Vector2D::operator/(const double   &scalar) const;
%newobject ugdk::math::Vector2D::operator*(const ugdk::math::Vector2D &right) const;

%include <ugdk/math/vector2D.h>
%template(Vector2DList) std::vector<ugdk::math::Vector2D>;

%include <ugdk/math/integer2D.h>

%include <ugdk/math/frame.h>

%newobject ugdk::math::Geometry::Compose(const Geometry* mod1, const Geometry* mod2);
%newobject ugdk::math::Geometry::Copy(const Geometry* mod2);
%rename(ComposeNew) ugdk::math::Geometry::Compose(const Geometry* mod1, const Geometry* mod2);

%include <ugdk/math/geometry.h>


namespace ugdk {
namespace math {
    export_class(Vector2D)
    export_class(Integer2D)
    export_class(Frame)
    export_class(Geometry)
}
}
 
confirm_exports(ugdk_math)
 
